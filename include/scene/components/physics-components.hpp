#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include <glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <iostream>
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"

#include "twe-math/twe-math.hpp"

namespace TWE {
    enum class ColliderType {
        None = -1,
        Box
    };

    class PhysicsComponent {
    public:
        PhysicsComponent();
        PhysicsComponent(btDynamicsWorld* dynamicsWorld, ColliderType colliderType, const glm::vec3& shapeSize, const glm::vec3& localScale, const glm::vec3& pos, 
            const glm::vec3& rotation, float mass);
        PhysicsComponent(const PhysicsComponent& physics);
        void setMass(float mass);
        void setPosition(const glm::vec3& pos);
        void setRotation(const glm::vec3& rotation);
        void setRotation(const glm::quat& quatretion);
        void setSize(const glm::vec3& size);
        void setColliderType(ColliderType type);
        void setShapeDimensions(const glm::vec3& size);
        [[nodiscard]] glm::mat4 getModel();
        [[nodiscard]] float getMass() const noexcept;
        [[nodiscard]] btRigidBody* getRigidBody() const noexcept;
        [[nodiscard]] ColliderType getColliderType() const noexcept;
        [[nodiscard]] glm::vec3 getShapeDimensions();
        [[nodiscard]] glm::vec3 getLocalScale();
        [[nodiscard]] glm::vec3 getPosition();
        [[nodiscard]] glm::vec3 getRotation();
    private:
        float _mass;
        std::shared_ptr<btRigidBody> _rigidBody;
        ColliderType _colliderType;
        btTransform _transform;
        btDynamicsWorld* _dynamicsWorld;
    };
}

#endif