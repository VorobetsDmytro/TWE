#include "undo-redo/ur-commands/mesh-renderer-component/change-mesh-rendere-component-state-command.hpp"

namespace TWE {
    ChangeMeshRendererComponentState::ChangeMeshRendererComponentState(const Entity& entity, const MeshRendererComponent& oldState,  const MeshRendererComponent& newState)
    : _entity(entity), _oldState(oldState), _newState(newState) {}

    void ChangeMeshRendererComponentState::execute() {
        if(!_entity.hasComponent<MeshRendererComponent>())
            return;
        auto& meshRendererComponent = _entity.getComponent<MeshRendererComponent>();
        auto& shader = _newState.getShader();
        meshRendererComponent.setShader(shader->getVertPath().c_str(), 
            shader->getFragPath().c_str(), _newState.getRegistryId());
        meshRendererComponent.setMaterial(_newState.getMaterial());
        meshRendererComponent.setIs3D(_newState.getIs3D());
    }

    void ChangeMeshRendererComponentState::unExecute() {
        if(!_entity.hasComponent<MeshRendererComponent>())
            return;
        auto& meshRendererComponent = _entity.getComponent<MeshRendererComponent>();
        auto& shader = _oldState.getShader();
        meshRendererComponent.setShader(shader->getVertPath().c_str(), 
            shader->getFragPath().c_str(), _oldState.getRegistryId());
        meshRendererComponent.setMaterial(_oldState.getMaterial());
        meshRendererComponent.setIs3D(_oldState.getIs3D());
    }
}