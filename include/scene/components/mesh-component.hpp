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
        MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize);
        MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<std::string>& texPaths);
        MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<Texture*>& texs);
        MeshComponent(const MeshComponent& mesh);
        GLfloat* vertices;
        GLsizei vertSize;
        GLuint* indices;
        GLsizei indSize;
        std::shared_ptr<VAO> vao;
        std::shared_ptr<VBO> vbo;
        std::shared_ptr<EBO> ebo;
        std::vector<std::shared_ptr<Texture>> textures;
        std::string modelPath;
    private:
        void create();
    };
}

#endif