#ifndef DEBUG_CAMERA_HPP
#define DEBUG_CAMERA_HPP

#define DEBUG_CAMERA_BOOST_DEFAULT 1.5f
#define DEBUG_CAMERA_BOOST_MAX 10.f

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "input/input.hpp"

namespace TWE {
    class DebugCamera: public Camera {
    public:
        DebugCamera(const glm::vec3& pos = glm::vec3(0.f, 0.f, 0.f), float sensitivity = 0.5f, float speed = 5.f);
        void keyInput(float deltaTime);
        void mouseInput(float xOffset, float yOffset);
        void setPosition(const glm::vec3& pos);
        [[nodiscard]] glm::vec3 getForward() const noexcept;
        [[nodiscard]] glm::vec3 getRight() const noexcept;
        [[nodiscard]] glm::vec3 getUp() const noexcept;
        [[nodiscard]] glm::vec3 getPosition() const noexcept;
    private:
        void updateState();
        glm::vec3 pos;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 worldUp;
        float speed;
        float boost;
        float sensitivity;
        float pitch;
        float yaw;
    };
}

#endif