#include "renderer/texture.hpp"

namespace TWE {
    Texture::Texture(const Texture& texture) {
        this->_id = texture._id;
        this->_texType = texture._texType;
        this->_inOutTexFormat = texture._inOutTexFormat;
        this->_texNum = texture._texNum;
    }

    Texture::Texture(const std::string& imgPath, uint32_t texNum) {
        _texType = GL_TEXTURE_2D;
        _inOutTexFormat = GL_RGBA;
        _texNum = texNum;
        int width, height, chanInFile;
        auto imgBytes = stbi_load(imgPath.c_str(), &width, &height, &chanInFile, 4);
        glGenTextures(1, &_id);
        glActiveTexture(GL_TEXTURE0 + _texNum);
        glBindTexture(_texType, _id);
        glTexParameteri(_texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(_texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(_texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(_texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(_texType, 0, _inOutTexFormat, width, height, 0, _inOutTexFormat, GL_UNSIGNED_BYTE, imgBytes);
        glGenerateMipmap(_texType);
        stbi_image_free(imgBytes);
        glBindTexture(_texType, 0);
    }

    Texture::Texture(uint32_t id, uint32_t type, uint32_t inOutTexFormat, uint32_t texNum) {
        _id = id;
        _texType = type;
        _inOutTexFormat = inOutTexFormat;
        _texNum = texNum;
    }

    Texture::~Texture() {
        clean();
    }

    void Texture::bind(){
        glActiveTexture(GL_TEXTURE0 + _texNum);
        glBindTexture(_texType, _id);
    }
    void Texture::unbind(){
        glBindTexture(_texType, 0);
    }
    void Texture::clean(){
        glDeleteTextures(1, &_id);
    }

    void Texture::setId(uint32_t id) {
        _id = id;
    }

    void Texture::setType(uint32_t type) {
        _texType = type;
    }

    uint32_t Texture::getId() const noexcept { return _id; }
}