#include "entity/entity.hpp"

namespace TWE {
    Entity::Entity(): entity(entt::null), scene(nullptr) {}

    Entity::Entity(entt::entity entity, TWE::Scene* scene): entity(entity), scene(scene) {}

    Entity::Entity(const Entity& entity) {
        this->entity = entity.entity;
        this->scene = entity.scene;
    }

    bool Entity::operator==(const Entity& other) {
        return this->entity == other.entity && this->scene == other.scene;
    }

    bool Entity::operator!=(const Entity& other) {
        return !(*this == other);
    }

    entt::entity Entity::getSource() const noexcept { return entity; }
}