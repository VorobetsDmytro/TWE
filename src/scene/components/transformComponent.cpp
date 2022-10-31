#include "scene/components/transformComponent.hpp"

namespace TWE {
    TransformComponent::TransformComponent(){
        position = glm::vec3(0.f);
        rotation = glm::quat(glm::vec3(0.f));
        size = glm::vec3(1.f);
        model = glm::mat4(1.f);
    }

    TransformComponent::TransformComponent(const TransformComponent& transform) {
        this->position = transform.position;
        this->rotation = transform.rotation;
        this->size = transform.size;
        this->model = transform.model;
    }

    void TransformComponent::move(const glm::vec3& pos) {
        model = glm::translate(model, pos / size);
        position = model[3];
    }

    void TransformComponent::rotate(float angle, const glm::vec3& axis) {
        model = glm::rotate(model, glm::radians<GLfloat>(angle), axis);
        rotation = glm::angleAxis(angle, axis);
    }

    void TransformComponent::scale(const glm::vec3& size) {
        model = glm::scale(model, size);
        this->size.x = glm::length(model[0]);
        this->size.y = glm::length(model[1]);
        this->size.z = glm::length(model[2]);
    }

    void TransformComponent::setPos(const glm::vec3& pos) {
        model[3].x = pos.x;
        model[3].y = pos.y;
        model[3].z = pos.z;
        position = model[3];
    }

    void TransformComponent::setRotation(float angle, const glm::vec3& axis) {
        glm::mat4 translation = glm::translate(glm::mat4(1.f), position);
        glm::mat4 rotation = glm::rotate(translation, glm::radians<GLfloat>(angle), axis);
        glm::mat4 scaleMat = glm::scale(rotation, size);
        model = scaleMat;
        this->rotation = glm::angleAxis(angle, axis);
    }

    glm::vec3 TransformComponent::getForward() const noexcept { return glm::normalize(glm::vec3(model[2])); }
    glm::vec3 TransformComponent::getRight() const noexcept { return glm::normalize(glm::vec3(model[0])); }
    glm::vec3 TransformComponent::getUp() const noexcept { return glm::normalize(glm::vec3(model[1])); }
}