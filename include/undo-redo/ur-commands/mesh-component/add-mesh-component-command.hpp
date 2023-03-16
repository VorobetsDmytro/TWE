#ifndef ADD_MESH_COMPONENT_COMMAND_HPP
#define ADD_MESH_COMPONENT_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "scene/shape.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class AddMeshComponentCommand: public IURCommand {
    public:
        AddMeshComponentCommand(const Entity& entity);
        void execute() override;
        void unExecute() override;
    private:
        bool meshComponentAdded = false;
        bool meshRendererComponentAdded = false;
        Entity _entity;
    };
}

#endif