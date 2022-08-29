#include "texture.hpp"

Texture::Texture(const Texture& texture) {
    this->_id = texture._id;
    this->_texType = texture._texType;
    this->_inOutTexFormat = texture._inOutTexFormat;
}

Texture::Texture(const char* imgPath, GLuint texNum) {
    _texType = GL_TEXTURE_2D;
    _inOutTexFormat = GL_RGBA;
    stbi_set_flip_vertically_on_load(true);
    int width, height, chanInFile;
    auto imgBytes = stbi_load(imgPath, &width, &height, &chanInFile, 4);
    if (stbi_failure_reason())
        std::cout << "STBI ERROR:\n" << stbi_failure_reason() << std::endl;
    glGenTextures(1, &_id);
    glActiveTexture(GL_TEXTURE0);
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

Texture::~Texture() {
    clean();
}

void Texture::bind(){
    glBindTexture(_texType, _id);
}
void Texture::unbind(){
    glBindTexture(_texType, 0);
}
void Texture::clean(){
    glDeleteTextures(1, &_id);
}

GLuint Texture::getId() { return _id; }