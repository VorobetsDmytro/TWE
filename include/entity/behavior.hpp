#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include "entity.hpp"
#include "input/input.hpp"

namespace TWE {
    class Behavior {
    public:
        virtual void start() {}
        virtual void update(float deltaTime) {}
    protected:
        template<typename T>
        bool hasComponent();
        template<typename T>
        T& getComponent();
        template<typename T, typename ...Args>
        T& addComponent(Args&&... args);
        template<typename T>
        void removeComponent();
        Entity gameObject;
    private:
        void setInput(bool* keyboardPressedKeys, bool* mousePressedButtons, float* mouseOffset) {
            Input::keyboardPressedKeys = keyboardPressedKeys;
            Input::mousePressedButtons = mousePressedButtons;
            Input::mouseOffset = mouseOffset;
        }
        friend class ScriptComponent;
        friend class Scene;
    };

    template<typename T>
    bool Behavior::hasComponent() {
        return gameObject.hasComponent<T>();
    }

    template<typename T>
    T& Behavior::getComponent() {
        return gameObject.getComponent<T>();
    }

    template<typename T, typename ...Args>
    T& Behavior::addComponent(Args&&... args) {
        return gameObject.addComponent<T, Args>(args);
    }

    template<typename T>
    void Behavior::removeComponent() {
        gameObject.removeComponent<T>();
    }
}

#endif