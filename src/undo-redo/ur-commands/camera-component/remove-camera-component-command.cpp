#include "undo-redo/ur-commands/camera-component/remove-camera-component-command.hpp"

namespace TWE {
    RemoveCameraComponentCommand::RemoveCameraComponentCommand(const Entity& entity): _entity(entity) {}

    void RemoveCameraComponentCommand::execute() {
        if(!_entity.hasComponent<CameraComponent>())
            return;
        cameraComponent = _entity.getComponent<CameraComponent>();
        _entity.removeComponent<CameraComponent>();
    }

    void RemoveCameraComponentCommand::unExecute() {
        if(_entity.hasComponent<CameraComponent>())
            return;
        _entity.addComponent<CameraComponent>(cameraComponent);
    }
}