#ifndef TEST_C_TEMPORARY_4_H
#define TEST_C_TEMPORARY_4_H

#include <iostream>
#include <Temporary_1.h>
#include <Temporary_3.h>
#include <filesystem.hpp>
#include <sdf/sdf.hh>
#include <string>

using namespace std;

class Model{
public:
    Model(const char* rootDirectory, const char* modelFile, bool withPoseEstimation = true);

    ~Model();

    void render(VectorXd &pose, Mat &img, bool clear, string program = "color");

    void render(Mat &img, bool clear, string program = "color");

    void updateViewMatrix(sf::Window &window);

    Renderer *renderer;
private:
    vector<Mesh*> meshes;
    FileSystem *filesystem;
};

#endif //TEST_C_TEMPORARY_4_H
