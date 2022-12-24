#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include <memory>

#include "renderer/camera.hpp"

namespace TWE {
    class CameraComponent {
    public:
        CameraComponent();
        ~CameraComponent();
        CameraComponent(const CameraComponent& cameraComponent);
        void setFocuse(bool isFocusedOn);
        [[nodiscard]] bool isFocusedOn() const noexcept;
        [[nodiscard]] Camera* getSource();
    private:
        Camera _camera;
        bool _isFocusedOn;
    };
}

#endif