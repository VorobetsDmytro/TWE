#ifndef VAO_HPP
#define VAO_HPP

#include <glad.h>

#include "vbo.hpp"

namespace TWE {
    class VAO {
    public:
        VAO();
        VAO(const VAO& vao);
        ~VAO();
        void bind();
        void unbind();
        void clean();
        void setAttrib(GLuint layout, GLint size, GLenum type, GLboolean normalized, GLsizei step, const void* offset, VBO& vbo);
        [[nodiscard]] GLuint getId();
    private:
        GLuint _id;
    };
}

#endif