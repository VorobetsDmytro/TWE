#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "entt/entt.hpp"

namespace TWE {
    class Scene;
    class Entity {
    public:
        Entity();
        Entity(entt::entity entity, Scene* scene);
        Entity(const Entity& entity);
        template<typename T>
        bool hasComponent();
        template<typename T>
        T& getComponent();
        template<typename T, typename ...Args>
        T& addComponent(Args&&... args);
        template<typename T>
        void removeComponent();
    private:
        entt::entity entity;
        Scene* scene;
    };

    template<typename T>
    bool Entity::hasComponent() {
        return scene->_registry->any_of<T>(entity);
    }

    template<typename T>
    T& Entity::getComponent() {
        return scene->_registry->get<T>(entity);
    }

    template<typename T, typename ...Args>
    T& Entity::addComponent(Args&&... args) {
        return scene->_registry->emplace<T>(entity, std::forward<Args>(args)...);
    }

    template<typename T>
    void Entity::removeComponent() {
        scene->_registry->remove<T>(entity);
    }
}

#endif