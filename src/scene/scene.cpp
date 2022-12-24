#include "scene/scene.hpp"

namespace TWE {
    Scene::Scene(uint32_t windowWidth, uint32_t windowHeight) {
        _scenePhysics.collisionConfig = new btDefaultCollisionConfiguration();
        _scenePhysics.dispatcher = new btCollisionDispatcher(_scenePhysics.collisionConfig);
        _scenePhysics.broadPhase = new btDbvtBroadphase();
        _scenePhysics.solver = new btSequentialImpulseConstraintSolver();
        _scenePhysics.world = new btDiscreteDynamicsWorld(_scenePhysics.dispatcher, _scenePhysics.broadPhase, _scenePhysics.solver, _scenePhysics.collisionConfig); 
        _scenePhysics.world->setGravity({0.f, -9.81f, 0.f});

        _entityRegistry.curEntityRegistry = &_entityRegistry.editEntityRegistry;
        _sceneState = SceneState::Edit;

        FBOAttachmentSpecification attachments = { FBOTextureFormat::RGBA8, FBOTextureFormat::R32I, FBOTextureFormat::DEPTH24STENCIL8 };
        _frameBuffer = std::make_unique<FBO>(windowWidth, windowHeight, attachments);

        _debugCamera = nullptr;
        _scriptDLLRegistry = nullptr;
        _isFocusedOnDebugCamera = true;
        _drawLightMeshes = true;
        _drawColliders = true;
        _name = "Unnamed";
    }

