#include "modelloader.hpp"

Object* ModelLoader::loadModel(const std::string& path) {
    Assimp::Importer importer;
    hasTextures = false;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "Error model loading:\n" << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    fileDir = path.substr(0, path.find_last_of('/'));
    procNode(scene->mRootNode, scene);
    return new Object(meshes, ShaderIndices::DEFAULT_VERT, hasTextures ? ShaderIndices::TEXTURE_FRAG : ShaderIndices::DEFAULT_FRAG);
}

std::vector<std::string> ModelLoader::loadMatText(aiMaterial* mat, aiTextureType type) {
    std::vector<std::string> texturesPath;
    int texCount = mat->GetTextureCount(type);
    for(int i = 0; i < texCount; ++i) {
        aiString texturePath;
        mat->GetTexture(type, i, &texturePath);
        std::string fullPath = fileDir + '/' + texturePath.C_Str();
        texturesPath.push_back(fullPath);
        if(!hasTextures)
            hasTextures = true;
    }
    return texturesPath;
}

Mesh ModelLoader::procMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<std::string> texturesPath;
    for(int i = 0; i < mesh->mNumVertices; ++i){
        //verts
        if(!mesh->mVertices)
            continue;
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        //normals
        vertices.push_back(mesh->mNormals ? mesh->mNormals[i].x : 0.f);
        vertices.push_back(mesh->mNormals ? mesh->mNormals[i].y : 0.f);
        vertices.push_back(mesh->mNormals ? mesh->mNormals[i].z : 0.f);
        //texture
        if( mesh->mTextureCoords[0] ) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0.f);
            vertices.push_back(0.f);
        }
    }
    for(int i = 0; i < mesh->mNumFaces; ++i){
        auto face = mesh->mFaces[i];
        for(int j = 0 ; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::string> diff = loadMatText(material, aiTextureType_DIFFUSE);
        texturesPath.insert(texturesPath.end(), diff.begin(), diff.end());
    }
    if(texturesPath.size() > 0)
        return Mesh(vertices.data(), vertices.size() * sizeof(GLfloat), indices.data(), indices.size() * sizeof(GLuint), texturesPath);
    return Mesh(vertices.data(), vertices.size() * sizeof(GLfloat), indices.data(), indices.size() * sizeof(GLuint));
}

void ModelLoader::procNode(aiNode* node, const aiScene* scene) {
    for(int i = 0; i < node->mNumMeshes; ++i){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(procMesh(mesh, scene));
    }
    for(int i = 0; i < node->mNumChildren; ++i)
        procNode(node->mChildren[i], scene);
}