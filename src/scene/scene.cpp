#include "scene/scene.hpp"

namespace TWE {
    Scene::Scene(uint32_t windowWidth, uint32_t windowHeight) {
        _collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
        _dispatcher = std::make_unique<btCollisionDispatcher>(_collisionConfig.get());
        _broadPhase = std::make_unique<btDbvtBroadphase>();
        _solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        _world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfig.get());
        _world->setGravity({0.f, -9.81f, 0.f});
        _registry = std::make_unique<entt::registry>();
        FBOAttachmentSpecification attachments = { FBOTextureFormat::RGBA8, FBOTextureFormat::R32I, FBOTextureFormat::DEPTH24STENCIL8 };
        _frameBuffer = std::make_unique<FBO>(windowWidth, windowHeight, attachments);
        _debugCamera = nullptr;
        _isFocusedOnDebugCamera = true;
        _isFocusedOnViewport = false;
        _drawLightMeshes = true;
        _name = "Unnamed";
    }

    Scene::~Scene() {
        _registry->each([&](entt::entity entity){
            Entity instance = { entity, this };
            if(instance.hasComponent<ScriptComponent>())
                instance.getComponent<ScriptComponent>().destroy();
            if(instance.hasComponent<PhysicsComponent>())
                _world->removeRigidBody(instance.getComponent<PhysicsComponent>().getRigidBody());
        });
    }

    void Scene::setTransMat(const glm::mat4& transform, TransformMatrixOptions option) {
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            meshRendererComponent.shader->setUniform(TRANS_MAT_OPTIONS[option], transform);
        });
    }

    void Scene::setDebugCamera(DebugCamera* debugCamera) {
        _debugCamera = debugCamera;
    }

    void Scene::setName(const std::string& name) {
        _name = name;
    }

    void Scene::reset() {
        _registry->each([&](entt::entity entity){
            Entity instance = { entity, this };
            if(instance.hasComponent<ScriptComponent>())
                instance.getComponent<ScriptComponent>().destroy();
            if(instance.hasComponent<PhysicsComponent>())
                _world->removeRigidBody(instance.getComponent<PhysicsComponent>().getRigidBody());
        });
        _registry->clear();
        _registry = std::make_unique<entt::registry>();
    }

    bool Scene::proccesKeyInput(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if(_isFocusedOnViewport) {
            Input::keyCallback(window, key, scancode, action, mode);
            return true;
        }
        return false;
    }

    bool Scene::proccesMouseButtonInput(GLFWwindow* window, int button, int action, int mods) {
        if(_isFocusedOnViewport) {
            Input::mouseButtonCallback(window, button, action, mods);
            return true;
        }
        return false;
    }

    bool Scene::proccesMouseInput(GLFWwindow* window, double xpos, double ypos) {
        if(_isFocusedOnViewport) {
            Input::mouseCallback(window, xpos, ypos);
            return true;
        }
        return false;
    }

    void Scene::setLight(const LightComponent& light, const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const  uint32_t index) {
        std::string lightIndex = "lights[]";
        lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
        auto& lightEnteties = _registry->view<LightComponent>();
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            if(_registry->any_of<LightComponent>(entity))
                return;
            Renderer::setLight(meshRendererComponent, light, transform, meshRenderer, lightIndex, lightEnteties.size());
        });
    }

    void Scene::setViewPos(const glm::vec3& pos) {
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            Renderer::setViewPosition(meshRendererComponent, pos);
        });
    }

    void Scene::updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos) {
        setTransMat(view, TransformMatrixOptions::VIEW);
        setTransMat(projection, TransformMatrixOptions::PROJECTION);
        setViewPos(pos);
    }

    void Scene::generateShadows(uint32_t windowWidth, uint32_t windowHeight) {
        glm::mat4& lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 500.f);
        int lightIndex = -1;
        setDrawLightMeshes(false);
        _registry->view<LightComponent, TransformComponent>().each([&](entt::entity entity, LightComponent& lightComponent, TransformComponent& transformComponent){
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
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            if(_registry->any_of<LightComponent>(entity))
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
            auto& camsView = _registry->view<CameraComponent, TransformComponent>();
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

    void Scene::update() {
        _registry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& sc){
            if(!sc._instance) {
                sc.initialize(entity, this);
                sc._instance->start();
            }
            sc._instance->update(Time::deltaTime);
        });
        if(!updateView()) {
            _frameBuffer->bind();
            Renderer::cleanScreen({});
            _frameBuffer->unbind();
            return;
        }
        uint32_t lightIndex = 0;
        _registry->view<LightComponent, TransformComponent, MeshRendererComponent>()
            .each([&](entt::entity entity, LightComponent& lightComponent, TransformComponent& transformComponent, MeshRendererComponent& meshRendererComponent){
                setLight(lightComponent, transformComponent, meshRendererComponent, lightIndex++);
            });
        setDrawLightMeshes(true);
        _frameBuffer->bind();
        Renderer::cleanScreen({});
        draw();
        _frameBuffer->unbind();
    }

    void Scene::draw() {
        _registry->view<MeshComponent, MeshRendererComponent, TransformComponent>()
            .each([&](entt::entity entity, MeshComponent& meshComponent, MeshRendererComponent& meshRendererComponent, TransformComponent& transformComponent){
                if(_registry->any_of<LightComponent>(entity))
                    if(!_drawLightMeshes)
                        return;
                Renderer::execute(&meshComponent, &meshRendererComponent, &transformComponent);
            });
    }

    void Scene::updatePhysics() {
        _registry->view<PhysicsComponent, TransformComponent>().each([](entt::entity entity, PhysicsComponent& physicsComponent, TransformComponent& transformComponent){
            btRigidBody* rigidBody = physicsComponent.getRigidBody();
            rigidBody->activate();
            if(physicsComponent.getMass() == 0.f)
                return;
            btTransform worldTransform;
            rigidBody->getMotionState()->getWorldTransform(worldTransform);
            btVector3 pos = worldTransform.getOrigin();
            btQuaternion quat = worldTransform.getRotation();
            float angle = quat.getAngle();
            btVector3 axis = quat.getAxis();
            transformComponent.setPosition(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
            transformComponent.setRotation(glm::degrees(angle), {axis.getX(), axis.getY() ,axis.getZ()});
        });
        _world->stepSimulation(Time::deltaTime);
    }

    void Scene::linkRigidBody(const PhysicsComponent& physicsComponent) {
        _world->addRigidBody(physicsComponent.getRigidBody());
    }

    void Scene::setFocusOnDebugCamera(bool isFocusedOnDebugCamera){
        _isFocusedOnDebugCamera = isFocusedOnDebugCamera;
    }

    void Scene::setFocusOnViewport(bool isFocusedOnViewport) {
        _isFocusedOnViewport = isFocusedOnViewport;
    }

    void Scene::setDrawLightMeshes(bool drawLightMeshes) {
        _drawLightMeshes = drawLightMeshes;
    }

    Entity Scene::createEntity(const std::string& name) {
        auto entity = _registry->create();
        _registry->emplace<TransformComponent>(entity);
        _registry->emplace<NameComponent>(entity, name);
        return { entity, this };
    }

    bool& Scene::getIsFocusedOnDebugCamera() { return _isFocusedOnDebugCamera; }
    bool Scene::getIsFocusedOnDebugCamera() const noexcept { return _isFocusedOnDebugCamera; }
    bool Scene::getIsFocusedOnViewport() const noexcept { return _isFocusedOnViewport; };
    bool Scene::getDrawLightMeshes() const noexcept { return _drawLightMeshes; }
    entt::registry* Scene::getRegistry() const noexcept { return _registry.get(); }
    btDynamicsWorld* Scene::getDynamicWorld() const noexcept { return _world.get(); }
    std::string Scene::getName() const noexcept { return _name; }
    FBO* Scene::getFrameBuffer() const noexcept { return _frameBuffer.get(); }
}