#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <functional>

#include "entity/behavior.hpp"

namespace TWE {
    class ScriptComponent {
    public:
        ScriptComponent();
        template<typename T>
        void bind();
        std::function<void(entt::entity entity, Scene* scene)> initialize; 
        std::function<void()> destroy; 
        Behavior* instance;
    };

    template<typename T>
    void ScriptComponent::bind() {
        initialize = [&](entt::entity entity, Scene* scene){ 
            instance = static_cast<Behavior*>(new T); 
            instance->gameObject = { entity, scene };
        };
        destroy = [&](){ 
            delete instance;
            instance = nullptr;
        };
    }
}

#endif