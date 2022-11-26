#include "scene/components/physics-components.hpp"

namespace TWE {
    PhysicsComponent::PhysicsComponent(): _colliderType(ColliderType::None){}

    PhysicsComponent::PhysicsComponent(ColliderType colliderType, const glm::vec3& size, const glm::vec3& pos, const glm::vec3& rotation, float mass)
    : _colliderType(colliderType) {
        _transform.setIdentity();
        _transform.setOrigin({pos.x, pos.y, pos.z});
        _transform.setRotation({rotation.x, rotation.y, rotation.z});
        btBoxShape* boxShape = new btBoxShape({size.x / 2, size.y / 2, size.z / 2});
        btVector3 inertia(0.f, 0.f, 0.f);
        boxShape->calculateLocalInertia(mass, inertia);
        btMotionState* motionState = new btDefaultMotionState(_transform);
        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, motionState, boxShape, inertia);
        _rigidBody = std::make_shared<btRigidBody>(constrInfo);
        _rigidBody->setUserPointer(this);
        _mass = mass;
    }

    PhysicsComponent::PhysicsComponent(const PhysicsComponent& physics) {
        this->_mass = physics._mass;
        this->_rigidBody = physics._rigidBody;
        this->_colliderType = physics._colliderType;
    }

    void PhysicsComponent::setMass(btDynamicsWorld* dynamicsWorldfloat, float mass) {
        dynamicsWorldfloat->removeRigidBody(_rigidBody.get());
        _mass = mass;
        btVector3 inertia;
        _rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
        _rigidBody->setMassProps(mass, inertia);
        _rigidBody->setLinearVelocity({0.f, 0.f, 0.f});
        _rigidBody->setPushVelocity({0.f, 0.f, 0.f});
        _rigidBody->setAngularVelocity({0.f, 0.f, 0.f});
        _rigidBody->setTurnVelocity({0.f, 0.f, 0.f});
        dynamicsWorldfloat->addRigidBody(_rigidBody.get());
    }

    void PhysicsComponent::setPosition(const glm::vec3& pos) {
        btTransform oldTransform = _rigidBody->getWorldTransform();
        btQuaternion oldBodyRotation = oldTransform.getRotation();
        _transform.setIdentity();
        _transform.setRotation(oldBodyRotation);
        _transform.setOrigin({pos.x, pos.y, pos.z});
        _rigidBody->setWorldTransform(_transform);
    }

    void PhysicsComponent::setRotation(const glm::vec3& rotation) {
        _transform = _rigidBody->getWorldTransform();
        btQuaternion newRotation(rotation.y, rotation.x, rotation.z);
        btQuaternion oldBodyRotation = _transform.getRotation();
        btQuaternion deltaRotation = newRotation - oldBodyRotation;
        _transform.setRotation(oldBodyRotation + deltaRotation);
        _rigidBody->setWorldTransform(_transform);
    }

    void PhysicsComponent::setRotation(const glm::quat& quatretion) {
        _transform = _rigidBody->getWorldTransform();
        _transform.setRotation({quatretion.x, quatretion.y, quatretion.z, quatretion.w});
        _rigidBody->setWorldTransform(_transform);
    }

    void PhysicsComponent::setSize(btDynamicsWorld* dynamicsWorldfloat, const glm::vec3& size) {
        dynamicsWorldfloat->removeRigidBody(_rigidBody.get());
        btCollisionShape* shape = _rigidBody->getCollisionShape();
        shape->setLocalScaling({size.x, size.y, size.z});
        dynamicsWorldfloat->addRigidBody(_rigidBody.get());
    }

    void PhysicsComponent::setColliderType(ColliderType type) {
        _colliderType = type;
        // TODO: Change a collider type in the rigid body
    }

    btRigidBody* PhysicsComponent::getRigidBody() const noexcept { return _rigidBody.get(); }
    ColliderType PhysicsComponent::getColliderType() const noexcept { return _colliderType; }
    float PhysicsComponent::getMass() const noexcept { return _mass; }
}