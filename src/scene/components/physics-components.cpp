#include "scene/components/physics-components.hpp"

namespace TWE {
    PhysicsComponent::PhysicsComponent(): _colliderType(ColliderType::None){}

    PhysicsComponent::PhysicsComponent(btDynamicsWorld* dynamicsWorld, ColliderType colliderType, const glm::vec3& shapeSize, 
    const glm::vec3& localScale, const glm::vec3& pos, const glm::vec3& rotation, float mass)
    : _colliderType(colliderType), _dynamicsWorld(dynamicsWorld) {
        _transform.setIdentity();
        _transform.setOrigin({pos.x, pos.y, pos.z});
        _transform.setRotation({rotation.x, rotation.y, rotation.z});
        btBoxShape* boxShape = new btBoxShape({shapeSize.x / 2, shapeSize.y / 2, shapeSize.z / 2});
        boxShape->setLocalScaling({localScale.x, localScale.y, localScale.z});
        boxShape->setMargin(0.f);
        btVector3 inertia(0.f, 0.f, 0.f);
        boxShape->calculateLocalInertia(mass, inertia);
        btMotionState* motionState = new btDefaultMotionState(_transform);
        btRigidBody::btRigidBodyConstructionInfo constrInfo(mass, motionState, boxShape, inertia);
        _rigidBody = new btRigidBody(constrInfo); //std::make_shared<btRigidBody>(constrInfo);
        _rigidBody->setUserPointer(this);
        _mass = mass;
        dynamicsWorld->addRigidBody(_rigidBody);
    }

    PhysicsComponent::PhysicsComponent(const PhysicsComponent& physics) {
        this->_mass = physics._mass;
        this->_rigidBody = physics._rigidBody;
        this->_colliderType = physics._colliderType;
        this->_dynamicsWorld = physics._dynamicsWorld;
        this->_transform = physics._transform;
    }

    void PhysicsComponent::setMass(float mass) {
        _dynamicsWorld->removeRigidBody(_rigidBody);
        _mass = mass;
        btVector3 inertia;
        _rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
        _rigidBody->setMassProps(mass, inertia);
        _rigidBody->setLinearVelocity({0.f, 0.f, 0.f});
        _rigidBody->setPushVelocity({0.f, 0.f, 0.f});
        _rigidBody->setAngularVelocity({0.f, 0.f, 0.f});
        _rigidBody->setTurnVelocity({0.f, 0.f, 0.f});
        _dynamicsWorld->addRigidBody(_rigidBody);
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

    void PhysicsComponent::setSize(const glm::vec3& size) {
        _dynamicsWorld->removeRigidBody(_rigidBody);
        btCollisionShape* shape = _rigidBody->getCollisionShape();
        shape->setLocalScaling({size.x, size.y, size.z});
        _dynamicsWorld->addRigidBody(_rigidBody);
    }

    void PhysicsComponent::setColliderType(ColliderType type) {
        _colliderType = type;
        // TODO: Change a collider type in the rigid body
    }

    void PhysicsComponent::setShapeDimensions(const glm::vec3& size) {
        _dynamicsWorld->removeRigidBody(_rigidBody);
        btVector3 shapeDimensions = { size.x, size.y, size.z };
        btBoxShape* boxShape = (btBoxShape*)_rigidBody->getCollisionShape();
        boxShape->setImplicitShapeDimensions(shapeDimensions / 2);
        _dynamicsWorld->addRigidBody(_rigidBody);
    }

    glm::mat4 PhysicsComponent::getModel() {
        _transform = _rigidBody->getWorldTransform();
        glm::mat4 model;
        _transform.getOpenGLMatrix(glm::value_ptr(model));
        btBoxShape* boxShape = (btBoxShape*)_rigidBody->getCollisionShape();  
        btVector3 shapeDimensions = boxShape->getImplicitShapeDimensions();
        btVector3 localScaling = boxShape->getLocalScaling();
        btVector3& size = (shapeDimensions * 2);
        model = glm::scale(model, {size.x(), size.y(), size.z()});
        return model;
    }

    glm::vec3 PhysicsComponent::getShapeDimensions() { 
        btBoxShape* boxShape = (btBoxShape*)_rigidBody->getCollisionShape();
        btVector3 shapeDimensions = boxShape->getImplicitShapeDimensions();
        btVector3& size = (shapeDimensions * 2);
        return { size.x(), size.y(), size.z() };
    }
    
    glm::vec3 PhysicsComponent::getLocalScale() { 
        auto& localScaling = _rigidBody->getCollisionShape()->getLocalScaling(); 
        return { localScaling.x(), localScaling.y(), localScaling.z() };
    }

    glm::vec3 PhysicsComponent::getPosition() {
        auto& position = _rigidBody->getWorldTransform().getOrigin();
        return { position.x(), position.y(), position.z() };
    }

    glm::vec3 PhysicsComponent::getRotation() {
        auto& quat = _rigidBody->getWorldTransform().getRotation();
        glm::vec3 rot;
        quat.getEulerZYX(rot.z, rot.y, rot.x);
        return rot;
    }

    btRigidBody* PhysicsComponent::getRigidBody() const noexcept { return _rigidBody; }
    btDynamicsWorld* PhysicsComponent::getDynamicsWorld() const noexcept { return _dynamicsWorld; }
    ColliderType PhysicsComponent::getColliderType() const noexcept { return _colliderType; }
    float PhysicsComponent::getMass() const noexcept { return _mass; }
}