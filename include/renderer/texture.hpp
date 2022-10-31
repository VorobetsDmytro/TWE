#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad.h>
#include <stb_image.h>
#include <string>

#include "shader.hpp"
#include "fbo.hpp"

namespace TWE {
    class Texture {
    public:
        Texture() = default;
        Texture(const Texture& texture);
        Texture(const std::string& imgPath, GLuint texNum);
        Texture(const GLuint id, const GLenum type);
        ~Texture();
        void bind();
        void unbind();
        void clean();
        void setId(GLuint id);
        void setType(GLenum type);
        [[nodiscard]] GLuint getId() const noexcept;
        [[nodiscard]] static GLuint linkDepthTexture(uint32_t width, uint32_t height, FBO& fbo);
    private:
        GLuint _id;
        GLuint _texNum;
        GLenum _texType;
        GLenum _inOutTexFormat;
    };
}

#endif