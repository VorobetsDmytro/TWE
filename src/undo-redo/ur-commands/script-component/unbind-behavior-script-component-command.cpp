#include "undo-redo/ur-commands/script-component/unbind-behavior-script-component-command.hpp"

namespace TWE {
    UnbindBehaviorScriptComponentCommand::UnbindBehaviorScriptComponentCommand(const Entity& entity, const std::string& behaviorClassName)
        : _entity(entity), _behaviorClassName(behaviorClassName) {}

    void UnbindBehaviorScriptComponentCommand::execute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = _entity.getComponent<ScriptComponent>();
        auto script = scriptComponent.getScript(_behaviorClassName);
        if(!script)
            return;
        _isEnabled = script->isEnabled;
        scriptComponent.unbind(_behaviorClassName);
    }

    void UnbindBehaviorScriptComponentCommand::unExecute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = _entity.getComponent<ScriptComponent>();
        auto scriptDLLData = _entity.getScene()->getScriptDLLRegistry()->get(_behaviorClassName);
        if(scriptDLLData && scriptDLLData->isValid) {
            auto behaviorFactory = DLLCreator::loadDLLFunc(*scriptDLLData);
            if(behaviorFactory) {
                Behavior* behavior = (Behavior*)behaviorFactory();
                auto scriptSpec = scriptComponent.bind(behavior, scriptDLLData->scriptName);
                if(scriptSpec)
                    scriptSpec->isEnabled = _isEnabled;
            }
        }
    }
}