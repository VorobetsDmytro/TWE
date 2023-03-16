#include "undo-redo/ur-commands/camera-component/add-camera-component-command.hpp"

namespace TWE {
    AddCameraComponentCommand::AddCameraComponentCommand(const Entity& entity, FBO* fbo): _entity(entity), _fbo(fbo) {}

    void AddCameraComponentCommand::execute() {
        if(_entity.hasComponent<CameraComponent>() || !_fbo)
            return;
        auto& cameraComponent = _entity.addComponent<CameraComponent>();
        auto fboSize = _fbo->getSize();
        cameraComponent.getSource()->setPerspective(90.f, fboSize.width, fboSize.height);
    }

    void AddCameraComponentCommand::unExecute() {
        if(!_entity.hasComponent<CameraComponent>())
            return;
        _entity.removeComponent<CameraComponent>();
    }
}