#include "renderer/texture.hpp"

namespace TWE {
    Texture::Texture(const TextureAttachmentSpecification& attachments)
    : _attachments(attachments) {
        for(auto& specification : _attachments.textureSpecifications)
            create(specification);
    }

    Texture::Texture(const Texture& texture) {
        this->_attachments = texture._attachments;
    }

    Texture::~Texture() {
        clean();
    }

    void Texture::bind(){
        for(auto& specification : _attachments.textureSpecifications) {
            glActiveTexture(GL_TEXTURE0 + specification.texNumber);
            glBindTexture((GLenum)specification.texType, specification.id);
        }
    }

    void Texture::unbind(){
        for(auto& specification : _attachments.textureSpecifications) 
            glBindTexture((GLenum)specification.texType, 0);
    }

    void Texture::clean(){
        for(auto& specification : _attachments.textureSpecifications)
            glDeleteTextures(1, &specification.id);
    }

    void Texture::setAttachments(const TextureAttachmentSpecification& attachments) {
        _attachments = attachments;
    }

    void Texture::setTexture(TextureSpecification& textureSpec) {
        int index = getIndexByTexNumber(textureSpec.texNumber);
        if(index == -1) {
            _attachments.textureSpecifications.push_back(textureSpec);
            index = _attachments.textureSpecifications.size() - 1;
        }
        else
            _attachments.textureSpecifications[index] = textureSpec;
        glBindTexture((GLenum)_attachments.textureSpecifications[index].texType, 0);
        glDeleteTextures(1, &_attachments.textureSpecifications[index].id);
        create(_attachments.textureSpecifications[index]);
    }

    void Texture::removeTexture(uint32_t texNumber) {
        int index = getIndexByTexNumber(texNumber);
        if(index == -1)
            return;
        glBindTexture((GLenum)_attachments.textureSpecifications[index].texType, 0);
        glDeleteTextures(1, &_attachments.textureSpecifications[index].id);
        _attachments.textureSpecifications.erase(_attachments.textureSpecifications.begin() + index);
    }

    TextureSpecification* Texture::getTextureSpecByTexNumber(uint32_t texNumber) {
        auto it = std::find_if(_attachments.textureSpecifications.begin(), _attachments.textureSpecifications.end(), [&](const TextureSpecification& spec){
            return spec.texNumber == texNumber;
        });
        if(it == _attachments.textureSpecifications.end())
            return nullptr;
        return &*it;
    }

    void Texture::create(TextureSpecification& textureSpecification) {
        int width, height, chanInFile;
        auto imgBytes = stbi_load(textureSpecification.imgPath.c_str(), &width, &height, &chanInFile, 4);
        glGenTextures(1, &textureSpecification.id);
        glActiveTexture(GL_TEXTURE0 + textureSpecification.texNumber);
        GLenum textureType = (GLenum)textureSpecification.texType;
        GLenum textureInOutFormat = (GLenum)textureSpecification.inOutTexFormat;
        glBindTexture(textureType, textureSpecification.id);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(textureType, 0, textureInOutFormat, width, height, 0, textureInOutFormat, GL_UNSIGNED_BYTE, imgBytes);
        glGenerateMipmap(textureType);
        stbi_image_free(imgBytes);
        glBindTexture(textureType, 0);
    }

    uint32_t Texture::getIndexByTexNumber(uint32_t texNumber) const noexcept {
        int size = _attachments.textureSpecifications.size();
        for(int i = 0; i < size; ++i)
            if(_attachments.textureSpecifications[i].texNumber == texNumber)
                return i;
        return -1;
    }

    uint32_t Texture::getId(int index) const noexcept { return _attachments.textureSpecifications[index].id; }
    TextureAttachmentSpecification& Texture::getAttachments() { return _attachments; }
}