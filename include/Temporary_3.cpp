#include <Temporary_3.h>

Renderer::Renderer(const char *rootDirectory) {
    char file[200];
    // create color program
    string src;
    sprintf(file, "%s/shader/color.vertexshader", rootDirectory);
    loadShaderCodeFromFile(file, src);
    compileShader(src, GL_VERTEX_SHADER, shader["color_vertex"]);

    sprintf(file, "%s/shader/color.fragmentshader", rootDirectory);
    loadShaderCodeFromFile(file, src);
    compileShader(src, GL_FRAGMENT_SHADER, shader["color_fragment"]);

    sprintf(file, "%s/shader/color.vertexshader", rootDirectory);
    loadShaderCodeFromFile(file, src);
    compileShader(src, GL_VERTEX_SHADER, shader["color_vertex_simple"]);

    sprintf(file, "%s/shader/color_simple.fragmentshader", rootDirectory);
    loadShaderCodeFromFile(file, src);
    compileShader(src, GL_FRAGMENT_SHADER, shader["color_fragment_simple"]);

    sprintf(file, "%s/shader/Model_Loading.vs", rootDirectory);
    loadShaderCodeFromFile(file, src);
    compileShader(src, GL_VERTEX_SHADER, shader["Model_Loading_vertex"]);

    sprintf(file, "%s/shader/Model_Loading.fs", rootDirectory);
    loadShaderCodeFromFile(file, src);
    compileShader(src, GL_FRAGMENT_SHADER, shader["Model_Loading_fragment"]);

    if (createRenderProgram(shader["color_vertex"], shader["color_fragment"], program["color"]) == GL_FALSE)
        return;
    if (createRenderProgram(shader["color_vertex_simple"], shader["color_fragment_simple"], program["color_simple"]) == GL_FALSE)
        return;
    if (createRenderProgram(shader["Model_Loading_vertex"], shader["Model_Loading_fragment"], program["Model_Loading"]) == GL_FALSE)
        return;

    MatrixID["color"] = glGetUniformLocation(program["color"], "MVP");
    ViewMatrixID["color"] = glGetUniformLocation(program["color"], "ViewMatrix");
    ModelMatrixID["color"] = glGetUniformLocation(program["color"], "ModelMatrix");
    LightPositionID["color"] = glGetUniformLocation(program["color"], "LightPosition_worldspace");

    MatrixID["color_simple"] = glGetUniformLocation(program["color_simple"], "MVP");
    ViewMatrixID["color_simple"] = glGetUniformLocation(program["color_simple"], "ViewMatrix");
    ModelMatrixID["color_simple"] = glGetUniformLocation(program["color_simple"], "ModelMatrix");
    LightPositionID["color_simple"] = glGetUniformLocation(program["color_simple"], "LightPosition_worldspace");

    model_ID["Model_Loading"] = glGetUniformLocation(program["Model_Loading"], "model");
    view_ID["Model_Loading"] = glGetUniformLocation(program["Model_Loading"], "view");
    projection_ID["Model_Loading"] = glGetUniformLocation(program["Model_Loading"], "projection");

    Mat cameraMatrix, distCoeffs;
    sprintf(file, "%s/intrinsics.xml", rootDirectory);
    cv::FileStorage fs(file, cv::FileStorage::READ);
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    fs.release();

    // calculate undistortion mapping
    Mat img_rectified, map1, map2;
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                            getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, cv::Size(WIDTH, HEIGHT), 1,
                                                      cv::Size(WIDTH, HEIGHT), 0),
                            cv::Size(WIDTH, HEIGHT), CV_16SC2, map1, map2);

    ViewMatrix = Matrix4f::Identity();
    ViewMatrix.topRightCorner(3,1) << 0,0,-1;

    float n = 0.01; // near field
    float f = 1000; // far field
    ProjectionMatrix << cameraMatrix.at<double>(0, 0) / cameraMatrix.at<double>(0, 2), 0.0, 0.0, 0.0,
            0.0, cameraMatrix.at<double>(1, 1) / cameraMatrix.at<double>(1, 2), 0.0, 0.0,
            0.0, 0.0, -(f + n) / (f - n), (-2.0f * f * n) / (f - n),
            0.0, 0.0, -1.0, 0.0;
    K << cameraMatrix.at<double>(0, 0), cameraMatrix.at<double>(0, 1), cameraMatrix.at<double>(0, 2),
            cameraMatrix.at<double>(1, 0), cameraMatrix.at<double>(1, 1), cameraMatrix.at<double>(1, 2),
            cameraMatrix.at<double>(2, 0), cameraMatrix.at<double>(2, 1), cameraMatrix.at<double>(2, 2);
    cout << "K\n" << K << endl;
    Kinv = K.inverse();

    // background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
}

