#ifndef MESH_HPP
#define MESH_HPP

#include <glad.h>
#include <memory>
#include <vector>
#include <string>

#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "texture.hpp"

class Mesh {
public:
    Mesh(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize);
    Mesh(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<std::string>& texPaths);
    Mesh(const Mesh& mesh);
    void draw();
    void setTexture(const char* texPath, GLuint texNum);
    GLfloat* vertices;
    GLsizei vertSize;
    GLuint* indices;
    GLsizei indSize;
    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<EBO> ebo;
    std::vector<std::shared_ptr<Texture>> textures;
private:
    void create();
};

#endif