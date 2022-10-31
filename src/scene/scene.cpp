#include "scene/scene.hpp"

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
    }

    void Scene::setTransMat(const glm::mat4& transform, TransformMatrixOptions option) {
        auto& group = _registry->view<MeshRendererComponent>();
        for(auto entity : group) {
            auto& meshRendererComponent = group.get<MeshRendererComponent>(entity);
            meshRendererComponent.shader->setUniform(TRANS_MAT_OPTIONS[option], transform);
        }
    }

    void Scene::setLight(const LightComponent& light, const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const  uint32_t index) {
        std::string lightIndex = "lights[]";
        lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
        auto& lightEnteties = _registry->view<LightComponent>();
        auto& group = _registry->view<MeshRendererComponent>();
        for(auto entity : group) {
            if(_registry->any_of<LightComponent>(entity))
                continue;
            auto& meshRendererComponent = group.get<MeshRendererComponent>(entity);
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
        }
    }

    void Scene::setViewPos(const glm::vec3& pos) {
        auto& group = _registry->view<MeshRendererComponent>();
        for(auto entity : group) {
            auto& meshRendererComponent = group.get<MeshRendererComponent>(entity);
            meshRendererComponent.shader->setUniform("viewPos", pos);
        }
    }

    void Scene::updateView(const Camera& cam, int wndWidth, int wndHeight) {
        glm::mat4 view = cam.getViewMat();
        glm::mat4 projection = cam.getProjectionMat(wndWidth, wndHeight);
        setTransMat(view, TransformMatrixOptions::VIEW);
        setTransMat(projection, TransformMatrixOptions::PROJECTION);
        setViewPos(cam.getPosition());
    }

    void Scene::updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos) {
        setTransMat(view, TransformMatrixOptions::VIEW);
        setTransMat(projection, TransformMatrixOptions::PROJECTION);
        setViewPos(pos);
    }

    void Scene::generateShadows(uint32_t windowWidth, uint32_t windowHeight) {
        glm::mat4& lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.f);
        int lightIndex = -1;
        auto& group = _registry->view<LightComponent, TransformComponent>();
        for(auto entity : group) {
            ++lightIndex;
            auto& [lightComponent, transformComponent] = group.get<LightComponent, TransformComponent>(entity);
            if(lightComponent.type != "dir")
                continue;
            glm::mat4& lightView = glm::lookAt(transformComponent.position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
            setShadows(lightComponent, lightProjection * lightView, lightIndex);
            generateDepthMap(lightComponent, transformComponent, lightProjection, lightView);
        }
        Renderer::setViewport(0, 0, windowWidth, windowHeight);
    }

    void Scene::setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index) {
        auto& view = _registry->view<MeshRendererComponent>();
        for(auto entity : view) {
            auto& meshRendererComponent = view.get<MeshRendererComponent>(entity);
            if(_registry->any_of<LightComponent>(entity))
                continue;
            std::string lightIndex = "lights[]";
            lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
            meshRendererComponent.shader->setUniform((lightIndex + ".lightSpaceMat").c_str(), lightSpaceMat);
            meshRendererComponent.shader->setUniform((lightIndex + ".shadowMap").c_str(), 1);
        }
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
        draw(false);
        glCullFace(GL_BACK);
        fbo->unbind();
    }

    void Scene::draw(bool drawLightMeshes) {
        uint32_t lightIndex = 0;
        auto& lightsGroup = _registry->view<LightComponent, TransformComponent, MeshRendererComponent>();
        for(auto lightEntity : lightsGroup) {
            auto& [lightComponent, transformComponent, meshRendererComponent] = lightsGroup.get<LightComponent, TransformComponent, MeshRendererComponent>(lightEntity);
            setLight(lightComponent, transformComponent, meshRendererComponent, lightIndex++);
        }
        auto& group = _registry->view<MeshComponent, MeshRendererComponent, TransformComponent>();
        for(auto entity : group) {
            auto& [meshComponent, meshRendererComponent, transformComponent] = group.get<MeshComponent, MeshRendererComponent, TransformComponent>(entity);
            if(_registry->any_of<LightComponent>(entity))
                if(!drawLightMeshes)
                    continue;
            Renderer::execute(&meshComponent, &meshRendererComponent, &transformComponent);
        }
    }

    void Scene::updatePhysics() {
        auto& view = _registry->view<PhysicsComponent, TransformComponent>();
        for(auto entity : view) {
            auto& [physicsComponent, transformComponent] = view.get<PhysicsComponent, TransformComponent>(entity);
            btRigidBody* rigidBody = physicsComponent.getRigidBody();
            rigidBody->activate();
            if(physicsComponent.getMass() == 0.f)
                continue;
            btTransform worldTransform;
            rigidBody->getMotionState()->getWorldTransform(worldTransform);
            btVector3 pos = worldTransform.getOrigin();
            btQuaternion quat = worldTransform.getRotation();
            float angle = quat.getAngle();
            btVector3 axis = quat.getAxis();
            transformComponent.setPos(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
            transformComponent.setRotation(glm::degrees(angle), {axis.getX(), axis.getY() ,axis.getZ()});
        }
        _world->stepSimulation(Time::deltaTime);
    }

    void Scene::linkRigidBody(const PhysicsComponent& physicsComponent) {
        _world->addRigidBody(physicsComponent.getRigidBody());
    }

    Entity Scene::createEntity() {
        auto entity = _registry->create();
        _registry->emplace<TransformComponent>(entity);
        return { entity, this };
    }

    entt::registry* Scene::getRegistry() const noexcept { return _registry.get(); }
    btDynamicsWorld* Scene::getDynamicWorld() const noexcept { return _world.get(); }
}