#include "model-loader/model-loader.hpp"

namespace TWE {
    void ModelLoader::clean() {
        meshes.clear();
        filePath.clear();
        hasTextures = false;
    }

    ModelLoaderData* ModelLoader::loadModel(const std::string& path) {
        if(!std::filesystem::exists(path))
            return nullptr;
        clean();
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            std::cout << "Error model loading:\n" << importer.GetErrorString() << std::endl;
            return nullptr;
        }
        filePath = path;
        procNode(scene->mRootNode, scene);
        return new ModelLoaderData(meshes, path);
    }

    MeshComponent ModelLoader::procMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<GLuint> indices;
        int vertSize = mesh->mNumVertices * 8;
        float* vertices = new float[vertSize];
        std::vector<std::string> texturesPath;
        int verticesIndex = 0;
        for(int i = 0; i < mesh->mNumVertices; ++i){
            //vertex
            if(!mesh->mVertices)
                continue;
            vertices[verticesIndex++] = mesh->mVertices[i].x;
            vertices[verticesIndex++] = mesh->mVertices[i].y;
            vertices[verticesIndex++] = mesh->mVertices[i].z;
            //normals
            vertices[verticesIndex++] = mesh->mNormals ? mesh->mNormals[i].x : 0.f;
            vertices[verticesIndex++] = mesh->mNormals ? mesh->mNormals[i].y : 0.f;
            vertices[verticesIndex++] = mesh->mNormals ? mesh->mNormals[i].z : 0.f;
            //texture coords
            if( mesh->mTextureCoords[0] ) {
                vertices[verticesIndex++] = mesh->mTextureCoords[0][i].x;
                vertices[verticesIndex++] = mesh->mTextureCoords[0][i].y;
            }
            else {
                vertices[verticesIndex++] = 0.f;
                vertices[verticesIndex++] = 0.f;
            }
        }
        for(int i = 0; i < mesh->mNumFaces; ++i){
            auto face = mesh->mFaces[i];
            for(int j = 0 ; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }
        int indSize = indices.size();
        uint32_t* copyIndices = new uint32_t[indSize];
        for(int i = 0; i < indSize; ++i)
            copyIndices[i] = indices[i];
        ModelMeshSpecification modelSpec(true, filePath, -1);
        return MeshComponent(vertices, vertSize * sizeof(GLfloat), copyIndices, indSize * sizeof(GLuint), "Model mesh", modelSpec);
    }

    void ModelLoader::procNode(aiNode* node, const aiScene* scene) {
        for(int i = 0; i < node->mNumMeshes; ++i){
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(procMesh(mesh, scene));
        }
        for(int i = 0; i < node->mNumChildren; ++i)
            procNode(node->mChildren[i], scene);
    }
}