#include "undo-redo/ur-commands/camera-component/add-camera-component-command.hpp"

namespace TWE {
    AddCameraComponentCommand::AddCameraComponentCommand(const Entity& entity): _entity(entity) {}

    void AddCameraComponentCommand::execute() {
        if(_entity.hasComponent<CameraComponent>())
            return;
        auto& cameraComponent = _entity.addComponent<CameraComponent>();
        auto fboSize = _entity.getScene()->getFrameBuffer()->getSize();
        cameraComponent.getSource()->setPerspective(90.f, fboSize.first, fboSize.second);
    }

    void AddCameraComponentCommand::unExecute() {
        if(!_entity.hasComponent<CameraComponent>())
            return;
        _entity.removeComponent<CameraComponent>();
    }
}