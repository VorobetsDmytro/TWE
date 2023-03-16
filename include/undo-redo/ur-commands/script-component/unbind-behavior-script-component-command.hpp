#ifndef UNBIND_BEHAVIOR_SCRIPT_COMPONENT_COMMAND_HPP
#define UNBIND_BEHAVIOR_SCRIPT_COMPONENT_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class UnbindBehaviorScriptComponentCommand: public IURCommand {
    public:
        UnbindBehaviorScriptComponentCommand(const Entity& entity, const std::string& behaviorClassName);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        std::string _behaviorClassName;
        bool _isEnabled;
    };
}

#endif