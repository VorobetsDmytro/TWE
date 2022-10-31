#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad.h>

namespace TWE {
    class TransformComponent {
    public:
        TransformComponent();
        TransformComponent(const TransformComponent& transform);
        void rotate(float angle, const glm::vec3& axis);
        void move(const glm::vec3& pos);
        void scale(const glm::vec3& size);
        void setPos(const glm::vec3& pos);
        void setRotation(float angle, const glm::vec3& axis);
        [[nodiscard]] glm::vec3 getForward() const noexcept;
        [[nodiscard]] glm::vec3 getRight() const noexcept;
        [[nodiscard]] glm::vec3 getUp() const noexcept;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 size;
        glm::mat4 model;
    };
}

#endif