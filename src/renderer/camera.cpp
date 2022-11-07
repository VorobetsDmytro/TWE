#include "renderer/camera.hpp"

namespace TWE {
    Camera::Camera(){
        setPerspective(60.f, 16, 9);
    }

    Camera::Camera(const Camera& camera) {
        this->projection = camera.projection;
    }

    void Camera::setPerspective(float fov, int wndWidth, int wndHeight, float near, float far) {
        projection = glm::perspective(glm::radians<float>(fov), static_cast<float>(wndWidth) / static_cast<float>(wndHeight), near, far);
    }

    void Camera::setOrthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
        projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    glm::mat4 Camera::getView(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up) {
        return glm::lookAt(pos, pos + forward, up);
    }

    glm::mat4 Camera::getProjection() noexcept { return projection; }

    const glm::mat4& Camera::getProjection() const noexcept { return projection; }
}
