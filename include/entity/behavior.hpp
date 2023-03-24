#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include <string>
#include <filesystem>
#include <glm.hpp>
#include <btBulletDynamicsCommon.h>
#include "imgui.h"

#include "entity.hpp"
#include "input/input.hpp"
#include "scene/shape-specification.hpp"

namespace TWE {
    class Behavior {
    public:
        virtual void start() {}
        virtual void update(float deltaTime) {}
        virtual void collisionDetection(Entity collidedEntity, const btCollisionObject* collisionObj) {}
    protected:
        template<typename T>
        bool hasComponent();
        template<typename T>
        T* getComponent();
        template<typename T, typename ...Args>
        T* addComponent(Args&&... args);
        template<typename T>
        void removeComponent();
        template<typename T = void>
        void destroy();
        void loadScene(const std::filesystem::path& scenePath) { 
           loadScenePath = scenePath;
           needLoadScene = true;
        }
        InputSpecification* input;
        ImGuiContext* imguiContext;
        Entity gameObject;
        std::filesystem::path rootPath;
        ShapeSpecification* shapeSpec;
    private:
        bool needLoadScene = false;
        std::filesystem::path loadScenePath;
        void setInput(InputSpecification* input) {
            Input::inputSpecification = input;
            this->input = input;
        }
        void setImguiContext(ImGuiContext* context) {
            imguiContext = context;
        }
        void setRootPath(const std::filesystem::path& rootPath) {
            this->rootPath = rootPath;
        }
        void setShapeSpecification(ShapeSpecification* shapeSpec) {
            this->shapeSpec = shapeSpec;
        }
        friend class ScriptSpecification;
        friend class SceneScripts;
    };

    template<typename T>
    bool Behavior::hasComponent() {
        return gameObject.hasComponent<T>();
    }

    template<typename T>
    T* Behavior::getComponent() {
        if(!hasComponent<T>()) {
            std::string componentName = typeid(T).name();
            componentName = componentName.substr(6);
            throw std::runtime_error("Error: A " + componentName + " was not found.");
        }
        return &gameObject.getComponent<T>();
    }

    template<typename T, typename ...Args>
    T* Behavior::addComponent(Args&&... args) {
        if(hasComponent<T>()) {
            std::string componentName = typeid(T).name();
            componentName = componentName.substr(6);
            throw std::runtime_error("Error: The " + componentName + " is already exists.");
        }
        return &gameObject.addComponent<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    void Behavior::removeComponent() {
        if(!hasComponent<T>()) {
            std::string componentName = typeid(T).name();
            componentName = componentName.substr(6);
            throw std::runtime_error("Error: A " + componentName + " was not found.");
        }
        gameObject.removeComponent<T>();
    }

    template<typename T>
    void Behavior::destroy() {
        gameObject.destroy();
    }
}

#endif