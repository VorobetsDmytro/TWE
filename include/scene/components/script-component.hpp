#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <functional>
#include <string>

#include "entity/behavior.hpp"

namespace TWE {
    class ScriptComponent {
    public:
        ScriptComponent();
        template<typename T>
        void bind();
        void bind(Behavior* behavior, const std::string& behaviorClassName) {
            _behaviorClassName = behaviorClassName;
            _tempInstance = behavior;
            initialize = [&](entt::entity entity, Scene* scene){ 
                _instance = _tempInstance;
                _instance->gameObject = { entity, scene };
                _instance->setInput(Input::keyboardPressedKeys, Input::mousePressedButtons, Input::mouseOffset);
            };
            destroy = [&](){ 
                delete _instance;
                _instance = nullptr;
            };
        }
        [[nodiscard]] std::string getBehaviorClassName() const noexcept {
            return _behaviorClassName;
        }
        std::function<void(entt::entity entity, Scene* scene)> initialize; 
        std::function<void()> destroy;
    private:
        Behavior* _tempInstance;
        Behavior* _instance;
        std::string _behaviorClassName;
        friend class Scene;
    };

    template<typename T>
    void ScriptComponent::bind() {
        _behaviorClassName = std::string(typeid(T).name()).substr(6);
        initialize = [&](entt::entity entity, Scene* scene){ 
            _instance = static_cast<Behavior*>(new T);
            _instance->gameObject = { entity, scene };
            _instance->setInput(Input::keyboardPressedKeys, Input::mousePressedButtons, Input::mouseOffset);
        };
        destroy = [&](){ 
            delete _instance;
            _instance = nullptr;
        };
    }
}

#endif