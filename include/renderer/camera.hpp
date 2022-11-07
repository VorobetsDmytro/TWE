#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace TWE {
    class Camera {
    public:
        Camera();
        Camera(const Camera& camera);
        void setPerspective(float fov, int wndWidth, int wndHeight, float near = 0.1f, float far = 1000.f);
        void setOrthographic(float left, float right, float bottom, float top, float zNear = -1.f, float zFar = 1000.f);
        [[nodiscard]] glm::mat4 getView(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up);
        [[nodiscard]] glm::mat4 getProjection() noexcept;
        [[nodiscard]] const glm::mat4& getProjection() const noexcept;
    protected:
        glm::mat4 projection;
    };
}

#endif