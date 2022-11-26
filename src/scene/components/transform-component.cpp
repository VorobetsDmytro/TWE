#include "scene/components/transform-component.hpp"

namespace TWE {
    TransformComponent::TransformComponent(){
        position = glm::vec3(0.f);
        rotation = glm::vec3(0.f);
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
        setRotation({ rotation.x + (axis.x * (angle * PI / 180.f)), rotation.y + (axis.y * (angle * PI / 180.f)), rotation.z + (axis.z * (angle * PI / 180.f)) });
    }

    void TransformComponent::scale(const glm::vec3& size) {
        model = glm::scale(model, size);
        this->size.x = glm::length(model[0]);
        this->size.y = glm::length(model[1]);
        this->size.z = glm::length(model[2]);
    }

    void TransformComponent::setPosition(const glm::vec3& pos) {
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
        this->rotation = glm::eulerAngles(glm::angleAxis(angle, axis)) * PI / 180.f;
    }

    void TransformComponent::setRotation(const glm::quat& quat) {
        glm::mat4 translation = glm::translate(glm::mat4(1.f), position);
        float angle = Math::getAngle(quat);
        glm::vec3 axis = Math::getAxis(quat);
        glm::mat4 rotation = glm::rotate(translation, glm::radians<GLfloat>(angle), axis);
        glm::mat4 scaleMat = glm::scale(rotation, size);
        model = scaleMat;
        this->rotation = glm::eulerAngles(glm::angleAxis(angle, axis)) * PI / 180.f;
    }

    void TransformComponent::setRotation(const glm::vec3& angles) {
        glm::mat4 translation = glm::translate(glm::mat4(1.f), position);
        glm::mat4 rotation = glm::toMat4(glm::quat(angles));
        glm::mat4 scaleMat = glm::scale(translation * rotation, size);
        model = scaleMat;
        this->rotation = angles;
    }

    void TransformComponent::setSize(const glm::vec3& size) {
        glm::mat4 translation = glm::translate(glm::mat4(1.f), position);
        glm::mat4 rotation = glm::toMat4(glm::quat(this->rotation));
        glm::mat4 scaleMat = glm::scale(translation * rotation, size);
        model = scaleMat;
        this->size = size;
    }

    glm::vec3 TransformComponent::getForward() const noexcept { return glm::normalize(glm::vec3(model[2])); }
    glm::vec3 TransformComponent::getRight() const noexcept { return glm::normalize(glm::vec3(model[0])); }
    glm::vec3 TransformComponent::getUp() const noexcept { return glm::normalize(glm::vec3(model[1])); }
}