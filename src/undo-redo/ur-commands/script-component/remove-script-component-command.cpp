#include "undo-redo/ur-commands/script-component/remove-script-component-command.hpp"

namespace TWE {
    RemoveScriptComponentCommand::RemoveScriptComponentCommand(const Entity& entity)
    : _entity(entity) {
        _scripts = _entity.getComponent<ScriptComponent>().getScripts();
    }

    void RemoveScriptComponentCommand::execute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        _entity.getComponent<ScriptComponent>().clean();
        _entity.removeComponent<ScriptComponent>();
    }

    void RemoveScriptComponentCommand::unExecute() {
        if(_entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = _entity.addComponent<ScriptComponent>();
        for(auto& script : _scripts) {
            auto scriptDLLData = _entity.getScene()->getScriptDLLRegistry()->get(script.behaviorClassName);
            if(scriptDLLData && scriptDLLData->isValid) {
                auto behaviorFactory = DLLCreator::loadDLLFunc(*scriptDLLData);
                if(behaviorFactory) {
                    Behavior* behavior = (Behavior*)behaviorFactory();
                    auto scriptSpec = scriptComponent.bind(behavior, scriptDLLData->scriptName);
                    if(scriptSpec)
                        scriptSpec->isEnabled = true;
                }
            }
        }
    }
}