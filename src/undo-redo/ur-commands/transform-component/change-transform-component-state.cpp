#include "undo-redo/ur-commands/transform-component/change-transform-component-state.hpp"

namespace TWE {
    ChangeTransformComponentState::ChangeTransformComponentState(const Entity& entity, const TransformComponent& oldState, 
    const TransformComponent& newState): _entity(entity), _oldState(oldState), _newState(newState) {}

    void ChangeTransformComponentState::execute() {
        if(!_entity.hasComponent<TransformComponent>())
            return;
        auto& transformComponent = _entity.getComponent<TransformComponent>();
        transformComponent.setPosition(_newState.getPosition());
        transformComponent.setRotation(_newState.getRotation());
        transformComponent.setSize(_newState.getSize());
    }

    void ChangeTransformComponentState::unExecute() {
        if(!_entity.hasComponent<TransformComponent>())
            return;
        auto& transformComponent = _entity.getComponent<TransformComponent>();
        transformComponent.setPosition(_oldState.getPosition());
        transformComponent.setRotation(_oldState.getRotation());
        transformComponent.setSize(_oldState.getSize());
    }
}