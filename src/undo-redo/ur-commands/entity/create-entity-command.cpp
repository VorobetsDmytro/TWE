#include "undo-redo/ur-commands/entity/create-entity-command.hpp"

namespace TWE {
    CreateEntityCommand::CreateEntityCommand(const Entity& entity, std::function<void()> unselectFunc)
    : _entity(entity), _unselectFunc(unselectFunc) {
        _sourceEntity = _entity.getSource();
        _sourceScene = _entity.getScene();
        copyComponents(_entity);
        notExecuteOnce = true;
        auto& childs = _entity.getComponent<ParentChildsComponent>().childs;
        for(auto child : childs) {
            Entity childEntity = { child, _sourceScene };
            _childsCommand.push_back(new CreateEntityCommand(childEntity, unselectFunc));
            _childsCommand.back()->notExecuteOnce = false;
        }
    }

    CreateEntityCommand::~CreateEntityCommand() {
        delete transformComponent;
        delete nameComponent;
        delete idComponent;
        delete creationTypeComponent;
        delete parentChildsComponent;
        if(lightComponent)
            delete lightComponent;
        if(cameraComponent)
            delete cameraComponent;
        if(meshComponent)
            delete meshComponent;
        if(meshRendererComponent)
            delete meshRendererComponent;
    }

    void CreateEntityCommand::execute() {
        if(notExecuteOnce) {
            notExecuteOnce = false;
            return;
        }
        _entity = { _sourceEntity, _sourceScene };
        setComponents(_entity);
        for(auto command : _childsCommand)
            command->execute();
    }

    void CreateEntityCommand::unExecute() {
        _unselectFunc();
        for(auto command : _childsCommand)
            command->unExecute();
        _entity.destroy();
    }

    void CreateEntityCommand::copyComponents(Entity& entity) {
        transformComponent = new TransformComponent(entity.getComponent<TransformComponent>());
        nameComponent = new NameComponent(entity.getComponent<NameComponent>());
        idComponent = new IDComponent(entity.getComponent<IDComponent>());
        creationTypeComponent = new CreationTypeComponent(entity.getComponent<CreationTypeComponent>());
        parentChildsComponent = new ParentChildsComponent(entity.getComponent<ParentChildsComponent>());
        if(entity.hasComponent<LightComponent>())
            lightComponent = new LightComponent(entity.getComponent<LightComponent>());
        if(entity.hasComponent<CameraComponent>())
            cameraComponent = new CameraComponent(entity.getComponent<CameraComponent>());
        if(entity.hasComponent<MeshComponent>()) {
            auto& meshComponentCopy = entity.getComponent<MeshComponent>();
            meshComponent = new MeshComponent(meshComponentCopy.vao, meshComponentCopy.vbo, meshComponentCopy.ebo, 
                meshComponentCopy.registryId, meshComponentCopy.modelSpec, meshComponentCopy.texture);
        }
        if(entity.hasComponent<MeshRendererComponent>()) {
            auto& meshRendererComponentCopy = entity.getComponent<MeshRendererComponent>();
            meshRendererComponent = new MeshRendererComponent(meshRendererComponentCopy.shader->getVertPath().c_str(),
                meshRendererComponentCopy.shader->getFragPath().c_str(), (int)entity.getSource(), meshRendererComponentCopy.registryId);
            meshRendererComponent->setMaterial(meshRendererComponentCopy.material);
        }
    }

    void CreateEntityCommand::setComponents(Entity& entity) {
        if(!entity.hasComponent<TransformComponent>())
            entity.addComponent<TransformComponent>(*transformComponent);
        if(!entity.hasComponent<NameComponent>())
            entity.addComponent<NameComponent>(*nameComponent);
        if(!entity.hasComponent<IDComponent>())
            entity.addComponent<IDComponent>(*idComponent);
        if(!entity.hasComponent<CreationTypeComponent>())
            entity.addComponent<CreationTypeComponent>(*creationTypeComponent);
        if(!entity.hasComponent<ParentChildsComponent>())
            entity.addComponent<ParentChildsComponent>(*parentChildsComponent);
        if(parentChildsComponent->parent != entt::null) {
            Entity parentEntity = { parentChildsComponent->parent, _sourceScene };
            if(parentEntity.hasComponent<ParentChildsComponent>()) {
                auto& childs = parentEntity.getComponent<ParentChildsComponent>().childs;
                auto& itChild = std::find_if(childs.begin(), childs.end(), [&](entt::entity entityChild) {
                    return entityChild == entity.getSource();
                });
                if(itChild == childs.end())
                    childs.push_back(entity.getSource());
            }
        }
        if(lightComponent && !entity.hasComponent<LightComponent>())
            entity.addComponent<LightComponent>(*lightComponent);
        if(cameraComponent && !entity.hasComponent<CameraComponent>())
            entity.addComponent<CameraComponent>(*cameraComponent);
        if(meshComponent && !entity.hasComponent<MeshComponent>())
            entity.addComponent<MeshComponent>(*meshComponent);
        if(meshRendererComponent && !entity.hasComponent<MeshRendererComponent>())
            entity.addComponent<MeshRendererComponent>(*meshRendererComponent);
    }
}