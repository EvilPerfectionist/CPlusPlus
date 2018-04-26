#include <filesystem.hpp>
#include <iostream>
#include <string>

void Model(string const &path)
{
    std::cout << "Path = " << path << "\n";
}

int main()
{
    FileSystem *filesystem = new FileSystem("/home/leon/Instrument_Pose_Estimation/Test_C/");
    path fullFilePath;
    filesystem->find("model.sdf", &fullFilePath);
    std::cout << fullFilePath << "\n";
    std::cout << "FullFilePath = " << fullFilePath << "\n";
    string path_final = fullFilePath.c_str();
    Model(path_final);
}