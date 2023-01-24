#include "undo-redo/ur-commands/mesh-component/add-mesh-component-command.hpp"

namespace TWE {
    AddMeshComponentCommand::AddMeshComponentCommand(const Entity& entity): _entity(entity) {}

    void AddMeshComponentCommand::execute() {
        std::string meshId = "Cube mesh";
        std::string meshRendererId = "Default renderer";
        auto mesh = Shape::shapeSpec->meshRegistry->get(meshId);
        auto meshRenderer = Shape::shapeSpec->meshRendererRegistry->get(meshRendererId);
        if(mesh && meshRenderer) {
            if(!_entity.hasComponent<CreationTypeComponent>())
                return;
            auto& creationType = _entity.getComponent<CreationTypeComponent>();
            creationType.setType(EntityCreationType::Cube);
            if(_entity.hasComponent<MeshComponent>())
                _entity.removeComponent<MeshComponent>();
            else
                meshComponentAdded = true;
            if(_entity.hasComponent<MeshRendererComponent>())
                _entity.removeComponent<MeshRendererComponent>();
            else
                meshRendererComponentAdded = true;
            _entity.addComponent<MeshComponent>(mesh->vao, mesh->vbo, mesh->ebo, meshId, ModelMeshSpecification{});
            _entity.addComponent<MeshRendererComponent>(meshRenderer->vertexShaderPath.c_str(), 
                meshRenderer->fragmentShaderPath.c_str(), (int)_entity.getSource(), meshRendererId);
        }
    }

    void AddMeshComponentCommand::unExecute() {
        if(meshComponentAdded && _entity.hasComponent<MeshComponent>())
            _entity.removeComponent<MeshComponent>();
        if(meshRendererComponentAdded && _entity.hasComponent<MeshRendererComponent>())
            _entity.removeComponent<MeshRendererComponent>();
        meshComponentAdded = false;
        meshRendererComponentAdded = false;
    }
}