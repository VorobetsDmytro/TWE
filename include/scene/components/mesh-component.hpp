#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <glad.h>
#include <memory>
#include <vector>
#include <string>

#include "renderer/vao.hpp"
#include "renderer/vbo.hpp"
#include "renderer/ebo.hpp"
#include "renderer/texture.hpp"

namespace TWE {
    class MeshComponent {
    public:
        MeshComponent(float* vertices, int vertSize, uint32_t* indices, int indSize, const std::string& registryId, const std::vector<std::string>& texPaths = {});
        MeshComponent(float* vertices, int vertSize, uint32_t* indices, int indSize, const std::string& registryId, const std::vector<Texture*>& texs);
        MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, const std::vector<std::string>& texPaths = {});
        MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, const std::vector<Texture*>& texs);
        MeshComponent(const MeshComponent& mesh);
        void setMesh(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId);
        std::shared_ptr<VAO> vao;
        std::shared_ptr<VBO> vbo;
        std::shared_ptr<EBO> ebo;
        std::vector<std::shared_ptr<Texture>> textures;
        std::string modelPath;
        std::string registryId;
    private:
        void create(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize);
    };
}

#endif