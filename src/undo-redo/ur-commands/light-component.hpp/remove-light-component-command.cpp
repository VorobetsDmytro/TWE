#include "undo-redo/ur-commands/light-component.hpp/remove-light-component-command.hpp"

namespace TWE {
    RemoveLightComponentCommand::RemoveLightComponentCommand(const Entity& entity): _entity(entity) {}

    void RemoveLightComponentCommand::execute() {
        if(!_entity.hasComponent<LightComponent>())
            return;
        _entity.removeComponent<LightComponent>();
    }

    void RemoveLightComponentCommand::unExecute() {
        if(_entity.hasComponent<LightComponent>())
            return;
        _entity.addComponent<LightComponent>();
    }
}