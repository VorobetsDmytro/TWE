#include "undo-redo/ur-commands/script-component/add-script-component-command.hpp"

namespace TWE {
    AddScriptComponentCommand::AddScriptComponentCommand(const Entity& entity): _entity(entity) {}

    void AddScriptComponentCommand::execute() {
        if(_entity.hasComponent<ScriptComponent>())
            return;
        _entity.addComponent<ScriptComponent>();
    }

    void AddScriptComponentCommand::unExecute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        _entity.getComponent<ScriptComponent>().clean();
        _entity.removeComponent<ScriptComponent>();
    }
}