#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad.h>
#include <stb_image.h>

#include "shader.hpp"

class Texture {
public:
    Texture() = default;
    Texture(const Texture& texture);
    Texture(const char* imgPath, GLuint texNum);
    ~Texture();
    void bind();
    void unbind();
    void clean();
    [[nodiscard]] GLuint getId();
private:
    GLuint _id;
    GLenum _texType;
    GLenum _inOutTexFormat;
};

#endif