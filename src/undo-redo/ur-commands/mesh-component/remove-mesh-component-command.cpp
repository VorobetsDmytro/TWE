#include "undo-redo/ur-commands/mesh-component/remove-mesh-component-command.hpp"

namespace TWE {
    RemoveMeshComponentCommand::RemoveMeshComponentCommand(const Entity& entity): _entity(entity) {
        _meshComponent = _entity.getComponent<MeshComponent>();
    }

    void RemoveMeshComponentCommand::execute() {
        if(!_entity.hasComponent<MeshComponent>())
            return;
        _entity.removeComponent<MeshComponent>();
    }

    void RemoveMeshComponentCommand::unExecute() {
        if(_entity.hasComponent<MeshComponent>())
            return;
        _entity.addComponent<MeshComponent>(_meshComponent.vao, _meshComponent.vbo, _meshComponent.ebo,
            _meshComponent.registryId, _meshComponent.modelSpec, _meshComponent.texture);
    }
}