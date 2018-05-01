#ifndef TEST_C_TEMPORARY_4_H
#define TEST_C_TEMPORARY_4_H

#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <Temporary_3.h>
#include <filesystem.hpp>
#include <sdf/sdf.hh>
#include <string>
#include <Temporary_9.h>

using namespace std;

enum{ NORMALS, TANGENTS};

class Model{
public:
    Model(const char* rootDirectory, const char* modelFile, bool withPoseEstimation = true);

    ~Model();

    void render(VectorXd &pose, Mat &img, bool clear, string program = "color");

    void render(Mat &img, bool clear, string program = "color");

    void Draw(bool clear, string program);

    void updateViewMatrix(sf::Window &window);

    void visualize(int type = NORMALS);

    Renderer *renderer;

    Poseestimator *poseestimator;

private:
    vector<Model_OBJ*> model_objs;
    vector<Mesh*> meshes;
    FileSystem *filesystem;
};

#endif //TEST_C_TEMPORARY_4_H
