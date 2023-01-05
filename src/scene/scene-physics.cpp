#include "scene/scene-physics.hpp"

namespace TWE {
    ScenePhysics::ScenePhysics() {
        _collisionConfig = new btDefaultCollisionConfiguration();
        _dispatcher = new btCollisionDispatcher(_collisionConfig);
        _broadPhase = new btDbvtBroadphase();
        _solver = new btSequentialImpulseConstraintSolver();
        _world = new btDiscreteDynamicsWorld(_dispatcher, _broadPhase, _solver, _collisionConfig); 
        _world->setGravity({0.f, -9.81f, 0.f});
        _debugDrawer = new SceneBulletDebugDrawer();
        _debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        _world->setDebugDrawer(_debugDrawer);
    }

    void ScenePhysics::debugDrawWorld() {
        _world->debugDrawWorld();
    }
    
    btDynamicsWorld* ScenePhysics::getDynamicWorld() const noexcept { return _world; }
    SceneBulletDebugDrawer* ScenePhysics::getDebugDrawer() const noexcept { return _debugDrawer; }
}