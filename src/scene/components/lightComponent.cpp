#include "scene/components/lightComponent.hpp"

namespace TWE {
    LightComponent::LightComponent(GLfloat innerRadius, GLfloat outerRadius, GLfloat constant, GLfloat linear, GLfloat quadratic, const std::string& type)
    : innerRadius(innerRadius), outerRadius(outerRadius), constant(constant), linear(linear), quadratic(quadratic), type(type){
        castShadows = type == "dir";
        if(castShadows)
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
        this->_depthTexId = light._depthTexId;
    }

    void LightComponent::createDepthMap() {
        _fbo = std::make_shared<FBO>();
        _depthMapWidth = 4096;
        _depthMapHeight = 4096;
        _depthTexId = Texture::linkDepthTexture(_depthMapWidth, _depthMapHeight, *_fbo.get());
    }

    std::pair<uint32_t, uint32_t> LightComponent::getDepthMapSize() { return {_depthMapWidth, _depthMapHeight}; }

    FBO* LightComponent::getFBO() { return _fbo.get(); }

    GLuint LightComponent::getDepthTextureId() const noexcept { return _depthTexId; }
}