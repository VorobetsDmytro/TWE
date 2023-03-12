#include "undo-redo/ur-commands/physics-component/add-physics-component-command.hpp"

namespace TWE {
    AddPhysicsComponentCommand::AddPhysicsComponentCommand(const Entity& entity)
    : _entity(entity) {}

    void AddPhysicsComponentCommand::execute() {
        if(!_entity.hasComponent<TransformComponent>())
            return;
        auto& transformComponent = _entity.getComponent<TransformComponent>();
        if(_entity.hasComponent<PhysicsComponent>())
            return;
        _entity.addComponent<PhysicsComponent>(_entity.getScene()->getDynamicWorld(), ColliderType::Box, 
            glm::vec3{1.f, 1.f, 1.f}, transformComponent.getSize(), transformComponent.getPosition(), 
            transformComponent.getRotation(), 0.f, _entity.getSource());
    }

    void AddPhysicsComponentCommand::unExecute() {
        if(!_entity.hasComponent<PhysicsComponent>())
            return;
        auto& physicsComponent = _entity.getComponent<PhysicsComponent>();
        physicsComponent.getDynamicsWorld()->removeRigidBody(physicsComponent.getRigidBody());
        _entity.removeComponent<PhysicsComponent>();
    }
}