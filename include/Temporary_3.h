#ifndef TEST_C_TEMPORARY_3_H
#define TEST_C_TEMPORARY_3_H

// glew
#include <GL/glew.h>

// sfml
#include <SFML/Window.hpp>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// Eigen
#include <Eigen/Core>
#include <Eigen/Dense>

// std
#include <cstring>
#include <map>
#include <fstream>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <Temporary_2.h>

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * (float)M_PI / 180.0f)
// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0f / (float)M_PI)

#define WIDTH 640
#define HEIGHT 480

using namespace std;
using namespace Eigen;
using cv::Mat;

class Renderer {
public:
    Renderer(const char *rootDirectory);

    ~Renderer();

    void renderColor(Mesh *mesh, string program = "color");

    void getImage(Mat &img);

    Matrix3f K, Kinv; // intrinsics matrix

    Matrix4f ProjectionMatrix, ViewMatrix;

private:
    bool loadShaderCodeFromFile(const char *file_path, string &src);

    void compileShader(string src, GLenum type, GLuint &shader);

    GLint createRenderProgram(GLuint &vertex_shader, GLuint &fragment_shader, GLuint &program);

    map<string, GLuint> shader, program;

    map<string, GLuint> tbo;

    map<string, GLint> ViewMatrixID, MatrixID, ModelMatrixID, LightPositionID;
};

#endif //TEST_C_TEMPORARY_3_H
