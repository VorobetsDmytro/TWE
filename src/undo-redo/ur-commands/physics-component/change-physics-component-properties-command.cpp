#include "undo-redo/ur-commands/physics-component/change-physics-component-properties-command.hpp"

namespace TWE {
    ChangePhysicsComponentPropertiesCommand::ChangePhysicsComponentPropertiesCommand(const Entity& entity, 
    const PhysicsComponentProperties& oldProperties, const PhysicsComponentProperties& newProperties)
    : _entity(entity), _oldProperties(oldProperties), _newProperties(newProperties) {}

    void ChangePhysicsComponentPropertiesCommand::execute() {
        if(!_entity.hasComponent<PhysicsComponent>())
            return;
        auto& physicsComponent = _entity.getComponent<PhysicsComponent>();
        physicsComponent.setMass(_newProperties.mass);
        physicsComponent.setIsRotated(_newProperties.isRotated);
        physicsComponent.setIsTrigger(_newProperties.isTrigger);
        physicsComponent.setShapeDimensions(_newProperties.shapeDimensions);
    }

    void ChangePhysicsComponentPropertiesCommand::unExecute() {
        if(!_entity.hasComponent<PhysicsComponent>())
            return;
        auto& physicsComponent = _entity.getComponent<PhysicsComponent>();
        physicsComponent.setMass(_oldProperties.mass);
        physicsComponent.setIsRotated(_oldProperties.isRotated);
        physicsComponent.setIsTrigger(_oldProperties.isTrigger);
        physicsComponent.setShapeDimensions(_oldProperties.shapeDimensions);
    }
}