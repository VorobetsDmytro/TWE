#ifndef FBO_HPP
#define FBO_HPP

#include <glad.h>
#include <memory>

namespace TWE {
    class FBO {
    public:
        FBO();
        ~FBO();
        void bind();
        void unbind();
        void clean();
        [[nodiscard]] GLuint getId() const noexcept;
    private:
        GLuint _id;
    };
}

#endif