#include "renderer/ebo.hpp"

namespace TWE {
    EBO::EBO(GLuint* indices, GLsizeiptr size, GLenum drawType) {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, drawType);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    EBO::EBO(const EBO& ebo) {
        this->_id = ebo._id;
    }

    EBO::~EBO() {
        clean();
    }

    void EBO::bind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }

    void EBO::unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void EBO::clean(){
        glDeleteBuffers(1, &_id);
    }

    GLuint EBO::getId() { return _id; }
}