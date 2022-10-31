#include "entity/entity.hpp"

namespace TWE {
    Entity::Entity(): entity(entt::null), scene(nullptr) {}

    Entity::Entity(entt::entity entity, TWE::Scene* scene): entity(entity), scene(scene) {}

    Entity::Entity(const Entity& entity) {
        this->entity = entity.entity;
        this->scene = entity.scene;
    }
}