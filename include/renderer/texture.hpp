#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad.h>
#include <stb_image.h>
#include <string>

#include "shader.hpp"
#include "fbo.hpp"

namespace TWE {
    struct TextureSpecification {
        TextureSpecification() = default;
        TextureSpecification(const std::string& imgPath, uint32_t texType, uint32_t texNumber, uint32_t inOutTexFormat)
            : imgPath(imgPath), texType(texType), texNumber(texNumber), inOutTexFormat(inOutTexFormat){}
        std::string imgPath;
        uint32_t texNumber;
        uint32_t texType;
        uint32_t inOutTexFormat;
    };

    struct TextureAttachmentSpecification {
        TextureAttachmentSpecification() = default;
        TextureAttachmentSpecification(std::initializer_list<TextureSpecification> textureSpecifications)
            : textureSpecifications(textureSpecifications){}
        std::vector<TextureSpecification> textureSpecifications;
    };

    class Texture {
    public:
        Texture() = default;
        Texture(const Texture& texture);
        Texture(const TextureAttachmentSpecification& attachments);
        Texture(uint32_t id, const TextureAttachmentSpecification& attachments);
        ~Texture();
        void bind();
        void unbind();
        void clean();
        void setId(uint32_t id);
        [[nodiscard]] uint32_t getId() const noexcept;
        [[nodiscard]] TextureAttachmentSpecification& getAttachments();
    private:
        uint32_t _id;
        TextureAttachmentSpecification _attachments;
    };
}

#endif