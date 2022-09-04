#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad.h>
#include <stb_image.h>
#include <string>

#include "shader.hpp"

class Texture {
public:
    Texture() = default;
    Texture(const Texture& texture);
    Texture(const std::string& imgPath, GLuint texNum);
    ~Texture();
    void bind();
    void unbind();
    void clean();
    void setId(GLuint id);
    [[nodiscard]] GLuint getId();
private:
    GLuint _id;
    GLenum _texType;
    GLenum _inOutTexFormat;
};

#endif