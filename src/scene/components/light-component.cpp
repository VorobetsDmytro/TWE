#include "scene/components/light-component.hpp"

namespace TWE {
    LightComponent::LightComponent(const glm::vec3& color, float innerRadius, float outerRadius, float constant, float linear, float quadratic, LightType type)
    : color(color), innerRadius(innerRadius), outerRadius(outerRadius), constant(constant), linear(linear), quadratic(quadratic), type(type){
        castShadows = type == LightType::DIR;
        createDepthMap();
    }

    LightComponent::LightComponent(const LightComponent& light) {
        this->innerRadius = light.innerRadius;
        this->outerRadius = light.outerRadius;
        this->constant = light.constant;
        this->linear = light.linear;
        this->quadratic = light.quadratic;
        this->type = light.type;
        this->_fbo = light._fbo;
        this->color = light.color;
        this->castShadows = light.castShadows;
    }

    void LightComponent::setType(LightType type) {
        this->type = type;
        castShadows = false;
    }

    void LightComponent::setCastShadows(bool castShadows) {
        this->castShadows = castShadows;
    }

    void LightComponent::createDepthMap() {
        _fbo = std::make_shared<FBO>(4096, 4096);
        auto mapSize = _fbo->getSize();
        _fbo->createDepthMapTexture();
    }

    std::pair<uint32_t, uint32_t> LightComponent::getDepthMapSize() { return _fbo->getSize(); }

    FBO* LightComponent::getFBO() { return _fbo.get(); }

    uint32_t LightComponent::getDepthTextureId() const noexcept { return _fbo->getDepthId(); }

    std::vector<std::string> lightTypes = {
        "dir",
        "point",
        "spot"
    };
}