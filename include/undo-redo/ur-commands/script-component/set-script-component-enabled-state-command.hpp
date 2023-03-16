#ifndef SET_SCRIPT_COMPONENT_ENABLED_STATE_COMMAND_HPP
#define SET_SCRIPT_COMPONENT_ENABLED_STATE_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class SetScriptComponentEnabledStateCommand: public IURCommand {
    public:
        SetScriptComponentEnabledStateCommand(const Entity& entity, const std::string& behaviorClassName, bool newState);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        std::string _behaviorClassName;
        bool _oldState;
        bool _newState;
    };
}

#endif