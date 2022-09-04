#include "light.hpp"

Light::Light(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, GLfloat innerRadius, GLfloat outerRadius, GLfloat constant, GLfloat linear, GLfloat quadratic, const char* type)
    :Object(vertices, vertSize, indices, indSize, ShaderIndices::LIGHT_VERT, ShaderIndices::LIGHT_FRAG),
    innerRadius(innerRadius), outerRadius(outerRadius), constant(constant), linear(linear), quadratic(quadratic), type(type){}

Light::Light(const Light& light) {
    this->innerRadius = light.innerRadius;
    this->outerRadius = light.outerRadius;
    this->constant = light.constant;
    this->linear = light.linear;
    this->quadratic = light.quadratic;
    this->type = light.type;
}

void Light::setColor(const glm::vec3& color) {
    shader->getMaterial().objColor = color;
    shader->setUniform("lightColor", color);
}