#include "transform.hpp"

Transform::Transform(){
    position = glm::vec3(0.f);
    rotation = glm::vec3(0.f);
    scale = glm::vec3(1.f);
    model = glm::mat4(1.f);
}

Transform::Transform(const Transform& transform) {
    this->position = transform.position;
    this->rotation = transform.rotation;
    this->scale = transform.scale;
    this->model = transform.model;
}