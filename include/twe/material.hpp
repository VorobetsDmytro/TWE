#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glad.h>
#include <glm.hpp>

class Material {
public:
    Material(const glm::vec3& objColor = {1.f, 1.f, 1.f},
             GLfloat ambient = 0.01f, 
             GLfloat diffuse = 0.5f, 
             GLfloat specular = 1.f,
             GLfloat shininess = 64.f);
    Material(const Material& material);
    Material& operator=(const Material& material);
    glm::vec3 objColor;
    GLfloat ambient;
    GLfloat diffuse;
    GLfloat specular;
    GLfloat shininess;
};

#endif