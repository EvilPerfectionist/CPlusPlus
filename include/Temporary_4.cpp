#include <Temporary_4.h>

Model::Model(const char* rootDirectory, const char* modelFile, bool withPoseEstimation) {
    filesystem = new FileSystem(rootDirectory);

    renderer = new Renderer(rootDirectory);

    path fullFilePath;

    if (filesystem->find(modelFile, &fullFilePath)) {
        if (strcmp(fullFilePath.extension().c_str(), ".sdf") == 0) {
            std::cout << "success " << endl;

            std::ifstream stream(fullFilePath.c_str());
            std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
            sdf::SDF sdf;
            sdf.SetFromString(str);

            if (sdf.Root()->HasElement("model")) {
                printf("contain a model\n");
                sdf::ElementPtr modelElem = sdf.Root()->GetElement("model");
                // Read name attribute value
                if (modelElem->HasAttribute("name")) {
                    sdf::ParamPtr nameParam = modelElem->GetAttribute("name");
                    cout << "loading " << nameParam->GetAsString() << endl;
                    sdf::ElementPtr linkElem = modelElem->GetElement("link");
                    while (linkElem != NULL) {
                        if (linkElem->HasElement("visual")) {
                            sdf::ElementPtr uriElem = linkElem->GetElement("visual")->GetElement(
                                    "geometry")->GetElement(
                                    "mesh")->GetElement("uri");
                            sdf::ElementPtr poseElem = linkElem->GetElement("pose");
                            if (poseElem != NULL && uriElem != NULL &&
                                strcmp(uriElem->GetValue()->GetAsString().c_str(), "__default__") != 0) {
                                char file[200];
                                sprintf(file, "%s", uriElem->GetValue()->GetAsString().c_str() + 8);
                                std::cout << "File = " << file << "\n";
                                float x, y, z, roll, pitch, yaw;
                                if (sscanf(poseElem->GetValue()->GetAsString().c_str(), "%f %f %f %f %f %f", &x, &y, &z,
                                           &roll,
                                           &pitch, &yaw) != 6)
                                    printf("error reading pose parameters\n");
                                else {
                                    char modelPath[200];
                                    sprintf(modelPath, "%s/models/%s", rootDirectory, file);
                                    std::cout << "ModelPath = " << modelPath << "\n";
                                    path p(modelPath);
                                    if(exists(p) && is_regular_file(p)) {
                                        Mesh *mesh = new Mesh;
                                        mesh->name = linkElem->GetAttribute("name")->GetAsString();
                                        cout << "Mesh_Name is " << mesh->name << endl;
                                        VectorXf pose(6);
                                        pose << x, y, z, roll, pitch, yaw;
                                        cout << "Pose is " << pose << endl;
                                        mesh->LoadMesh(modelPath, pose);
                                        meshes.push_back(mesh);
                                    }else{
                                        printf("error loading modelfile %s\n", modelPath);
                                    }
                                }
                                printf("Pose_Numbers\n");
                            }
                            printf("Visual_Numbers\n");
                        }
                        linkElem = linkElem->GetNextElement();
                        printf("Link_Numbers\n");
                    }
                }
            }
            else{
                printf("does not contain a model\n");
            }
        }
        else if(strcmp(fullFilePath.extension().c_str(), ".dae") == 0) {
            Mesh *mesh = new Mesh;
            mesh->name = fullFilePath.stem().string();
            mesh->LoadMesh(fullFilePath.c_str());
            meshes.push_back(mesh);
        }
        else if(strcmp(fullFilePath.extension().c_str(), ".obj") == 0) {
            Mesh *mesh = new Mesh;
            mesh->name = fullFilePath.stem().string();
            mesh->LoadMesh(fullFilePath.c_str());
            meshes.push_back(mesh);
        }
        else{
            cout << "unknown model format, please provide either sdf or dae file" << endl;
        }
    }
    else {
        std::cout << "could not find model file: " << endl;
    }
}

Model::~Model(){
    for(uint mesh=0;mesh<meshes.size();mesh++)
        delete meshes[mesh];
    delete renderer;
    delete filesystem;
}

void Model::render(Mat &img, bool clear, string program){
    if(clear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(uint mesh=0; mesh<meshes.size(); mesh++){
        renderer->renderColor(meshes[mesh], program);
    }
    return renderer->getImage(img);
}

void Model::updateViewMatrix(sf::Window &window){
    float speed_trans = 0.01f , speed_rot = 0.001f;

    // Get mouse position
    sf::Vector2i windowsize = sf::Vector2i(window.getSize().x, window.getSize().y);
    double xpos, ypos;
    Matrix3f rot = Matrix3f::Identity();
    static float horizontalAngle = 0;
    static float verticalAngle = 0;

    static bool sticky = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        sticky = !sticky;
    }

    if(sticky) {
        sf::Vector2i mousepos = sf::Mouse::getPosition(window);
        sf::Vector2i delta = windowsize/2-mousepos;
        if(delta.x != 0 || delta.y != 0)
        {
            // set cursor to window center
            sf::Mouse::setPosition(windowsize/2, window);
            // Compute new orientation
            horizontalAngle -= speed_rot * float(delta.x);
            verticalAngle += speed_rot * float(delta.y);
        }
    }

    rot = Eigen::AngleAxisf(horizontalAngle, Vector3f::UnitY()) *
          Eigen::AngleAxisf(verticalAngle, Vector3f::UnitX());

    Vector3f direction = Vector3f::UnitZ();
    Vector3f right = Vector3f::UnitX();

    static Vector3f dcameraPos(0,0,-1);
    // Move forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        dcameraPos += direction  * speed_trans;
    }
    // Move backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        dcameraPos -= direction * speed_trans;
    }
    // Strafe right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        dcameraPos -= right * speed_trans;
    }
    // Strafe left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        dcameraPos += right * speed_trans;
    }

    Matrix4f RT = Matrix4f::Identity();
    RT.topLeftCorner(3,3) = rot;
    RT.topRightCorner(3,1) = dcameraPos;

    renderer->ViewMatrix = RT;
}