#include "undo-redo/ur-commands/light-component.hpp/add-light-component-command.hpp"

namespace TWE {
    AddLightComponentCommand::AddLightComponentCommand(const Entity& entity): _entity(entity) {}
    
    void AddLightComponentCommand::execute() {
        if(_entity.hasComponent<LightComponent>())
            return;
        _entity.addComponent<LightComponent>();
    }

    void AddLightComponentCommand::unExecute() {
        if(!_entity.hasComponent<LightComponent>())
            return;
        _entity.removeComponent<LightComponent>();
    }
}