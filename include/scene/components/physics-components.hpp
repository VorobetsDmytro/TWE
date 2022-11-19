#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include <glm.hpp>
#include <memory>
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"

namespace TWE {
    class PhysicsComponent {
    public:
        PhysicsComponent(const glm::vec3& size, const glm::vec3& pos, float mass);
        PhysicsComponent(const PhysicsComponent& physics);
        void setMass(btDynamicsWorld* dynamicsWorldfloat, float mass);
        void setWorldTransform(const glm::vec3& pos);
        void setRotation(const glm::vec3& rotation);
        void setSize(btDynamicsWorld* dynamicsWorldfloat, const glm::vec3& size);
        [[nodiscard]] float getMass() const noexcept;
        [[nodiscard]] btRigidBody* getRigidBody() const noexcept;
    private:
        float _mass;
        std::shared_ptr<btRigidBody> _rigidBody;
    };
}

#endif