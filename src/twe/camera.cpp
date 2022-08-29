#include "camera.hpp"

Camera::Camera(const glm::vec3& pos, GLfloat fov, GLfloat sensitivity, GLfloat speed)
:_pos(pos), _fov(fov), _sensitivity(sensitivity), _speed(speed) {
    _forward = glm::vec3(0.f, 0.f, 1.f);
    _worldUp = glm::vec3(0.f, 1.f, 0.f);
    _pitch = 0.f;
    _yaw = -90.f;
    _boost = 1.f;
    updateVecs();
}

void Camera::updateVecs() {
    glm::vec3 forwardTemp;
    forwardTemp.x = cos(glm::radians<float>(_pitch)) * cos(glm::radians<float>(_yaw));
    forwardTemp.y = sin(glm::radians<float>(_pitch));
    forwardTemp.z = cos(glm::radians<float>(_pitch)) * sin(glm::radians<float>(_yaw));
    _forward = glm::normalize(forwardTemp);
    _right = glm::normalize(glm::cross(_forward, _worldUp));
    _up = glm::normalize(glm::cross(_right, _forward));
}

void Camera::keyInput(CamMovement dir, GLfloat deltaTime) {
    GLfloat camSpeed = _speed * _boost * deltaTime;
    if(dir == CamMovement::FORWARD)
        _pos += _forward * camSpeed;
    if(dir == CamMovement::BACKWARD)
        _pos -= _forward * camSpeed;
    if(dir == CamMovement::RIGHT)
       _pos += _right * camSpeed;
    if(dir == CamMovement::LEFT)
       _pos -= _right * camSpeed;
    _boost = dir == CamMovement::L_SHIFT ? 2.f : 1.f;
}

void Camera::mouseInput(GLfloat xOffset, GLfloat yOffset) {
    xOffset *= _sensitivity;
    yOffset *= _sensitivity;
    _yaw += xOffset;
    _pitch += yOffset;
    if(_pitch > 89.9f)
        _pitch = 89.9f;
    else if(_pitch < -89.9f)
        _pitch = -89.9f;
    updateVecs();
}

void Camera::setPosition(const glm::vec3& pos) {
    _pos = pos;
}

glm::mat4 Camera::getViewMat() const noexcept {
    return glm::lookAt(_pos, _pos + _forward, _up);
}

glm::mat4 Camera::getProjectionMat(int wndWidth, int wndHeight) const noexcept {
    return glm::perspective(glm::radians<GLfloat>(_fov), static_cast<GLfloat>(wndWidth) / static_cast<GLfloat>(wndHeight), 0.1f, 1000.f);
}

glm::vec3 Camera::getPosition() const noexcept { return _pos; }

glm::vec3 Camera::getForward() const noexcept { return _forward; }
glm::vec3 Camera::getRight() const noexcept { return _right; }
glm::vec3 Camera::getUp() const noexcept { return _up; }