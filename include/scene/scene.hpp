#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <map>
#include <algorithm>
#include <iterator>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <entt/entt.hpp>

#include "renderer/camera.hpp"
#include "time.hpp"
#include "renderer/renderer.hpp"
#include "entity/entity.hpp"

namespace TWE {
    class Scene {
    public:
        Scene();
        void setTransMat(const glm::mat4& transform, TransformMatrixOptions option);
        void setLight(const LightComponent& light, const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const  uint32_t index);
        void setViewPos(const glm::vec3& pos);
        void updateView(const Camera& cam, int wndWidth, int wndHeight);
        void updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos);
        void draw(bool drawLightMeshes);
        void generateShadows(uint32_t windowWidth, uint32_t windowHeight);
        void updatePhysics();
        void linkRigidBody(const PhysicsComponent& physicsComponent);
        Entity createEntity();
        [[nodiscard]] entt::registry* getRegistry() const noexcept;
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept;
    private:
        void generateDepthMap(LightComponent& lightComponent, const TransformComponent& transformComponent, const glm::mat4& lightProjection, const glm::mat4& lightView);
        void setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index);
        GLint _lightsCount;
        std::unique_ptr<btDynamicsWorld> _world;
        std::unique_ptr<btDispatcher> _dispatcher;
        std::unique_ptr<btConstraintSolver> _solver;
        std::unique_ptr<btCollisionConfiguration> _collisionConfig;
        std::unique_ptr<btBroadphaseInterface> _broadPhase;
        std::unique_ptr<entt::registry> _registry;
        friend class Entity;
    };
}

#endif