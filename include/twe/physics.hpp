#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <glm.hpp>
#include <memory>
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"

class Physics {
public:
    Physics(const glm::vec3& size, const glm::vec3& pos, float mass, bool isDynamic = false);
    Physics(const Physics& physics);
    void setDynamic(bool isDynamic);
    void setMass(float mass);
    void create(const glm::vec3& size, const glm::vec3& pos, float mass, bool isDynamic = false);
    void setWorldTransform(const glm::vec3& pos);
    [[nodiscard]] btRigidBody* getRigidBody() const noexcept;
    [[nodiscard]] bool isDynamic() const noexcept;
private:
    float _mass;
    bool _isDynamic;
    std::shared_ptr<btRigidBody> _rigidBody;
};

#endif