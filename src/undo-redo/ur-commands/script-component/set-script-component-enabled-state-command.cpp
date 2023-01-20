#include "undo-redo/ur-commands/script-component/set-script-component-enabled-state-command.hpp"

namespace TWE {
    SetScriptComponentEnabledStateCommand::SetScriptComponentEnabledStateCommand(const Entity& entity, const std::string& behaviorClassName, bool newState)
    : _entity(entity), _behaviorClassName(behaviorClassName), _newState(newState) {
        auto& scriptComponent = _entity.getComponent<ScriptComponent>();
        auto script = scriptComponent.getScript(behaviorClassName);
        if(!script)
            return;
        _oldState = script->isEnabled;
    }

    void SetScriptComponentEnabledStateCommand::execute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = _entity.getComponent<ScriptComponent>();
        auto script = scriptComponent.getScript(_behaviorClassName);
        if(!script)
            return;
        script->isEnabled = _newState;
    }

    void SetScriptComponentEnabledStateCommand::unExecute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = _entity.getComponent<ScriptComponent>();
        auto script = scriptComponent.getScript(_behaviorClassName);
        if(!script)
            return;
        script->isEnabled = _oldState;
    }
}