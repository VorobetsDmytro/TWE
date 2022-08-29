#ifndef EBO_HPP
#define EBO_HPP

#include <glad.h>

class EBO {
public:
    EBO() = default;
    EBO(const EBO& ebo);
    EBO(GLuint* indices, GLsizeiptr size, GLenum drawType);
    ~EBO();
    void bind();
    void unbind();
    void clean();
    [[nodiscard]] GLuint getId();
private:
    GLuint _id;
};

#endif