    void Scene::setTransMat(const glm::mat4& transform, TransformMatrixOptions option) {
        _entityRegistry.curEntityRegistry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            meshRendererComponent.shader->setUniform(TRANS_MAT_OPTIONS[option], transform);
            meshRendererComponent.colliderShader->setUniform(TRANS_MAT_OPTIONS[option], transform);
        });
    }

    void Scene::setDebugCamera(DebugCamera* debugCamera) {
        _debugCamera = debugCamera;
    }

    void Scene::setName(const std::string& name) {
        _name = name;
    }

    void Scene::setScriptRegistry(Registry<Behavior>* scriptRegistry) {
        _scriptRegistry = scriptRegistry;
    }

    void Scene::setScriptDLLRegistry(Registry<DLLLoadData>* scriptDLLRegistry) {
        _scriptDLLRegistry = scriptDLLRegistry;
    }

    void Scene::setRegistryLoader(std::function<void(Registry<Behavior>&)> registryLoader) {
        _registryLoader = registryLoader;
    }

    void Scene::setState(SceneState state) {
        _sceneState = state;
        // switch (_sceneState) {
        // case SceneState::Edit:
        //     _entityRegistry.curEntityRegistry = &_entityRegistry.editEntityRegistry;
        //     break;
        // case SceneState::Run:
        //     copyEntityRegistry(_entityRegistry.editEntityRegistry, _entityRegistry.runEntityRegistry);
        //     _entityRegistry.curEntityRegistry = &_entityRegistry.runEntityRegistry;
        //     break;
        // }
    }

    std::vector<Entity> Scene::unbindScript(DLLLoadData* dllData) {
        std::vector<Entity> res;
        auto& view = _entityRegistry.curEntityRegistry->view<ScriptComponent>();
        for(auto entity : view) {
            auto& scriptComponent = view.get<ScriptComponent>(entity);
            if(scriptComponent.getBehaviorClassName() != dllData->scriptName)
                continue;
            scriptComponent.unbind();
            Entity instance { entity, this };
            res.push_back(instance);
        }
        return res;
    }

    void Scene::bindScript(DLLLoadData* dllData, std::vector<Entity>& entities) {
        for(auto& entity : entities) {
            auto behaviorFactory = DLLCreator::loadDLLFunc(*dllData);
            Behavior* behavior = (Behavior*)behaviorFactory();
            entity.getComponent<ScriptComponent>().bind(behavior, dllData->scriptName);
        }
    }

    void Scene::validateScript(const std::string& scriptName) {
        auto scriptDLLData = _scriptDLLRegistry->get(scriptName);
        if(scriptDLLData && scriptDLLData->isValid) {
            auto& entities = unbindScript(scriptDLLData);
            DLLCreator::freeDLLFunc(*scriptDLLData);
            _scriptDLLRegistry->erase(scriptName);
            auto dllData = new DLLLoadData(DLLCreator::compileScript(scriptName, scriptDLLData->scriptDirectoryPath));
            if(dllData && dllData->isValid) {
                _scriptDLLRegistry->add(scriptName, dllData);
                bindScript(dllData, entities);
            } else 
                std::cout << "Script " +  scriptName + " compile error!\n";
        } else 
            std::cout << "Script " +  scriptName + " was not found in the script dll registry!\n";
    }

    void Scene::validateScripts() {
        auto& scriptDLLRegistryKeys = _scriptDLLRegistry->getKeys();
        for(auto& scriptName : scriptDLLRegistryKeys) {
            auto scriptDLLData = _scriptDLLRegistry->get(scriptName);
            if(scriptDLLData && scriptDLLData->isValid) {
                auto& entities = unbindScript(scriptDLLData);
                DLLCreator::freeDLLFunc(*scriptDLLData);
                _scriptDLLRegistry->erase(scriptName);
                auto dllData = new DLLLoadData(DLLCreator::compileScript(scriptName, scriptDLLData->scriptDirectoryPath));
                if(dllData && dllData->isValid) {
                    _scriptDLLRegistry->add(scriptName, dllData);
                    bindScript(dllData, entities);
                } else 
                    std::cout << "Script " +  scriptName + " compile error!\n";
            } else 
                std::cout << "Script " +  scriptName + " was not found in the script dll registry!\n";
        }
    }

    void Scene::reset() {
        _entityRegistry.curEntityRegistry->each([&](entt::entity entity){
            Entity instance = { entity, this };
            if(instance.hasComponent<ScriptComponent>())
                instance.getComponent<ScriptComponent>().unbind();
            if(instance.hasComponent<PhysicsComponent>())
                _scenePhysics.world->removeRigidBody(instance.getComponent<PhysicsComponent>().getRigidBody());
        });
        _entityRegistry.curEntityRegistry->clear();
        *_entityRegistry.curEntityRegistry = {};
        _sceneState = SceneState::Edit;
    }

    void Scene::setLight(const LightComponent& light, const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const  uint32_t index) {
        std::string lightIndex = "lights[]";
        lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
        _entityRegistry.curEntityRegistry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            if(_entityRegistry.curEntityRegistry->any_of<LightComponent>(entity))
                return;
            Renderer::setLight(meshRendererComponent, light, transform, meshRenderer, lightIndex);
        });
    }

    void Scene::setViewPos(const glm::vec3& pos) {
        _entityRegistry.curEntityRegistry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            Renderer::setViewPosition(meshRendererComponent, pos);
        });
    }

    void Scene::updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos) {
        setTransMat(view, TransformMatrixOptions::VIEW);
        setTransMat(projection, TransformMatrixOptions::PROJECTION);
        setViewPos(pos);
    }

    void Scene::updateShadows(uint32_t windowWidth, uint32_t windowHeight) {
        glm::mat4& lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 500.f);
        int lightIndex = -1;
        _drawLightMeshes = false;
        _drawColliders = false;
        _entityRegistry.curEntityRegistry->view<LightComponent, TransformComponent>().each([&](entt::entity entity, LightComponent& lightComponent, 
        TransformComponent& transformComponent){
            ++lightIndex;
            if(!lightComponent.castShadows)
                return;
            glm::mat4& lightView = glm::lookAt(transformComponent.position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
            Renderer::generateDepthMap(lightComponent, transformComponent, lightProjection, lightView, this);
            setShadows(lightComponent, lightProjection * lightView, lightIndex);
        });
        Renderer::setViewport(0, 0, windowWidth, windowHeight);
    }

    void Scene::setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index) {
        std::string indexStr = std::to_string(index);
        std::string lightIndex = "lights[]";
        lightIndex.insert(lightIndex.length() - 1, indexStr);
        _entityRegistry.curEntityRegistry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            if(_entityRegistry.curEntityRegistry->any_of<LightComponent>(entity))
                return;
            Renderer::setShadows(meshRendererComponent, lightSpaceMat, lightIndex);
        });
    }

    bool Scene::updateView() {
        _sceneCameraSpecification.camera = nullptr;
        _sceneCameraSpecification.position = glm::vec3(0.f);
        _sceneCameraSpecification.forward = glm::vec3(0.f);
        _sceneCameraSpecification.up = glm::vec3(0.f);
        if(_isFocusedOnDebugCamera && _debugCamera) {
            _sceneCameraSpecification.camera = _debugCamera;
            _sceneCameraSpecification.position = _debugCamera->getPosition();
            _sceneCameraSpecification.forward = _debugCamera->getForward();
            _sceneCameraSpecification.up = _debugCamera->getUp();
        }
        else {
            auto& camsView = _entityRegistry.curEntityRegistry->view<CameraComponent, TransformComponent>();
            for(auto camEntity : camsView) {
                auto& [cameraComponent, transformComponent] = camsView.get<CameraComponent, TransformComponent>(camEntity);
                if(cameraComponent.isFocusedOn()){
                    _sceneCameraSpecification.camera = cameraComponent.getSource();
                    _sceneCameraSpecification.position = transformComponent.position;
                    _sceneCameraSpecification.forward = -transformComponent.getForward();
                    _sceneCameraSpecification.up = transformComponent.getUp();
                    break;
                }
            }
        }
        if(!_sceneCameraSpecification.camera)
            return false;
        updateView(_sceneCameraSpecification.camera->getView(_sceneCameraSpecification.position, _sceneCameraSpecification.forward, _sceneCameraSpecification.up),
                   _sceneCameraSpecification.camera->getProjection(), _sceneCameraSpecification.position);
        return true;
    }

    void Scene::updateEditState() {
        auto& fboSize = _frameBuffer->getSize();
        updateShadows(fboSize.first, fboSize.second);
        if(!updateView()) {
            _frameBuffer->bind();
            Renderer::cleanScreen({});
            _frameBuffer->unbind();
            return;
        }
        updateLight();
        _drawLightMeshes = _isFocusedOnDebugCamera;
        _drawColliders = _isFocusedOnDebugCamera;
        _frameBuffer->bind();
        Renderer::cleanScreen({});
        draw();
        _frameBuffer->unbind();
    }

    void Scene::updateRunState() {
        updatePhysics();
        updateScripts();
        auto& fboSize = _frameBuffer->getSize();
        updateShadows(fboSize.first, fboSize.second);
        if(!updateView()) {
            _frameBuffer->bind();
            Renderer::cleanScreen({});
            _frameBuffer->unbind();
            return;
        }
        updateLight();
        _drawLightMeshes = _isFocusedOnDebugCamera;
        _drawColliders = _isFocusedOnDebugCamera;
        _frameBuffer->bind();
        Renderer::cleanScreen({});
        draw();
        _frameBuffer->unbind();
    }

    void Scene::updateScripts() {
        _entityRegistry.curEntityRegistry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& scriptComponent){
            if(!scriptComponent.isEnabled)
                return;
            try {
                if(!scriptComponent._isInitialized) {
                    scriptComponent.initialize(entity, this);
                    scriptComponent._instance->start();
                }
                scriptComponent._instance->update(Time::getDeltaTime());
            } catch(const std::runtime_error& e) {
                std::cerr << e.what() << "\n";
                scriptComponent.isEnabled = false;
            }
        });
    } 

    void Scene::updateLight() {
        uint32_t lightIndex = 0;
        _entityRegistry.curEntityRegistry->view<LightComponent, TransformComponent, MeshRendererComponent>()
            .each([&](entt::entity entity, LightComponent& lightComponent, TransformComponent& transformComponent, MeshRendererComponent& meshRendererComponent){
                setLight(lightComponent, transformComponent, meshRendererComponent, lightIndex++);
            });
    }  

    void Scene::update() {
        switch (_sceneState) {
        case SceneState::Edit:
            updateEditState();
            break;
        case SceneState::Run:
            updateRunState();
            break;
        }
    }

    void Scene::draw() {
        auto& lightEnteties = _entityRegistry.curEntityRegistry->view<LightComponent>();
        _entityRegistry.curEntityRegistry->view<MeshComponent, MeshRendererComponent, TransformComponent>()
            .each([&](entt::entity entity, MeshComponent& meshComponent, MeshRendererComponent& meshRendererComponent, TransformComponent& transformComponent){
                if(_entityRegistry.curEntityRegistry->any_of<LightComponent>(entity))
                    if(!_drawLightMeshes)
                        return;
                if(_drawColliders && meshRendererComponent.showCollider) {
                    Entity instance = { entity, this };
                    if(instance.hasComponent<PhysicsComponent>()) {
                        auto& physicsComponent = instance.getComponent<PhysicsComponent>();
                        Renderer::execute(&meshComponent, &meshRendererComponent, &transformComponent, lightEnteties.size(), &physicsComponent);
                        return;
                    }
                }
                Renderer::execute(&meshComponent, &meshRendererComponent, &transformComponent, lightEnteties.size());
            });
    }

    void Scene::updatePhysics() {
        _entityRegistry.curEntityRegistry->view<PhysicsComponent, TransformComponent>().each([](entt::entity entity, PhysicsComponent& physicsComponent, 
        TransformComponent& transformComponent){
            btRigidBody* rigidBody = physicsComponent.getRigidBody();
            rigidBody->activate();
            if(physicsComponent.getMass() == 0.f)
                return;
            btTransform worldTransform;
            rigidBody->getMotionState()->getWorldTransform(worldTransform);
            btVector3 pos = worldTransform.getOrigin();
            btQuaternion quat = worldTransform.getRotation();
            transformComponent.setPosition(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
            glm::vec3 rot;
            quat.getEulerZYX(rot.z, rot.y, rot.x);
            transformComponent.setRotation(rot);
        });
        _scenePhysics.world->stepSimulation(Time::getDeltaTime());
    }

    void Scene::cleanEntity(Entity& entity) {
        if(entity.hasComponent<CameraComponent>())
            entity.removeComponent<CameraComponent>();
        if(entity.hasComponent<CreationTypeComponent>())
            entity.removeComponent<CreationTypeComponent>();
        if(entity.hasComponent<LightComponent>())
            entity.removeComponent<LightComponent>();
        if(entity.hasComponent<MeshComponent>())
            entity.removeComponent<MeshComponent>();
        if(entity.hasComponent<MeshRendererComponent>())
            entity.removeComponent<MeshRendererComponent>();
        if(entity.hasComponent<PhysicsComponent>()){
            _scenePhysics.world->removeRigidBody(entity.getComponent<PhysicsComponent>().getRigidBody());
            entity.removeComponent<PhysicsComponent>();
        }
        if(entity.hasComponent<ScriptComponent>()) {
            entity.getComponent<ScriptComponent>().unbind();
            entity.removeComponent<ScriptComponent>();
        }
        entity.removeComponent<TransformComponent>();
        entity.removeComponent<NameComponent>();
    }

    Entity Scene::createEntity(const std::string& name) {
        auto entity = _entityRegistry.curEntityRegistry->create();
        _entityRegistry.curEntityRegistry->emplace<TransformComponent>(entity);
        _entityRegistry.curEntityRegistry->emplace<NameComponent>(entity, name);
        _entityRegistry.curEntityRegistry->emplace<CreationTypeComponent>(entity);
        return { entity, this };
    }

    void Scene::copyEntityRegistry(entt::registry& from, entt::registry& to) {
        auto& view = from.view<NameComponent>();
        for(auto entity : view)
            copyEntity(Entity{ entity, this }, to);
    }

    Entity Scene::copyEntity(Entity& entity, entt::registry& to) {
        Entity instance = { to.create(), this};
        instance.addComponent<TransformComponent>(entity.getComponent<TransformComponent>());
        instance.addComponent<NameComponent>(entity.getComponent<NameComponent>());
        instance.addComponent<CreationTypeComponent>(entity.getComponent<CreationTypeComponent>());
        instance.addComponent<CameraComponent>(entity.getComponent<CameraComponent>());
        instance.addComponent<LightComponent>(entity.getComponent<LightComponent>());
        instance.addComponent<ScriptComponent>(entity.getComponent<ScriptComponent>());

        auto& meshComponent = entity.getComponent<MeshComponent>();
        instance.addComponent<MeshComponent>(meshComponent.vao, meshComponent.vbo, meshComponent.ebo, meshComponent.registryId, meshComponent.texture.get());

        auto& meshRendererComponent = entity.getComponent<MeshRendererComponent>();
        instance.addComponent<MeshRendererComponent>(meshRendererComponent.shader->getVertPath().c_str(), meshRendererComponent.shader->getFragPath().c_str(),
            (int)instance.getSource(), meshRendererComponent.registryId);

        auto& physicsComponent = entity.getComponent<PhysicsComponent>();
        instance.addComponent<PhysicsComponent>(_scenePhysics.world, physicsComponent.getColliderType(), 
            physicsComponent.getShapeDimensions(), physicsComponent.getLocalScale(), physicsComponent.getPosition(),
            physicsComponent.getRotation(), physicsComponent.getMass());

        return instance;
    }

    bool& Scene::getIsFocusedOnDebugCamera() { return _isFocusedOnDebugCamera; }
    bool Scene::getIsFocusedOnDebugCamera() const noexcept { return _isFocusedOnDebugCamera; }
    bool Scene::getDrawLightMeshes() const noexcept { return _drawLightMeshes; }
    entt::registry* Scene::getRegistry() const noexcept { return _entityRegistry.curEntityRegistry; }
    btDynamicsWorld* Scene::getDynamicWorld() const noexcept { return _scenePhysics.world; }
    std::string Scene::getName() const noexcept { return _name; }
    FBO* Scene::getFrameBuffer() const noexcept { return _frameBuffer.get(); }
    Registry<Behavior>* Scene::getScriptRegistry() const noexcept { return _scriptRegistry; }
    Registry<DLLLoadData>* Scene::getScriptDLLRegistry() const noexcept { return _scriptDLLRegistry; }
    std::function<void(Registry<Behavior>&)> Scene::getRegistryLoader() const noexcept { return _registryLoader; }
}