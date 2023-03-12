#include "undo-redo/ur-commands/mesh-component/change-mesh-component-state-command.hpp"

namespace TWE {
    ChangeMeshComponentStateCommand::ChangeMeshComponentStateCommand(const Entity& entity, const MeshComponent& newState)
    : _entity(entity), _newState(newState) {
        _oldState = _entity.getComponent<MeshComponent>();
    }

    void ChangeMeshComponentStateCommand::execute() {
        if(!_entity.hasComponent<MeshComponent>())
            return;
        auto& meshComponent = _entity.getComponent<MeshComponent>();
        meshComponent.setMesh(_newState.getVAO(), _newState.getVBO(), _newState.getEBO(), _newState.getRegistryId(), _newState.getModelMeshSpecification());
        meshComponent.setTexture(_newState.getTexture());
    }

    void ChangeMeshComponentStateCommand::unExecute() {
        if(!_entity.hasComponent<MeshComponent>())
            return;
        auto& meshComponent = _entity.getComponent<MeshComponent>();
        meshComponent.setMesh(_oldState.getVAO(), _oldState.getVBO(), _oldState.getEBO(), _oldState.getRegistryId(), _oldState.getModelMeshSpecification());
        meshComponent.setTexture(_oldState.getTexture());
    }
}