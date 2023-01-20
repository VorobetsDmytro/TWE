#include "undo-redo/ur-commands/script-component/bind-behavior-script-component-command.hpp"

namespace TWE {
    BindBehaviorScriptComponentCommand::BindBehaviorScriptComponentCommand(const Entity& entity, const std::string& behaviorClassName)
        : _entity(entity), _behaviorClassName(behaviorClassName) {}

    void BindBehaviorScriptComponentCommand::execute() {
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
                    scriptSpec->isEnabled = true;
            }
        }
    }

    void BindBehaviorScriptComponentCommand::unExecute() {
        if(!_entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = _entity.getComponent<ScriptComponent>();
        scriptComponent.unbind(_behaviorClassName);
    }
}