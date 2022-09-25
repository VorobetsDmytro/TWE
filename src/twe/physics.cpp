#include "physics.hpp"

Physics::Physics(const glm::vec3& size, const glm::vec3& pos, float mass, bool isDynamic) {
    create(size, pos, mass, isDynamic);
}

Physics::Physics(const Physics& physics) {
    this->_mass = physics._mass;
    this->_isDynamic = physics._isDynamic;
    this->_rigidBody = physics._rigidBody;
}

void Physics::create(const glm::vec3& size, const glm::vec3& pos, float mass, bool isDynamic) {
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
    _isDynamic = isDynamic;
    _mass = mass;
}

void Physics::setDynamic(bool isDynamic) {
    _isDynamic = isDynamic;
}

void Physics::setMass(float mass) {
    _mass = mass;
    _rigidBody->setMassProps(mass, _rigidBody->getLocalInertia());
}

void Physics::setWorldTransform(const glm::vec3& pos) {
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin({pos.x, pos.y, pos.z});
    _rigidBody->setWorldTransform(transform);
}

btRigidBody* Physics::getRigidBody() const noexcept { return _rigidBody.get(); }
bool Physics::isDynamic() const noexcept { return _isDynamic; }