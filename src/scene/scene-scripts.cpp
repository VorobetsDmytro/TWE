#include "scene/scene-scripts.hpp"

namespace TWE {
    SceneScripts::SceneScripts(ProjectData* projectData): _projectData(projectData) {}

    void SceneScripts::reset(entt::registry* registry) {
        registry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& scriptComponent){
            scriptComponent.clean();
        });
    }

    void SceneScripts::update(entt::registry* registry, Scene* scene) {
        static std::filesystem::path loadScenePath;
        bool needLoadScene = false;
        registry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& scriptComponent){
            auto& scripts = scriptComponent.getScripts();
            for(auto& script : scripts) {
                if(!script.isEnabled)
                    return;
                try {
                    if(!script.isInitialized) {
                        script.initialize(entity, scene, _projectData->rootPath, Shape::shapeSpec);
                        script.instance->start();
                    }
                    if(script.instance->gameObject.hasComponent<PhysicsComponent>()) {
                        auto& physicsComponent = script.instance->gameObject.getComponent<PhysicsComponent>();
                        auto collisions = scene->getSceneStateSpecification()->physics.getCollisionDetection(physicsComponent.getRigidBody());
                        for(auto collisionObj : collisions){
                            auto userPointer = (PhysicsUserPointer*)collisionObj->getUserPointer();
                            script.instance->collisionDetection(Entity{ userPointer->entity, scene }, collisionObj);
                        }
                    }
                    script.instance->update(Time::getDeltaTime());
                    if(script.instance->needLoadScene) {
                        needLoadScene = true;
                        loadScenePath = script.instance->loadScenePath;
                        script.instance->needLoadScene = false;
                    }
                } catch(const std::exception& error) {
                    std::cerr << error.what() << std::endl;
                    script.isEnabled = false;
                }
            }
        });
        if(needLoadScene) {
            if(SceneSerializer::deserialize(scene, loadScenePath.string())) {
                std::filesystem::path scenePath = std::filesystem::relative(loadScenePath, _projectData->rootPath);
                if(!scenePath.empty())
                    _projectData->lastScenePath = scenePath;
                else
                    _projectData->lastScenePath = loadScenePath;
                scene->setState(SceneState::Run);
            }
        }
    }

    void SceneScripts::bindScript(DLLLoadData* dllData, Entity& entity) {
        if(!dllData || !dllData->isValid)
            return;
        auto behaviorFactory = DLLCreator::loadDLLFunc(*dllData);
        if(!behaviorFactory)
            return;
        Behavior* behavior = (Behavior*)behaviorFactory();
        entity.getComponent<ScriptComponent>().bind(behavior, dllData->scriptName);
    }

    void SceneScripts::bindScript(DLLLoadData* dllData, std::vector<Entity>& entities) {
        for(auto& entity : entities)
            bindScript(dllData, entity);
    }

    std::vector<Entity> SceneScripts::unbindScript(entt::registry* registry, DLLLoadData* dllData, Scene* scene) {
        std::vector<Entity> res;
        auto& view = registry->view<ScriptComponent>();
        for(auto entity : view) {
            auto& scriptComponent = view.get<ScriptComponent>(entity);
            auto script = scriptComponent.getScript(dllData->scriptName);
            if(!script)
                continue;
            scriptComponent.unbind(script->behaviorClassName);
            Entity instance { entity, scene };
            res.push_back(instance);
        }
        return res;
    }

    void SceneScripts::validateScript(const std::string scriptName, Scene* scene) {
        auto scriptDLLData = scene->_scriptDLLRegistry->get(scriptName);
        if(scriptDLLData) {
            auto& editEntities = unbindScript(&scene->_sceneRegistry.edit.entityRegistry, scriptDLLData, scene);
            unbindScript(&scene->_sceneRegistry.run.entityRegistry, scriptDLLData, scene);
            DLLCreator::freeDLLFunc(*scriptDLLData);
            scene->_scriptDLLRegistry->erase(scriptName);
            auto dllData = new DLLLoadData(DLLCreator::compileScript(scriptName, scriptDLLData->scriptDirectoryPath));
            scene->_scriptDLLRegistry->add(scriptName, dllData);
            if(!dllData || !dllData->isValid) {
                std::cout << "Script " +  scriptName + " compile error!\n";
                dllData->dllPath = scriptDLLData->dllPath;
                dllData->factoryFuncName = scriptDLLData->factoryFuncName;
                dllData->scriptDirectoryPath = scriptDLLData->scriptDirectoryPath;
                dllData->scriptName = scriptDLLData->scriptName;
            }
            bindScript(dllData, editEntities);
        } else 
            std::cout << "Script " +  scriptName + " was not found in the script dll registry!\n";
    }

    void SceneScripts::validateScripts(Scene* scene) {
        auto& scriptDLLRegistryKeys = scene->_scriptDLLRegistry->getKeys();
        for(auto& scriptName : scriptDLLRegistryKeys)
            validateScript(scriptName, scene);
    }
}