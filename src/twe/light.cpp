#include "light.hpp"

Light::Light(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, GLfloat innerRadius, GLfloat outerRadius, GLfloat constant, GLfloat linear, GLfloat quadratic, const std::string& type)
    :Object(vertices, vertSize, indices, indSize, ShaderIndices::LIGHT_VERT, ShaderIndices::LIGHT_FRAG),
    innerRadius(innerRadius), outerRadius(outerRadius), constant(constant), linear(linear), quadratic(quadratic), type(type){
        castShadows = type == "dir";
        if(castShadows)
            createDepthMap();
    }

Light::Light(const Light& light) {
    this->innerRadius = light.innerRadius;
    this->outerRadius = light.outerRadius;
    this->constant = light.constant;
    this->linear = light.linear;
    this->quadratic = light.quadratic;
    this->type = light.type;
    this->_fbo = light._fbo;
    this->_depthTexId = light._depthTexId;
}

void Light::createDepthMap() {
    _fbo = std::make_shared<FBO>();
    _depthMapWidth = 4096;
    _depthMapHeight = 4096;
    _depthTexId = Texture::linkDepthTexture(_depthMapWidth, _depthMapHeight, *_fbo.get());
}

void Light::setColor(const glm::vec3& color) {
    shader->getMaterial().objColor = color;
    shader->setUniform("lightColor", color);
}

std::pair<uint32_t, uint32_t> Light::getDepthMapSize() { return {_depthMapWidth, _depthMapHeight}; }

FBO* Light::getFBO() { return _fbo.get(); }

GLuint Light::getDepthTextureId() const noexcept { return _depthTexId; }