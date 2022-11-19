#include "scene/components/physics-components.hpp"

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
        btTransform oldTransform = _rigidBody->getWorldTransform();
        btQuaternion oldBodyRotation = oldTransform.getRotation();
        btTransform transform;
        transform.setIdentity();
        transform.setRotation(oldBodyRotation);
        transform.setOrigin({pos.x, pos.y, pos.z});
        _rigidBody->setWorldTransform(transform);
    }

    void PhysicsComponent::setRotation(const glm::vec3& rotation) {
        btQuaternion deltaLocalRotation(rotation.y, rotation.x, rotation.z);
        btTransform transform = _rigidBody->getWorldTransform();
        btQuaternion oldBodyRotation = transform.getRotation();
        btQuaternion newBodyRotation;
        if(rotation == glm::vec3(0.f))
            newBodyRotation = {0.f, 0.f, 0.f};
        else if(_mass == 0.f)
            newBodyRotation = oldBodyRotation + deltaLocalRotation;
        else
            newBodyRotation = oldBodyRotation * deltaLocalRotation;
        transform.setRotation(newBodyRotation);
        _rigidBody->setWorldTransform(transform);
    }

    void PhysicsComponent::setSize(btDynamicsWorld* dynamicsWorldfloat, const glm::vec3& size) {
        dynamicsWorldfloat->removeRigidBody(_rigidBody.get());
        btCollisionShape* shape = _rigidBody->getCollisionShape();
        shape->setLocalScaling({size.x, size.y, size.z});
        dynamicsWorldfloat->addRigidBody(_rigidBody.get());
    }

    btRigidBody* PhysicsComponent::getRigidBody() const noexcept { return _rigidBody.get(); }
    float PhysicsComponent::getMass() const noexcept { return _mass; }
}