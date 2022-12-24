#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include <string>
#include <glm.hpp>

#include "entity.hpp"
#include "input/input.hpp"

namespace TWE {
    struct InputSpecification {
    public:
        InputSpecification() = default;
        InputSpecification(bool* keyboardPressedKeys, bool* mousePressedButtons, float* mouseOffset)
            : keyboardPressedKeys(keyboardPressedKeys), mousePressedButtons(mousePressedButtons), mouseOffset(mouseOffset) {}
        bool isKeyPressed(Keyboard key) { return keyboardPressedKeys[key]; }
        bool isMouseButtonPressed(Mouse button) { return mousePressedButtons[button]; }
        glm::vec2 getMouseOffset() { return { mouseOffset[0], mouseOffset[1] }; }
    private:
        bool* keyboardPressedKeys;
        bool* mousePressedButtons;
        float* mouseOffset;
    };

    class Behavior {
    public:
        virtual void start() {}
        virtual void update(float deltaTime) {}
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
        InputSpecification input;
    private:
        void setInput(bool* keyboardPressedKeys, bool* mousePressedButtons, float* mouseOffset) {
            input = { keyboardPressedKeys, mousePressedButtons, mouseOffset };
        }
        Entity gameObject;
        friend class ScriptComponent;
        friend class Scene;
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
            throw std::runtime_error("Error: A " + componentName + " is already exists.");
        }
        return &gameObject.addComponent<T, Args>(args);
    }

    template<typename T>
    void Behavior::removeComponent() {
        gameObject.removeComponent<T>();
    }

    template<typename T>
    void Behavior::destroy() {
        gameObject.destroy();
    }
}

#endif