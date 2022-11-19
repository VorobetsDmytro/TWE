#include "renderer/fbo.hpp"

namespace TWE {
    FBO::FBO(uint32_t width, uint32_t height): _width(width), _height(height) {
        glGenFramebuffers(1, &_id);
        _colorId = 0;
        _depthId = 0;
    }

    FBO::~FBO() {
        clean();
    }

    void FBO::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glViewport(0, 0, _width, _height);
    }

    void FBO::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::clean() {
        glDeleteFramebuffers(1, &_id);
        glDeleteTextures(1, &_colorId);
        glDeleteTextures(1, &_depthId);
    }

    void FBO::resize(uint32_t width, uint32_t height) {
        _width = width;
        _height = height;
        if(_colorId)
            glDeleteTextures(1, &_colorId);
        if(_depthId)
            glDeleteTextures(1, &_depthId);
    }

    void FBO::setColorId(uint32_t colorId) {
        _colorId = colorId;
    }

    void FBO::setDepthId(uint32_t depthId) {
        _depthId = depthId;
    }

    void FBO::createDepthMapTexture() {
        unsigned int depth;
        glGenTextures(1, &depth);
        glBindTexture(GL_TEXTURE_2D, depth);
        auto size = getSize();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.first, size.second, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        unbind();
        setDepthId(depth);
    }

    void FBO::createFrameTexture() {
        unsigned int color;
        unsigned int depth;
        bind();
        glGenTextures(1, &color);
        glBindTexture(GL_TEXTURE_2D, color);
        auto size = getSize();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.first, size.second, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
        glGenTextures(1, &depth);
        glBindTexture(GL_TEXTURE_2D, depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, size.first, size.second, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
        unbind();
        setColorId(color);
        setDepthId(depth);
    }

    uint32_t FBO::getId() const noexcept { return _id; }
    uint32_t FBO::getColorId() const noexcept { return _colorId; }
    uint32_t FBO::getDepthId() const noexcept { return _depthId; }
    std::pair<uint32_t, uint32_t> FBO::getSize() const noexcept { return {_width, _height}; }
}