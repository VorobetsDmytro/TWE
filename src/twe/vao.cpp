#include "vao.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &_id);
}

VAO::VAO(const VAO& vao) {
    this->_id = vao._id;
}

VAO::~VAO() {
    clean();
}

void VAO::setAttrib(GLuint layout, GLint size, GLenum type, GLboolean normalized, GLsizei step, const void* offset, VBO& vbo) {
    vbo.bind();
    glVertexAttribPointer(layout, size, type, normalized, step, offset);
    glEnableVertexAttribArray(layout);
    vbo.unbind();
}

void VAO::bind() {
    glBindVertexArray(_id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::clean() {
    glDeleteVertexArrays(1, &_id);
}

GLuint VAO::getId() { return _id; }