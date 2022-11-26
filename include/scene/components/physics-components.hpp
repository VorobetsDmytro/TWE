#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include <glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"

namespace TWE {
    enum class ColliderType {
        None = -1,
        Box
    };

    class PhysicsComponent {
    public:
        PhysicsComponent();
        PhysicsComponent(ColliderType colliderType, const glm::vec3& size, const glm::vec3& pos, const glm::vec3& rotation, float mass);
        PhysicsComponent(const PhysicsComponent& physics);
        void setMass(btDynamicsWorld* dynamicsWorldfloat, float mass);
        void setPosition(const glm::vec3& pos);
        void setRotation(const glm::vec3& rotation);
        void setRotation(const glm::quat& quatretion);
        void setSize(btDynamicsWorld* dynamicsWorldfloat, const glm::vec3& size);
        void setColliderType(ColliderType type);
        [[nodiscard]] float getMass() const noexcept;
        [[nodiscard]] btRigidBody* getRigidBody() const noexcept;
        [[nodiscard]] ColliderType getColliderType() const noexcept;
    private:
        float _mass;
        std::shared_ptr<btRigidBody> _rigidBody;
        ColliderType _colliderType;
        btTransform _transform;
    };
}

#endif