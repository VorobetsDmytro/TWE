#include "scene/scene-physics.hpp"

namespace TWE {
    ScenePhysics::ScenePhysics(const std::filesystem::path& rootPath) {
        _collisionConfig = new btDefaultCollisionConfiguration();
        _dispatcher = new btCollisionDispatcher(_collisionConfig);
        _broadPhase = new btDbvtBroadphase();
        _solver = new btSequentialImpulseConstraintSolver();
        _world = new btDiscreteDynamicsWorld(_dispatcher, _broadPhase, _solver, _collisionConfig); 
        _world->setGravity({0.f, -9.81f, 0.f});
        _debugDrawer = new SceneBulletDebugDrawer(rootPath);
        _debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        _world->setDebugDrawer(_debugDrawer);
    }

    void ScenePhysics::reset(entt::registry* registry) {
        registry->view<PhysicsComponent>().each([&](entt::entity entity, PhysicsComponent& physicsComponent){
            if(!physicsComponent.getRigidBody())
                return;
            physicsComponent.getDynamicsWorld()->removeRigidBody(physicsComponent.getRigidBody());
        });
    }

    void ScenePhysics::updateWorldSimulation(entt::registry* registry, float deltaTime) {
        registry->view<PhysicsComponent, TransformComponent>().each([](entt::entity entity, PhysicsComponent& physicsComponent, 
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
            physicsComponent.setNeedUpdate(false);
        });
        _world->stepSimulation(deltaTime);
        checkCollisionsDetection();
    }

    void ScenePhysics::debugDrawWorld() {
        _world->debugDrawWorld();
    }

    void ScenePhysics::checkCollisionsDetection() {
        int numManifolds = _world->getDispatcher()->getNumManifolds();
        for(int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = _world->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = contactManifold->getBody0();
            const btCollisionObject* obB = contactManifold->getBody1();
            int objAIndex = obA->getUserIndex();
            int objBIndex = obB->getUserIndex();
            int numContacts = contactManifold->getNumContacts();
            for(int j = 0; j < numContacts; j++) {
                btManifoldPoint& manifoldPoint = contactManifold->getContactPoint(j);
                btScalar distance = manifoldPoint.getDistance();
                if(distance <= 0.f) {
                    if(std::find(collisions[obA->getUserIndex()].begin(), collisions[obA->getUserIndex()].end(), 
                    obB) == collisions[obA->getUserIndex()].end())
                        collisions[obA->getUserIndex()].emplace_back(obB);
                    if(std::find(collisions[obB->getUserIndex()].begin(), collisions[obB->getUserIndex()].end(), 
                    obA) == collisions[obB->getUserIndex()].end())
                        collisions[obB->getUserIndex()].emplace_back(obA);
                }
            }
        }
    }

    std::vector<const btCollisionObject*> ScenePhysics::getCollisionDetection(const btCollisionObject* obj) {
        auto item = collisions.find(obj->getUserIndex());
        if(item == collisions.end())
            return {};
        return item->second;
    }

    void ScenePhysics::cleanCollisionDetection() {
        collisions.clear();
    }
    
    btDynamicsWorld* ScenePhysics::getDynamicWorld() const noexcept { return _world; }
    SceneBulletDebugDrawer* ScenePhysics::getDebugDrawer() const noexcept { return _debugDrawer; }
}