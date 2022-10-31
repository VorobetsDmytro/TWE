#ifndef VBO_HPP
#define VBO_HPP

#include <glad.h>

namespace TWE {
    class VBO {
    public:
        VBO() = default;
        VBO(const VBO& vbo);
        VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType);
        ~VBO();
        void bind();
        void unbind();
        void clean();
        [[nodiscard]] GLuint getId();
    private:
        GLuint _id;
    };
}

#endif