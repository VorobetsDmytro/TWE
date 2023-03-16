#ifndef REMOVE_MESH_COMPONENT_COMMAND_HPP
#define REMOVE_MESH_COMPONENT_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class RemoveMeshComponentCommand: public IURCommand {
    public:
        RemoveMeshComponentCommand(const Entity& entity);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        MeshComponent _meshComponent;
    };
}

#endif