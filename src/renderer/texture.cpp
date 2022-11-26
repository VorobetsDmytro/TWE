#include "renderer/texture.hpp"

namespace TWE {
    Texture::Texture(uint32_t id, const TextureAttachmentSpecification& attachments)
    : _id(id), _attachments(attachments) {}

    Texture::Texture(const TextureAttachmentSpecification& attachments)
    : _attachments(attachments) {
        for(auto& specification : attachments.textureSpecifications) {
            int width, height, chanInFile;
            auto imgBytes = stbi_load(specification.imgPath.c_str(), &width, &height, &chanInFile, 4);
            glGenTextures(1, &_id);
            glActiveTexture(GL_TEXTURE0 + specification.texNumber);
            glBindTexture(specification.texType, _id);
            glTexParameteri(specification.texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(specification.texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(specification.texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(specification.texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(specification.texType, 0, specification.inOutTexFormat, width, height, 0, specification.inOutTexFormat, GL_UNSIGNED_BYTE, imgBytes);
            glGenerateMipmap(specification.texType);
            stbi_image_free(imgBytes);
            glBindTexture(specification.texType, 0);
        }
    }

    Texture::Texture(const Texture& texture) {
        this->_id = texture._id;
        this->_attachments = texture._attachments;
    }

    Texture::~Texture() {
        clean();
    }

    void Texture::bind(){
        for(auto& specification : _attachments.textureSpecifications) {
            glActiveTexture(GL_TEXTURE0 + specification.texNumber);
            glBindTexture(specification.texType, _id);
        }
    }
    void Texture::unbind(){
        for(auto& specification : _attachments.textureSpecifications) 
            glBindTexture(specification.texType, 0);
    }
    void Texture::clean(){
        glDeleteTextures(1, &_id);
    }

    void Texture::setId(uint32_t id) {
        _id = id;
    }

    uint32_t Texture::getId() const noexcept { return _id; }
    TextureAttachmentSpecification& Texture::getAttachments() { return _attachments; }
}