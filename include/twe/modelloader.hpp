#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <vector>

#include "object.hpp"

class ModelLoader {
public:
    ModelLoader() = default;
    Object* loadModel(const std::string& path);
private:
    void procNode(aiNode* node, const aiScene* scene);
    Mesh procMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::string> loadMatText(aiMaterial *mat, aiTextureType type);
    std::vector<Mesh> meshes;
    std::string fileDir;
    bool hasTextures;
};

#endif