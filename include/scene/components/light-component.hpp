#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#define SHADOW_MAP_DEFAULT_SIZE 2048

#include <glad.h>
#include <glm.hpp>
#include <string>
#include <memory>
#include <vector>

#include "renderer/fbo.hpp"
#include "renderer/texture.hpp"

namespace TWE {
    enum LightType {
        Dir,
        Point,
        Spot
    };

    extern std::vector<std::string> lightTypes;

    class LightComponent {
    public:
        LightComponent(const glm::vec3& color = {1.f, 1.f, 1.f}, float innerRadius = 15.f, float outerRadius = 25.f, float constant = 1.f, 
                       float linear = 0.045f, float quadratic = 0.0075f, LightType type = LightType::Point);
        LightComponent(const LightComponent& light);
        void setType(LightType type);
        void setFBO(std::shared_ptr<FBO> fbo);
        void setCastShadows(bool castShadows);
        void setLightProjectionAspect(float lightProjectionAspect);
        void setShadowMapSize(uint32_t size);
        [[nodiscard]] std::shared_ptr<FBO> getFBO();
        [[nodiscard]] glm::mat4 getLightProjection();
        [[nodiscard]] float getLightProjectionAspect() const noexcept;
        [[nodiscard]] uint32_t getDepthTextureId() const noexcept;
        [[nodiscard]] bool operator==(const LightComponent& lightComponent);
        [[nodiscard]] bool operator!=(const LightComponent& lightComponent);
        float innerRadius;
        float outerRadius;
        float constant;
        float linear; 
        float quadratic;
        glm::vec3 color;
        LightType type;
        bool castShadows;
    private:
        bool _needRecacheProjection;
        float _lightProjectionAspect;
        glm::mat4 _lightProjection;
        std::shared_ptr<FBO> _fbo;
    };
}

#endif