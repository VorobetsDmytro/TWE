#include "texture.hpp"

Texture::Texture(const Texture& texture) {
    this->_id = texture._id;
    this->_texType = texture._texType;
    this->_inOutTexFormat = texture._inOutTexFormat;
    this->_texNum = texture._texNum;
}

Texture::Texture(const std::string& imgPath, GLuint texNum) {
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

Texture::Texture(const GLuint id, const GLenum type) {
    _id = id;
    _texType = type;
    _inOutTexFormat = GL_RGBA;
    _texNum = 0;
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

void Texture::setId(GLuint id) {
    _id = id;
}

void Texture::setType(GLenum type) {
    _texType = type;
}

GLuint Texture::getId() const noexcept { return _id; }

GLuint Texture::linkDepthTexture(uint32_t width, uint32_t height, FBO& fbo) {
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    fbo.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    fbo.unbind();
    return depthMap;
}