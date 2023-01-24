#include "scene/components/light-component.hpp"

namespace TWE {
    LightComponent::LightComponent(const glm::vec3& color, float innerRadius, float outerRadius, float constant, float linear, float quadratic, LightType type)
    : color(color), innerRadius(innerRadius), outerRadius(outerRadius), constant(constant), linear(linear), quadratic(quadratic), type(type){
        castShadows = type == LightType::Dir;
        _fbo = nullptr;
        if(type == LightType::Dir) {
            this->color = { 0.9922f, 0.9843f, 0.8275f };
            setShadowMapSize(SHADOW_MAP_DEFAULT_SIZE);
        } else
            _fbo = nullptr;
        _needRecacheProjection = true;
        _lightProjectionAspect = 20.f;
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
        this->_needRecacheProjection = light._needRecacheProjection;
        this->_lightProjectionAspect = light._lightProjectionAspect;
        this->_lightProjection = light._lightProjection;
        this->castShadows = light.castShadows;
    }

    void LightComponent::setType(LightType type) {
        this->type = type;
        if(type == LightType::Dir) {
            castShadows = true;
            if(!_fbo)
                setShadowMapSize(SHADOW_MAP_DEFAULT_SIZE);
        } else
            castShadows = false;
    }

    void LightComponent::setCastShadows(bool castShadows) {
        this->castShadows = castShadows;
    }

    void LightComponent::setFBO(std::shared_ptr<FBO> fbo) {
        _fbo = fbo;
    }

    void LightComponent::setShadowMapSize(uint32_t size) {
        FBOAttachmentSpecification attachments = { FBOTextureFormat::DEPTH24STENCIL8 };
        _fbo = std::make_shared<FBO>(size, size, attachments);
    }

    void LightComponent::setLightProjectionAspect(float lightProjectionAspect) {
        _lightProjectionAspect = lightProjectionAspect;
        _needRecacheProjection = true;
    }

    glm::mat4 LightComponent::getLightProjection() {
        if(_needRecacheProjection) {
            _lightProjection = glm::ortho(-_lightProjectionAspect, _lightProjectionAspect, -_lightProjectionAspect, _lightProjectionAspect, 0.1f, 500.f);
            _needRecacheProjection = false;
        }
        return _lightProjection;
    }

    bool LightComponent::operator==(const LightComponent& lightComponent) {
        return this->innerRadius == lightComponent.innerRadius
            && this->outerRadius == lightComponent.outerRadius
            && this->constant == lightComponent.constant
            && this->linear == lightComponent.linear
            && this->quadratic == lightComponent.quadratic
            && this->type == lightComponent.type
            && this->_fbo == lightComponent._fbo
            && this->color == lightComponent.color
            && this->_needRecacheProjection == lightComponent._needRecacheProjection
            && this->_lightProjectionAspect == lightComponent._lightProjectionAspect
            && this->_lightProjection == lightComponent._lightProjection
            && this->castShadows == lightComponent.castShadows;
    }

    bool LightComponent::operator!=(const LightComponent& lightComponent) {
        return !(*this == lightComponent);
    }

    std::shared_ptr<FBO> LightComponent::getFBO() { 
        return _fbo; 
    }

    float LightComponent::getLightProjectionAspect() const noexcept { return _lightProjectionAspect; }
    uint32_t LightComponent::getDepthTextureId() const noexcept { return _fbo->getDepthAttachment(); }

    std::vector<std::string> lightTypes = {
        "dir",
        "point",
        "spot"
    };
}