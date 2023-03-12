#include "undo-redo/ur-commands/entity/remove-entity-command.hpp"

namespace TWE {
    RemoveEntityCommand::RemoveEntityCommand(const Entity& entity, std::function<void()> unselectFunc)
    : _entity(entity), _unselectFunc(unselectFunc) {
        _sourceEntity = _entity.getSource();
        _sourceScene = _entity.getScene();
        auto collisionConfig = new btDefaultCollisionConfiguration();
        auto dispatcher = new btCollisionDispatcher(collisionConfig);
        auto broadPhase = new btDbvtBroadphase();
        auto solver = new btSequentialImpulseConstraintSolver();
        _tempWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfig);
        if(_entity.hasComponent<PhysicsComponent>())
            _dynamicsWorld = _entity.getComponent<PhysicsComponent>().getDynamicsWorld();
        auto& parentChildsComponent = _entity.getComponent<ParentChildsComponent>();
        copyComponents(_entity);
        for(auto child : parentChildsComponent.childs) {
            Entity childEntity = { child, _sourceScene };
            _childsCommand.push_back(new RemoveEntityCommand(childEntity, unselectFunc));
        }
    }

    RemoveEntityCommand::~RemoveEntityCommand() {
        for(auto command : _childsCommand)
            delete command;
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
        if(physicsComponent)
            delete physicsComponent;
        if(scriptComponent)
            scriptComponent->clean();
        if(audioComponent)
            audioComponent->clean();
    }

    void RemoveEntityCommand::execute() {
        _unselectFunc();
        for(auto command : _childsCommand)
            command->execute();
        _entity.destroy();
    }

    void RemoveEntityCommand::unExecute() {
        _entity = { _sourceEntity, _sourceScene };
        setComponents(_entity);
        for(auto command : _childsCommand)
            command->unExecute();
    }

    void RemoveEntityCommand::copyComponents(Entity& entity) {
        auto dynamicsWorld = _tempWorld;
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
            meshComponent = new MeshComponent(meshComponentCopy.getVAO(), meshComponentCopy.getVBO(), meshComponentCopy.getEBO(), 
                meshComponentCopy.getRegistryId(), meshComponentCopy.getModelMeshSpecification(), meshComponentCopy.getTexture());
        }
        if(entity.hasComponent<MeshRendererComponent>()) {
            auto& meshRendererComponentCopy = entity.getComponent<MeshRendererComponent>();
            auto& shader = meshRendererComponentCopy.getShader();
            meshRendererComponent = new MeshRendererComponent(shader->getVertPath().c_str(),
                shader->getFragPath().c_str(), (int)entity.getSource(), meshRendererComponentCopy.getRegistryId());
            meshRendererComponent->setMaterial(meshRendererComponentCopy.getMaterial());
            meshRendererComponent->setIs3D(meshRendererComponentCopy.getIs3D());
        }
        if(entity.hasComponent<PhysicsComponent>()) {
            auto& physicsComponentCopy = entity.getComponent<PhysicsComponent>();
            if(physicsComponentCopy.getColliderType() != ColliderType::TriangleMesh) {
                physicsComponent = new PhysicsComponent(dynamicsWorld, physicsComponentCopy.getColliderType(), 
                    physicsComponentCopy.getShapeDimensions(), physicsComponentCopy.getLocalScale(), physicsComponentCopy.getPosition(),
                    physicsComponentCopy.getRotation(), physicsComponentCopy.getMass(), entity.getSource());
                physicsComponent->setIsRotated(physicsComponentCopy.getIsRotated());
            } else {
                physicsComponent = new PhysicsComponent(dynamicsWorld, physicsComponentCopy.getColliderType(), 
                    physicsComponentCopy.getTriangleMesh(), physicsComponentCopy.getLocalScale(), physicsComponentCopy.getPosition(),
                    physicsComponentCopy.getRotation(), entity.getSource());
            }
        }
        if(entity.hasComponent<ScriptComponent>()) {
            scriptComponent = new ScriptComponent();
            scriptComponent->getScripts() = entity.getComponent<ScriptComponent>().getScripts();
        }
        if(entity.hasComponent<AudioComponent>()) {
            auto& audioComponentCopy = entity.getComponent<AudioComponent>();
            audioComponent = new AudioComponent(_sourceScene->getSceneAudio()->getSoundEngine());
            auto& soundSources = audioComponentCopy.getSoundSources();
            for(auto soundSource : soundSources) {
                auto newSoundSource = audioComponent->addSoundSource(soundSource->getSoundSourcePath(), soundSource->getIs3D());
                newSoundSource->setPlayLooped(soundSource->getPlayLooped());
                newSoundSource->setStartPaused(soundSource->getStartPaused());
                newSoundSource->setVolume(soundSource->getVolume());
                newSoundSource->setMinDistance(soundSource->getMinDistance());
                newSoundSource->setMaxDistance(soundSource->getMaxDistance());
                newSoundSource->setPlaybackSpeed(soundSource->getPlaybackSpeed());
            }
        }
    }

    void RemoveEntityCommand::setComponents(Entity& entity) {
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
        if(physicsComponent && !entity.hasComponent<PhysicsComponent>()) {
            if(physicsComponent->getColliderType() != ColliderType::TriangleMesh) {
                auto physicsComponentNew = entity.addComponent<PhysicsComponent>(_dynamicsWorld, physicsComponent->getColliderType(), 
                    physicsComponent->getShapeDimensions(), physicsComponent->getLocalScale(), physicsComponent->getPosition(),
                    physicsComponent->getRotation(), physicsComponent->getMass(), entity.getSource());
                physicsComponentNew.setIsRotated(physicsComponent->getIsRotated());
            } else {
                entity.addComponent<PhysicsComponent>(_dynamicsWorld, physicsComponent->getColliderType(), 
                    physicsComponent->getTriangleMesh(), physicsComponent->getLocalScale(), physicsComponent->getPosition(),
                    physicsComponent->getRotation(), entity.getSource());
            }
        }
        if(scriptComponent && !entity.hasComponent<ScriptComponent>()) {
            auto& scriptComponentNew = entity.addComponent<ScriptComponent>();
            auto& scripts = scriptComponent->getScripts();
            for(auto& script : scripts) {
                auto scriptDLLData = _sourceScene->getScriptDLLRegistry()->get(script.behaviorClassName);
                if(scriptDLLData && scriptDLLData->isValid) {
                    auto behaviorFactory = DLLCreator::loadDLLFunc(*scriptDLLData);
                    if(behaviorFactory) {
                        Behavior* behavior = (Behavior*)behaviorFactory();
                        auto scriptSpec = scriptComponentNew.bind(behavior, scriptDLLData->scriptName);
                        if(scriptSpec)
                            scriptSpec->isEnabled = script.isEnabled;
                    }
                }
            }
        }
        if(audioComponent && !entity.hasComponent<AudioComponent>()) {
            auto& soundSources = audioComponent->getSoundSources();
            auto& audioComponentNew = entity.addComponent<AudioComponent>(_sourceScene->getSceneAudio()->getSoundEngine());
            for(auto soundSource : soundSources) {
                auto newSoundSource = audioComponentNew.addSoundSource(soundSource->getSoundSourcePath(), soundSource->getIs3D());
                newSoundSource->setPlayLooped(soundSource->getPlayLooped());
                newSoundSource->setStartPaused(soundSource->getStartPaused());
                newSoundSource->setVolume(soundSource->getVolume());
                newSoundSource->setMinDistance(soundSource->getMinDistance());
                newSoundSource->setMaxDistance(soundSource->getMaxDistance());
                newSoundSource->setPlaybackSpeed(soundSource->getPlaybackSpeed());
            }
        }
    }
}