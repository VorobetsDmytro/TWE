#include "mesh.hpp"

Mesh::Mesh(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize)
:vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
    create();
}

Mesh::Mesh(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<std::string>& texPaths)
:vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
    create();
    int texPathsSize = texPaths.size();
    for(int i = 0; i < texPathsSize; ++i)
        textures.push_back(std::make_shared<Texture>(texPaths[i].c_str(), i));
}

Mesh::Mesh(const Mesh& mesh) {
    this->vertices = mesh.vertices;
    this->vertSize = mesh.vertSize;
    this->indices = mesh.indices;
    this->indSize = mesh.indSize;
    this->vao = mesh.vao;
    this->vbo = mesh.vbo;
    this->ebo = mesh.ebo;
    this->textures = mesh.textures;
}

void Mesh::create() {
    vao = std::make_shared<VAO>();
    vao->bind();
    vbo = std::make_shared<VBO>(vertices, vertSize, GL_STATIC_DRAW);
    vbo->bind();
    ebo = std::make_shared<EBO>(indices, indSize, GL_STATIC_DRAW);
    ebo->bind();
    vao->setAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0, *vbo.get());
    vao->setAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)), *vbo.get());
    vao->setAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)), *vbo.get());
    vao->unbind();
    vbo->unbind();
    ebo->unbind();
}

void Mesh::draw() {
    vao->bind();
    for(auto& texture : textures)
        texture->bind();
    glDrawElements(GL_TRIANGLES, indSize / sizeof(GLsizei), GL_UNSIGNED_INT, (GLvoid*)0);
    for(auto& texture : textures)
        texture->unbind();
    vao->unbind();
}

void Mesh::setTexture(const char* texPath, GLuint texNum) {
    if(textures.size() <= texNum)
        textures.resize(texNum + 1);
    textures[texNum] = std::make_shared<Texture>(texPath, texNum);
}