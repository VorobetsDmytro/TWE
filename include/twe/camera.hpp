#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum CamMovement{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    L_SHIFT
};

class Camera {
public:
    Camera(const glm::vec3& pos = glm::vec3(0.f, 0.f, 0.f), GLfloat fov = 60.f, GLfloat sensitivity = 0.5f, GLfloat speed = 5.f);
    [[nodiscard]] glm::mat4 getViewMat() const noexcept;
    [[nodiscard]] glm::mat4 getProjectionMat(int wndWidth, int wndHeight) const noexcept;
    [[nodiscard]] glm::vec3 getPosition() const noexcept;
    [[nodiscard]] glm::vec3 getForward() const noexcept;
    [[nodiscard]] glm::vec3 getRight() const noexcept;
    [[nodiscard]] glm::vec3 getUp() const noexcept;
    void keyInput(CamMovement dir, GLfloat deltaTime);
    void mouseInput(GLfloat xOffset, GLfloat yOffset);
    void setPosition(const glm::vec3& pos);
private:
    void updateVecs();
    glm::vec3 _pos;
    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _worldUp;
    GLfloat _speed;
    GLfloat _boost;
    GLfloat _sensitivity;
    GLfloat _fov;
    GLfloat _pitch;
    GLfloat _yaw;
};

#endif