Renderer::~Renderer() {
    for (auto p:program)
        glDeleteProgram(p.second);
    for (auto t:tbo)
        glDeleteBuffers(1, &t.second);
}

bool Renderer::loadShaderCodeFromFile(const char *file_path, string &src) {
    src.clear();
    ifstream VertexShaderStream(file_path, ios::in);
    if (VertexShaderStream.is_open()) {
        string Line = "";
        while (getline(VertexShaderStream, Line))
            src += "\n" + Line;
        VertexShaderStream.close();
        printf("Successfully read %s\n", file_path);
        return true;
    } else {
        printf("Cannot read %s\n", file_path);
        getchar();
        return false;
    }
}

void Renderer::compileShader(string src, GLenum type, GLuint &shader) {
    shader = glCreateShader(type);
    const char *c_str = src.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);
}

GLint Renderer::createRenderProgram(GLuint &vertex_shader, GLuint &fragment_shader, GLuint &program) {
    // Link the program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    printf("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
    printf("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
    printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);

    GLint Result = GL_FALSE;
    int InfoLogLength;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &Result);
    cout << "Result is " << Result << endl;
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(vertex_shader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
        return Result;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(fragment_shader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
        return Result;
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return Result;
}

void Renderer::renderColor(Mesh *mesh, VectorXd &pose, string program) {
    Matrix3f rot = Matrix3f::Identity();
    Vector3f p(pose(3), pose(4), pose(5));
    float angle = p.norm();
    if (abs(angle) > 0.0000001) {
        p.normalize();
        Matrix3f skew;
        skew << 0, -p(2), p(1),
                p(2), 0, -p(0),
                -p(1), p(0), 0;
        rot = rot + sin(angle) * skew;
        rot = rot + (1.0 - cos(angle)) * skew * skew;
    }

    ViewMatrix.topLeftCorner(3,3) = rot;
    ViewMatrix.topRightCorner(3,1) << pose(0), pose(1), pose(2);

    renderColor(mesh, program);
}

void Renderer::renderColor(Mesh *mesh, string programName) {
    Eigen::Matrix4f MVP = ProjectionMatrix * ViewMatrix * mesh->ModelMatrix;

    // background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glUseProgram(program[programName]);
    glUniformMatrix4fv(ViewMatrixID[programName], 1, GL_FALSE, &ViewMatrix(0, 0));
    glUniformMatrix4fv(MatrixID[programName], 1, GL_FALSE, &MVP(0, 0));
    glUniformMatrix4fv(ModelMatrixID[programName], 1, GL_FALSE, &mesh->ModelMatrix(0, 0));
    static float x = 0;
//    Vector3f lightPosition;
//    lightPosition = ViewMatrix.topRightCorner(3,1);
//    glUniform3fv(LightPositionID[programName], 1, &lightPosition(0));

    mesh->Render();
}

void Renderer::renderColor_OBJ(Model_OBJ *obj, string programName) {
    GLuint ID = program[programName];

    // background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glUseProgram(program[programName]);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    //glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    glUniformMatrix4fv(projection_ID[programName], 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 view = glm::lookAt(
            glm::vec3(4.0f, 5.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
    );
    glUniformMatrix4fv(view_ID[programName], 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model_matrix = glm::scale(model_matrix, glm::vec3(5.2f, 5.2f, 5.2f));	// it's a bit too big for our scene, so scale it down
    glUniformMatrix4fv(model_ID[programName], 1, GL_FALSE, glm::value_ptr(model_matrix));
    obj->Draw(ID);
}

void Renderer::getImage(Mat &img){
    // get the image from opengl buffer
    GLubyte data[4 * WIDTH * HEIGHT]; // it should be 3 because of BGR, but using 3 results in strange border...
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, data);
    Mat tmp = cv::Mat(HEIGHT, WIDTH, CV_8UC3, data);
    flip(tmp, tmp, 0);
    tmp.copyTo(img);
}
