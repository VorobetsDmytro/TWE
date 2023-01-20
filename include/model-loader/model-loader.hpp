#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <filesystem>

#include "scene/components/components.hpp"
#include "model-loader-data.hpp"

namespace TWE {
    class ModelLoader {
    public:
        ModelLoader() = default;
        ModelLoaderData* loadModel(const std::string& path);
    private:
        void clean();
        void procNode(aiNode* node, const aiScene* scene);
        MeshComponent procMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MeshComponent> meshes;
        std::string filePath;
        bool hasTextures;
    };
}

#endif