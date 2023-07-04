#include "scene/components/light-component.hpp"

namespace TWE {
    LightComponent::LightComponent(const glm::vec3& color, float innerRadius, float outerRadius, float constant, float linear, float quadratic, LightType type)
    : _color(color), _innerRadius(innerRadius), _outerRadius(outerRadius), _constant(constant), _linear(linear), _quadratic(quadratic), _type(type){
        _castShadows = type == LightType::Dir;
        _fbo = nullptr;
        if(type == LightType::Dir) {
            _color = { 0.9922f, 0.9843f, 0.8275f };
            setShadowMapSize(SHADOW_MAP_DEFAULT_SIZE);
        } else
            _fbo = nullptr;
        _needRecacheProjection = true;
        _lightProjectionAspect = 20.f;
    }

    LightComponent::LightComponent(const LightComponent& light) {
        this->_innerRadius = light._innerRadius;
        this->_outerRadius = light._outerRadius;
        this->_constant = light._constant;
        this->_linear = light._linear;
        this->_quadratic = light._quadratic;
        this->_type = light._type;
        this->_fbo = light._fbo;
        this->_color = light._color;
        this->_needRecacheProjection = light._needRecacheProjection;
        this->_lightProjectionAspect = light._lightProjectionAspect;
        this->_lightProjection = light._lightProjection;
        this->_castShadows = light._castShadows;
    }

    void LightComponent::setType(LightType type) {
        _type = type;
        if(type == LightType::Dir && !_fbo) {
            setShadowMapSize(SHADOW_MAP_DEFAULT_SIZE);
        }
    }

    void LightComponent::setCastShadows(bool castShadows) {
        _castShadows = castShadows;
        if(_castShadows && !_fbo)
            setShadowMapSize(SHADOW_MAP_DEFAULT_SIZE);
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

    void LightComponent::setInnerRadius(float innerRadius){
        _innerRadius = innerRadius;
    }

    void LightComponent::setOuterRadius(float outerRadius){
        _outerRadius = outerRadius;
    }

    void LightComponent::setConstant(float constant){
        _constant = constant;
    }

    void LightComponent::setLinear(float linear){
        _linear = linear;
    }

    void LightComponent::setQuadratic(float quadratic){
        _quadratic = quadratic;
    }

    void LightComponent::setColor(const glm::vec3& color){
        _color = color;
    }

    glm::mat4 LightComponent::getLightProjection() {
        if(_needRecacheProjection) {
            _lightProjection = glm::ortho(-_lightProjectionAspect, _lightProjectionAspect, -_lightProjectionAspect, _lightProjectionAspect, 0.1f, 500.f);
            _needRecacheProjection = false;
        }
        return _lightProjection;
    }

    bool LightComponent::operator==(const LightComponent& lightComponent) {
        return this->_innerRadius == lightComponent._innerRadius
            && this->_outerRadius == lightComponent._outerRadius
            && this->_constant == lightComponent._constant
            && this->_linear == lightComponent._linear
            && this->_quadratic == lightComponent._quadratic
            && this->_type == lightComponent._type
            && this->_fbo == lightComponent._fbo
            && this->_color == lightComponent._color
            && this->_needRecacheProjection == lightComponent._needRecacheProjection
            && this->_lightProjectionAspect == lightComponent._lightProjectionAspect
            && this->_lightProjection == lightComponent._lightProjection
            && this->_castShadows == lightComponent._castShadows;
    }

    bool LightComponent::operator!=(const LightComponent& lightComponent) {
        return !(*this == lightComponent);
    }

    std::shared_ptr<FBO> LightComponent::getFBO() { 
        return _fbo; 
    }

    float LightComponent::getLightProjectionAspect() const noexcept { return _lightProjectionAspect; }
    uint32_t LightComponent::getDepthTextureId() const noexcept { return _fbo->getDepthAttachment(); }
    float LightComponent::getInnerRadius() const noexcept { return _innerRadius; }
    float LightComponent::getOuterRadius() const noexcept { return _outerRadius; }
    float LightComponent::getConstant() const noexcept { return _constant; }
    float LightComponent::getLinear() const noexcept { return _linear; }
    float LightComponent::getQuadratic() const noexcept { return _quadratic; }
    glm::vec3 LightComponent::getColor() const noexcept { return _color; }
    bool LightComponent::getCastShadows() const noexcept { return _castShadows; }
    LightType LightComponent::getType() const noexcept { return _type; }

    std::vector<std::string> lightTypes = {
        "dir",
        "point",
        "spot"
    };
}