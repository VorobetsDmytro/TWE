#ifndef REMOVE_LIGHT_COMPONENT_COMMAND_HPP
#define REMOVE_LIGHT_COMPONENT_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class RemoveLightComponentCommand: public IURCommand {
    public:
        RemoveLightComponentCommand(const Entity& entity);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
    };
}

#endif