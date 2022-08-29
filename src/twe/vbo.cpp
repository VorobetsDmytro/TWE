#include "vbo.hpp"

VBO::VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType) {
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, drawType);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::VBO(const VBO& vbo) {
    this->_id = vbo._id;
}

VBO::~VBO() {
    clean();
}

void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::clean(){
    glDeleteBuffers(1, &_id);
}

GLuint VBO::getId() { return _id; }