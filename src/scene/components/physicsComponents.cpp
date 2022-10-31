#include "scene/components/physicsComponents.hpp"

namespace TWE {
    PhysicsComponent::PhysicsComponent(const glm::vec3& size, const glm::vec3& pos, float mass) {
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin({pos.x, pos.y, pos.z});
        btBoxShape* boxShape = new btBoxShape({size.x / 2, size.y / 2, size.z / 2});
        btVector3 inertia(0.f, 0.f, 0.f);
        boxShape->calculateLocalInertia(mass, inertia);
        btMotionState* motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, motionState, boxShape, inertia);
        _rigidBody = std::make_shared<btRigidBody>(constrInfo);
        _rigidBody->setUserPointer(this);
        _mass = mass;
    }

    PhysicsComponent::PhysicsComponent(const PhysicsComponent& physics) {
        this->_mass = physics._mass;
        this->_rigidBody = physics._rigidBody;
    }

    void PhysicsComponent::setMass(btDynamicsWorld* dynamicsWorldfloat, float mass) {
        dynamicsWorldfloat->removeRigidBody(_rigidBody.get());
        _mass = mass;
        btVector3 inertia;
        _rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
        _rigidBody->setMassProps(mass, inertia);
        dynamicsWorldfloat->addRigidBody(_rigidBody.get());
    }

    void PhysicsComponent::setWorldTransform(const glm::vec3& pos) {
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin({pos.x, pos.y, pos.z});
        _rigidBody->setWorldTransform(transform);
    }

    btRigidBody* PhysicsComponent::getRigidBody() const noexcept { return _rigidBody.get(); }
    float PhysicsComponent::getMass() const noexcept { return _mass; }
}