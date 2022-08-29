#include "material.hpp"

Material::Material(const glm::vec3& objColor, GLfloat ambient, GLfloat diffuse, GLfloat specular, GLfloat shininess)
: objColor(objColor), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess){}

Material::Material(const Material& material) {
    this->objColor = material.objColor;
    this->ambient = material.ambient;
    this->diffuse = material.diffuse;
    this->specular = material.specular;
    this->shininess = material.shininess;
}

Material& Material::operator=(const Material& material) {
    this->objColor = material.objColor;
    this->ambient = material.ambient;
    this->diffuse = material.diffuse;
    this->specular = material.specular;
    this->shininess = material.shininess;
    return *this;
}