#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "entt/entt.hpp"

namespace TWE {
    class IScene;
    class Entity {
    public:
        Entity();
        Entity(entt::entity entity, IScene* scene);
        Entity(const Entity& entity);
        template<typename T>
        bool hasComponent();

        template<typename T>
        T& getComponent();

        template<typename T, typename ...Args>
        T& addComponent(Args&&... args);

        template<typename T>
        void removeComponent();

        template<typename T = void>
        void destroy();

        bool operator==(const Entity& other);
        bool operator!=(const Entity& other);
        [[nodiscard]] IScene* getScene();
        [[nodiscard]] entt::entity getSource() const noexcept;
    private:
        entt::entity _entity;
        IScene* _scene;
    };

    template<typename T>
    bool Entity::hasComponent() {
        return _scene->getSceneRegistry()->current->entityRegistry.any_of<T>(_entity);
    }

    template<typename T>
    T& Entity::getComponent() {
        if(!hasComponent<T>()) {
            std::string componentName = typeid(T).name();
            componentName = componentName.substr(6);
            throw std::runtime_error("Error: A " + componentName + " was not found.");
        }
        return _scene->getSceneRegistry()->current->entityRegistry.get<T>(_entity);
    }

    template<typename T, typename ...Args>
    T& Entity::addComponent(Args&&... args) {
        if(hasComponent<T>()) {
            std::string componentName = typeid(T).name();
            componentName = componentName.substr(6);
            throw std::runtime_error("Error: A " + componentName + " is already exists.");
        }
        return _scene->getSceneRegistry()->current->entityRegistry.emplace<T>(_entity, std::forward<Args>(args)...);
    }

    template<typename T>
    void Entity::removeComponent() {
        if(!hasComponent<T>()) {
            std::string componentName = typeid(T).name();
            componentName = componentName.substr(6);
            throw std::runtime_error("Error: A " + componentName + " was not found.");
        }
        _scene->getSceneRegistry()->current->entityRegistry.remove<T>(_entity);
    }

    template<typename T>
    void Entity::destroy() {
        _scene->cleanEntity(*this);
        _entity = entt::null;
        _scene = nullptr;
    }
}

#endif