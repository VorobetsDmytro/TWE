#ifndef SCENE_PHYSICS_HPP
#define SCENE_PHYSICS_HPP

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <map>
#include <vector>
#include <entt/entt.hpp>

#include "scene/scene-bullet-debug-drawer.hpp"
#include "scene/components/components.hpp"

namespace TWE {
    class ScenePhysics {
    public:
        ScenePhysics() = default;
        ScenePhysics(const std::filesystem::path& rootPath);
        void reset(entt::registry* registry);
        void updateWorldSimulation(entt::registry* registry, float deltaTime);
        void debugDrawWorld();
        void checkCollisionsDetection();
        void cleanCollisionDetection();
        [[nodiscard]] std::vector<const btCollisionObject*> getCollisionDetection(const btCollisionObject* obj);
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept;
        [[nodiscard]] SceneBulletDebugDrawer* getDebugDrawer() const noexcept;
    private:
        std::map<int, std::vector<const btCollisionObject*>> collisions;
        btDynamicsWorld* _world;
        btDispatcher* _dispatcher;
        btConstraintSolver* _solver;
        btCollisionConfiguration* _collisionConfig;
        btBroadphaseInterface* _broadPhase;
        SceneBulletDebugDrawer* _debugDrawer;
    };
}

#endif