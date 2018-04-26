#include <filesystem.hpp>
#include <iostream>
#include <string>
#include <sdf/sdf.hh>

using namespace std;

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

    char *rootDirectory = "/home/leon/Instrument_Pose_Estimation/Test_C/";
    if (filesystem->find("model.sdf", &fullFilePath)) {
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
                                printf("x = %f, y = %f, z = %f\n", x, y, z);
                                if (sscanf(poseElem->GetValue()->GetAsString().c_str(), "%f %f %f %f %f %f", &x, &y, &z,
                                           &roll,
                                           &pitch, &yaw) != 6)
                                    printf("error reading pose parameters\n");
                                else {
                                    char modelPath[200];
                                    sprintf(modelPath, "%smodels/%s", rootDirectory, file);
                                    std::cout << "ModelPath = " << modelPath << "\n";
                                    path p(modelPath);
                                    if(exists(p) && is_regular_file(p)) {

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