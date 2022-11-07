#include "scene/components/camera-component.hpp"

namespace TWE {
    CameraComponent::CameraComponent() {
        _camera = std::make_shared<Camera>();
        _isFocusedOn = true;
    }

    CameraComponent::CameraComponent(const CameraComponent& cameraComponent) {
        this->_camera = cameraComponent._camera;
        this->_isFocusedOn = cameraComponent._isFocusedOn;
    }

    void CameraComponent::setFocuse(bool isFocusedOn) {
        _isFocusedOn = isFocusedOn;
    }

    bool CameraComponent::isFocusedOn() const noexcept { return _isFocusedOn; }
    Camera* CameraComponent::getSource() const noexcept { return _camera.get(); }
}