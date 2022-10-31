#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include <glad.h>
#include <glm.hpp>
#include <string>
#include <memory>

#include "renderer/fbo.hpp"
#include "renderer/texture.hpp"

namespace TWE {
    class LightComponent {
    public:
        LightComponent(GLfloat innerRadius = 15.f, GLfloat outerRadius = 25.f, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f, const std::string& type = "dir");
        LightComponent(const LightComponent& light);
        [[nodiscard]] std::pair<uint32_t, uint32_t> getDepthMapSize();
        [[nodiscard]] FBO* getFBO();
        [[nodiscard]] GLuint getDepthTextureId() const noexcept;
        GLfloat innerRadius;
        GLfloat outerRadius;
        GLfloat constant;
        GLfloat linear; 
        GLfloat quadratic;
        std::string type;
        bool castShadows;
    private:
        void createDepthMap();
        std::shared_ptr<FBO> _fbo;
        GLuint _depthTexId;
        uint32_t _depthMapWidth;
        uint32_t _depthMapHeight;
    };
}

#endif