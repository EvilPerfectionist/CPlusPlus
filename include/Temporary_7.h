#ifndef TEST_C_TEMPORARY_7_H
#define TEST_C_TEMPORARY_7_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Temporary_8.h>
#include <filesystem.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Model_OBJ
{
public:
    /*  Model Data */
    vector<Texture_OBJ> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh_OBJ> meshes;
    string directory;

    Model_OBJ();

    void Model_OBJ_Init(path &FullFilePath);

    void Draw(GLuint id);

private:

    void loadModel(string const &path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh_OBJ processMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture_OBJ> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    unsigned int TextureFromFile(const char *path, const string &directory);
};

#endif //TEST_C_TEMPORARY_7_H
