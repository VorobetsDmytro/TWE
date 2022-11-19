#ifndef FBO_HPP
#define FBO_HPP

#include <glad.h>
#include <memory>

namespace TWE {
    class FBO {
    public:
        FBO(uint32_t width, uint32_t height);
        ~FBO();
        void bind();
        void unbind();
        void clean();
        void resize(uint32_t width, uint32_t height);
        void setColorId(uint32_t colorId);
        void setDepthId(uint32_t depthId);
        void createFrameTexture();
        void createDepthMapTexture();
        [[nodiscard]] uint32_t getId() const noexcept;
        [[nodiscard]] uint32_t getColorId() const noexcept;
        [[nodiscard]] uint32_t getDepthId() const noexcept;
        [[nodiscard]] std::pair<uint32_t, uint32_t> getSize() const noexcept;
    private:
        uint32_t _id;
        uint32_t _colorId;
        uint32_t _depthId;
        uint32_t _width;
        uint32_t _height;
    };
}

#endif