#include "scene/scene.hpp"
#include <iostream>

namespace TWE {
    Scene::Scene() {
        _lightsCount = 0;
        _collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
        _dispatcher = std::make_unique<btCollisionDispatcher>(_collisionConfig.get());
        _broadPhase = std::make_unique<btDbvtBroadphase>();
        _solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        _world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfig.get());
        _world->setGravity({0.f, -9.81f, 0.f});
        _registry = std::make_unique<entt::registry>();
        _debugCamera = nullptr;
        _isFocusedOnDebugCamera = true;
        _drawLightMeshes = true;
    }

    Scene::~Scene() {
        _registry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& scriptComponent){
            if(scriptComponent.instance)
                scriptComponent.destroy();
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

    void Scene::setLight(const LightComponent& light, const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const  uint32_t index) {
        std::string lightIndex = "lights[]";
        lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
        auto& lightEnteties = _registry->view<LightComponent>();
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            if(_registry->any_of<LightComponent>(entity))
                return;
            meshRendererComponent.shader->setUniform("lightCount", _lightsCount + (GLint)lightEnteties.size());
            meshRendererComponent.shader->setUniform((lightIndex + ".pos").c_str(), transform.position);
            meshRendererComponent.shader->setUniform((lightIndex + ".direction").c_str(), transform.getForward());
            meshRendererComponent.shader->setUniform((lightIndex + ".color").c_str(), meshRenderer.material->objColor);
            meshRendererComponent.shader->setUniform((lightIndex + ".cutOff").c_str(), glm::cos(glm::radians(light.innerRadius)));
            meshRendererComponent.shader->setUniform((lightIndex + ".outerCutOff").c_str(), glm::cos(glm::radians(light.outerRadius)));
            meshRendererComponent.shader->setUniform((lightIndex + ".fading.constant").c_str(), light.constant);
            meshRendererComponent.shader->setUniform((lightIndex + ".fading.linear").c_str(), light.linear);
            meshRendererComponent.shader->setUniform((lightIndex + ".fading.quadratic").c_str(), light.quadratic);
            if(light.type == "spot")
                meshRendererComponent.shader->setUniform((lightIndex + ".type.spot").c_str(), true);
            else if(light.type == "point")
                meshRendererComponent.shader->setUniform((lightIndex + ".type.point").c_str(), true);
            else
                meshRendererComponent.shader->setUniform((lightIndex + ".type.dir").c_str(), true);
        });
    }

    void Scene::setViewPos(const glm::vec3& pos) {
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            meshRendererComponent.shader->setUniform("viewPos", pos);
        });
    }

    void Scene::updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos) {
        setTransMat(view, TransformMatrixOptions::VIEW);
        setTransMat(projection, TransformMatrixOptions::PROJECTION);
        setViewPos(pos);
    }

    void Scene::generateShadows(uint32_t windowWidth, uint32_t windowHeight) {
        glm::mat4& lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.f);
        int lightIndex = -1;
        _registry->view<LightComponent, TransformComponent>().each([&](entt::entity entity, LightComponent& lightComponent, TransformComponent& transformComponent){
            ++lightIndex;
            if(lightComponent.type != "dir")
                return;
            glm::mat4& lightView = glm::lookAt(transformComponent.position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
            setShadows(lightComponent, lightProjection * lightView, lightIndex);
            generateDepthMap(lightComponent, transformComponent, lightProjection, lightView);
        });
        Renderer::setViewport(0, 0, windowWidth, windowHeight);
    }

    void Scene::setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index) {
        std::string indexStr = std::to_string(index);
        _registry->view<MeshRendererComponent>().each([&](entt::entity entity, MeshRendererComponent& meshRendererComponent){
            if(_registry->any_of<LightComponent>(entity))
                return;
            std::string lightIndex = "lights[]";
            lightIndex.insert(lightIndex.length() - 1, indexStr);
            meshRendererComponent.shader->setUniform((lightIndex + ".lightSpaceMat").c_str(), lightSpaceMat);
            meshRendererComponent.shader->setUniform((lightIndex + ".shadowMap").c_str(), 1);
        });
    }

    void Scene::generateDepthMap(LightComponent& lightComponent, const TransformComponent& transformComponent, const glm::mat4& lightProjection, const glm::mat4& lightView) {
        auto& depthMapSize = lightComponent.getDepthMapSize();
        FBO* fbo = lightComponent.getFBO();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lightComponent.getDepthTextureId());
        Renderer::setViewport(0, 0, depthMapSize.first, depthMapSize.second);
        fbo->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        updateView(lightView, lightProjection, transformComponent.position);
        glCullFace(GL_FRONT);
        setDrawLightMeshes(false);
        draw();
        glCullFace(GL_BACK);
        fbo->unbind();
    }

    bool Scene::updateView() {
        Camera* curCameraFocus = nullptr;
        glm::vec3 cameraPosition(0.f);
        glm::vec3 cameraForward(0.f);
        glm::vec3 cameraUp(0.f);
        if(_isFocusedOnDebugCamera && _debugCamera) {
            curCameraFocus = _debugCamera;
            cameraPosition = _debugCamera->getPosition();
            cameraForward = _debugCamera->getForward();
            cameraUp = _debugCamera->getUp();
        }
        else {
            auto& camsView = _registry->view<CameraComponent, TransformComponent>();
            for(auto camEntity : camsView) {
                auto& [cameraComponent, transformComponent] = camsView.get<CameraComponent, TransformComponent>(camEntity);
                if(cameraComponent.isFocusedOn()){
                    curCameraFocus = cameraComponent.getSource();
                    cameraPosition = transformComponent.position;
                    cameraForward = -transformComponent.getForward();
                    cameraUp = transformComponent.getUp();
                    break;
                }
            }
        }
        if(!curCameraFocus)
            return false;
        updateView(curCameraFocus->getView(cameraPosition, cameraForward, cameraUp), curCameraFocus->getProjection(), cameraPosition);
        return true;
    }

    void Scene::update() {
        _registry->view<ScriptComponent>().each([&](entt::entity entity, ScriptComponent& sc){
            if(!sc.instance) {
                sc.initialize(entity, this);
                sc.instance->start();
            }
            sc.instance->update();
        });
        if(!updateView())
            return;
        uint32_t lightIndex = 0;
        _registry->view<LightComponent, TransformComponent, MeshRendererComponent>()
            .each([&](entt::entity entity, LightComponent& lightComponent, TransformComponent& transformComponent, MeshRendererComponent& meshRendererComponent){
                setLight(lightComponent, transformComponent, meshRendererComponent, lightIndex++);
            });
        setDrawLightMeshes(true);
        draw();
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
            transformComponent.setPos(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
            transformComponent.setRotation(glm::degrees(angle), {axis.getX(), axis.getY() ,axis.getZ()});
        });
        _world->stepSimulation(Time::deltaTime);
    }

    void Scene::linkRigidBody(const PhysicsComponent& physicsComponent) {
        _world->addRigidBody(physicsComponent.getRigidBody());
    }

    void Scene::setFocusOnDebugCamera(bool isFocusedOnDebugCamera){
        this->_isFocusedOnDebugCamera = isFocusedOnDebugCamera;
    }

    void Scene::setDrawLightMeshes(bool drawLightMeshes) {
        this->_drawLightMeshes = drawLightMeshes;
    }

    Entity Scene::createEntity() {
        auto entity = _registry->create();
        _registry->emplace<TransformComponent>(entity);
        return { entity, this };
    }

    bool& Scene::getIsFocusedOnDebugCamera() { return _isFocusedOnDebugCamera; }
    bool Scene::getIsFocusedOnDebugCamera() const noexcept { return _isFocusedOnDebugCamera; }
    bool Scene::getDrawLightMeshes() const noexcept { return _drawLightMeshes; }
    entt::registry* Scene::getRegistry() const noexcept { return _registry.get(); }
    btDynamicsWorld* Scene::getDynamicWorld() const noexcept { return _world.get(); }
}