#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <glm/gtx/matrix_decompose.hpp>
#include <glad.h>

#include "twe-math/twe-math.hpp"

namespace TWE {
    class TransformComponent {
    public:
        TransformComponent();
        TransformComponent(const TransformComponent& transform);
        void rotate(float angle, const glm::vec3& axis);
        void move(const glm::vec3& pos);
        void scale(const glm::vec3& size);
        void setPosition(const glm::vec3& pos);
        void setRotation(float angle, const glm::vec3& axis);
        void setRotation(const glm::quat& quat);
        void setRotation(const glm::vec3& angles);
        void setSize(const glm::vec3& size);
        [[nodiscard]] glm::vec3 getForward() const noexcept;
        [[nodiscard]] glm::vec3 getRight() const noexcept;
        [[nodiscard]] glm::vec3 getUp() const noexcept;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 size;
        glm::mat4 model;
    };
}

#endif