#include "scene/scene.hpp"

namespace TWE {
    Scene::Scene(uint32_t windowWidth, uint32_t windowHeight) {
        initPhysics();
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

    void Scene::initPhysics() {
        _scenePhysics.collisionConfig = new btDefaultCollisionConfiguration();
        _scenePhysics.dispatcher = new btCollisionDispatcher(_scenePhysics.collisionConfig);
        _scenePhysics.broadPhase = new btDbvtBroadphase();
        _scenePhysics.solver = new btSequentialImpulseConstraintSolver();
        _scenePhysics.world = new btDiscreteDynamicsWorld(_scenePhysics.dispatcher, _scenePhysics.broadPhase, _scenePhysics.solver, _scenePhysics.collisionConfig); 
        _scenePhysics.world->setGravity({0.f, -9.81f, 0.f});
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

    void Scene::setScriptDLLRegistry(Registry<DLLLoadData>* scriptDLLRegistry) {
        _scriptDLLRegistry = scriptDLLRegistry;
    }

    void Scene::setState(SceneState state) {
        _sceneState = state;
        switch (_sceneState) {
        case SceneState::Edit:
            _isFocusedOnDebugCamera = true;
            _entityRegistry.curEntityRegistry = &_entityRegistry.editEntityRegistry;
            break;
        case SceneState::Run:
            _isFocusedOnDebugCamera = false;
            resetPhysics(&_entityRegistry.runEntityRegistry);
            resetScripts(&_entityRegistry.runEntityRegistry);
            resetEntityRegistry(&_entityRegistry.runEntityRegistry);
            initPhysics();
            copyEntityRegistry(_entityRegistry.editEntityRegistry, _entityRegistry.runEntityRegistry);
            _entityRegistry.curEntityRegistry = &_entityRegistry.runEntityRegistry;
            break;
        }
    }

    std::vector<Entity> Scene::unbindScript(entt::registry* registry, DLLLoadData* dllData) {
        std::vector<Entity> res;
        auto& view = registry->view<ScriptComponent>();
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

    void Scene::bindScript(DLLLoadData* dllData, Entity& entity) {
        if(!dllData || !dllData->isValid) {
            entity.getComponent<ScriptComponent>().bind<Behavior>();
            return;
        }
        auto behaviorFactory = DLLCreator::loadDLLFunc(*dllData);
        if(!behaviorFactory) {
            entity.getComponent<ScriptComponent>().bind<Behavior>();
            return;
        }
        Behavior* behavior = (Behavior*)behaviorFactory();
        entity.getComponent<ScriptComponent>().bind(behavior, dllData->scriptName);
    }

    void Scene::bindScript(DLLLoadData* dllData, std::vector<Entity>& entities) {
        for(auto& entity : entities)
            bindScript(dllData, entity);
    }

    void Scene::validateScript(const std::string& scriptName) {
        auto scriptDLLData = _scriptDLLRegistry->get(scriptName);
        if(scriptDLLData && scriptDLLData->isValid) {
            auto& editEntities = unbindScript(&_entityRegistry.editEntityRegistry, scriptDLLData);
            auto& runEntities = unbindScript(&_entityRegistry.runEntityRegistry, scriptDLLData);
            DLLCreator::freeDLLFunc(*scriptDLLData);
            _scriptDLLRegistry->erase(scriptName);
            auto dllData = new DLLLoadData(DLLCreator::compileScript(scriptName, scriptDLLData->scriptDirectoryPath));
            _scriptDLLRegistry->add(scriptName, dllData);
            if(!dllData || !dllData->isValid)
                std::cout << "Script " +  scriptName + " compile error!\n";
            bindScript(dllData, editEntities);
        } else 
            std::cout << "Script " +  scriptName + " was not found in the script dll registry!\n";
    }

    void Scene::validateScripts() {
        auto& scriptDLLRegistryKeys = _scriptDLLRegistry->getKeys();
        for(auto& scriptName : scriptDLLRegistryKeys)
            validateScript(scriptName);
    }

    void Scene::reset() {
        resetPhysics(&_entityRegistry.runEntityRegistry);
        resetScripts(&_entityRegistry.runEntityRegistry);
        resetEntityRegistry(&_entityRegistry.runEntityRegistry);
        resetScripts(&_entityRegistry.editEntityRegistry);
        resetEntityRegistry(&_entityRegistry.editEntityRegistry);
        setState(SceneState::Edit);
        initPhysics();
    }

    void Scene::resetEntityRegistry(entt::registry* registry) {
        registry->clear();
        *registry = {};
    }

    void Scene::resetScripts(entt::registry* registry) {
        registry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& scriptComponent){
            scriptComponent.unbind();
        });
    }

    void Scene::resetPhysics(entt::registry* registry) {
        registry->view<PhysicsComponent>().each([&](entt::entity entity, PhysicsComponent& physicsComponent){
            if(!physicsComponent.getRigidBody())
                return;
            delete physicsComponent.getRigidBody()->getMotionState();
            delete physicsComponent.getRigidBody()->getCollisionShape();
            _scenePhysics.world->removeRigidBody(physicsComponent.getRigidBody());
            delete physicsComponent.getRigidBody();
        });
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
        #ifndef TWE_BUILD
        _frameBuffer->bind();
        Renderer::cleanScreen({});
        draw();
        _frameBuffer->unbind();
        #else
        draw();
        #endif
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
        #ifndef TWE_BUILD
        _frameBuffer->bind();
        Renderer::cleanScreen({});
        draw();
        _frameBuffer->unbind();
        #else
        draw();
        #endif
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
        #ifndef TWE_BUILD
        switch (_sceneState) {
        case SceneState::Edit:
        case SceneState::Pause:
            updateEditState();
            break;
        case SceneState::Run:
            updateRunState();
            break;
        }
        #else
        updateRunState();
        #endif
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
            auto& physicsComponent = entity.getComponent<PhysicsComponent>();
            physicsComponent.getDynamicsWorld()->removeRigidBody(physicsComponent.getRigidBody());
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
        entt::entity instance = to.create();

        to.emplace<TransformComponent>(instance, entity.getComponent<TransformComponent>());
        to.emplace<NameComponent>(instance, entity.getComponent<NameComponent>());
        to.emplace<CreationTypeComponent>(instance, entity.getComponent<CreationTypeComponent>());
        if(entity.hasComponent<CameraComponent>())
            to.emplace<CameraComponent>(instance, entity.getComponent<CameraComponent>());
        if(entity.hasComponent<LightComponent>())
            to.emplace<LightComponent>(instance, entity.getComponent<LightComponent>());

        if(entity.hasComponent<ScriptComponent>()) {
            auto& scriptComponent = to.emplace<ScriptComponent>(instance);
            auto scriptDLLData = _scriptDLLRegistry->get(entity.getComponent<ScriptComponent>().getBehaviorClassName());
            if(!scriptDLLData || !scriptDLLData->isValid)
                scriptComponent.bind<Behavior>();
            else {
                auto behaviorFactory = DLLCreator::loadDLLFunc(*scriptDLLData);
                if(!behaviorFactory)
                    scriptComponent.bind<Behavior>();
                else {
                    Behavior* behavior = (Behavior*)behaviorFactory();
                    scriptComponent.bind(behavior, scriptDLLData->scriptName);
                }
            }
        }

        if(entity.hasComponent<MeshComponent>()) {
            auto& meshComponent = entity.getComponent<MeshComponent>();
            to.emplace<MeshComponent>(instance, meshComponent.vao, meshComponent.vbo, meshComponent.ebo, meshComponent.registryId, meshComponent.texture->getAttachments());
        }

        if(entity.hasComponent<MeshRendererComponent>()) {
            auto& meshRendererComponent = entity.getComponent<MeshRendererComponent>();
            auto& meshRendererComponentCopy = to.emplace<MeshRendererComponent>(instance, meshRendererComponent.shader->getVertPath().c_str(), meshRendererComponent.shader->getFragPath().c_str(),
                (int)instance, meshRendererComponent.registryId);
            meshRendererComponentCopy.setMaterial(meshRendererComponent.material);
        }

        if(entity.hasComponent<PhysicsComponent>()) {
            auto& physicsComponent = entity.getComponent<PhysicsComponent>();
            to.emplace<PhysicsComponent>(instance, _scenePhysics.world, physicsComponent.getColliderType(), 
                physicsComponent.getShapeDimensions(), physicsComponent.getLocalScale(), physicsComponent.getPosition(),
                physicsComponent.getRotation(), physicsComponent.getMass());
        }

        return { instance, this };
    }

    bool& Scene::getIsFocusedOnDebugCamera() { return _isFocusedOnDebugCamera; }
    bool Scene::getIsFocusedOnDebugCamera() const noexcept { return _isFocusedOnDebugCamera; }
    bool Scene::getDrawLightMeshes() const noexcept { return _drawLightMeshes; }
    entt::registry* Scene::getRegistry() const noexcept { return _entityRegistry.curEntityRegistry; }
    btDynamicsWorld* Scene::getDynamicWorld() const noexcept { return _scenePhysics.world; }
    std::string Scene::getName() const noexcept { return _name; }
    FBO* Scene::getFrameBuffer() const noexcept { return _frameBuffer.get(); }
    Registry<DLLLoadData>* Scene::getScriptDLLRegistry() const noexcept { return _scriptDLLRegistry; }
}