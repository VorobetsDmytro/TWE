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
        Texture(const std::string& imgPath, uint32_t texNum);
        Texture(uint32_t id, uint32_t type, uint32_t inOutTexFormat, uint32_t texNum);
        ~Texture();
        void bind();
        void unbind();
        void clean();
        void setId(uint32_t id);
        void setType(uint32_t type);
        [[nodiscard]] uint32_t getId() const noexcept;
    private:
        uint32_t _id;
        uint32_t _texNum;
        uint32_t _texType;
        uint32_t _inOutTexFormat;
    };
}

#endif