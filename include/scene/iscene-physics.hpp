#ifndef ISCENE_PHYSICS_HPP
#define ISCENE_PHYSICS_HPP

#include <map>
#include <vector>

#include <entt/entt.hpp>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "scene/scene-bullet-debug-drawer.hpp"

namespace TWE {
    class IScenePhysics {
    public:
        IScenePhysics() = default;
        virtual void reset(entt::registry* registry) {}
        virtual void updateWorldSimulation(entt::registry* registry, float deltaTime) {}
        virtual void debugDrawWorld() {}
        virtual void checkCollisionsDetection() {}
        virtual void cleanCollisionDetection() {}
        [[nodiscard]] virtual std::vector<const btCollisionObject*> getCollisionDetection(const btCollisionObject* obj) { return {}; }
        [[nodiscard]] virtual btDynamicsWorld* getDynamicWorld() const noexcept { return nullptr; }
        [[nodiscard]] virtual SceneBulletDebugDrawer* getDebugDrawer() const noexcept { return nullptr; }
    };
}

#endif