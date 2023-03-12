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
        void setInnerRadius(float innerRadius);
        void setOuterRadius(float outerRadius);
        void setConstant(float constant);
        void setLinear(float linear);
        void setQuadratic(float quadratic);
        void setColor(const glm::vec3& color);
        void setFBO(std::shared_ptr<FBO> fbo);
        void setCastShadows(bool castShadows);
        void setLightProjectionAspect(float lightProjectionAspect);
        void setShadowMapSize(uint32_t size);
        [[nodiscard]] std::shared_ptr<FBO> getFBO();
        [[nodiscard]] glm::mat4 getLightProjection();
        [[nodiscard]] float getLightProjectionAspect() const noexcept;
        [[nodiscard]] float getInnerRadius() const noexcept;
        [[nodiscard]] float getOuterRadius() const noexcept;
        [[nodiscard]] float getConstant() const noexcept;
        [[nodiscard]] float getLinear() const noexcept;
        [[nodiscard]] float getQuadratic() const noexcept;
        [[nodiscard]] bool getCastShadows() const noexcept;
        [[nodiscard]] LightType getType() const noexcept;
        [[nodiscard]] glm::vec3 getColor() const noexcept;
        [[nodiscard]] uint32_t getDepthTextureId() const noexcept;
        [[nodiscard]] bool operator==(const LightComponent& lightComponent);
        [[nodiscard]] bool operator!=(const LightComponent& lightComponent);
    private:
        float _innerRadius;
        float _outerRadius;
        float _constant;
        float _linear; 
        float _quadratic;
        glm::vec3 _color;
        LightType _type;
        bool _castShadows;
        bool _needRecacheProjection;
        float _lightProjectionAspect;
        glm::mat4 _lightProjection;
        std::shared_ptr<FBO> _fbo;
    };
}

#endif