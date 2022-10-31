#include "renderer/fbo.hpp"

namespace TWE {
    FBO::FBO() {
        glGenFramebuffers(1, &_id);
    }

    FBO::~FBO() {
        clean();
    }

    void FBO::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
    }

    void FBO::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::clean() {
        glDeleteFramebuffers(1, &_id);
    }

    GLuint FBO::getId() const noexcept { return _id; }
}