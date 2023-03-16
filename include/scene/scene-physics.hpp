#ifndef SCENE_PHYSICS_HPP
#define SCENE_PHYSICS_HPP

#include "scene/iscene-physics.hpp"
#include "scene/components/components.hpp"

namespace TWE {
    class ScenePhysics: public IScenePhysics {
    public:
        ScenePhysics();
        void reset(entt::registry* registry) override;
        void updateWorldSimulation(entt::registry* registry, float deltaTime) override;
        void debugDrawWorld() override;
        void checkCollisionsDetection() override;
        void cleanCollisionDetection() override;
        [[nodiscard]] std::vector<const btCollisionObject*> getCollisionDetection(const btCollisionObject* obj) override;
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept override;
        [[nodiscard]] SceneBulletDebugDrawer* getDebugDrawer() const noexcept override;
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