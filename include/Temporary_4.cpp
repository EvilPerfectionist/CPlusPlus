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
    }
    else {
        std::cout << "could not find model file: " << endl;
    }
}