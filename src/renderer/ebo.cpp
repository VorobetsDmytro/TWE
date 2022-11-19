#include "renderer/ebo.hpp"

namespace TWE {
    EBO::EBO(uint32_t* indices, long int size, uint32_t drawType) {
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

    uint32_t EBO::getId() { return _id; }
}