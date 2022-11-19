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
    private:
        Behavior* _instance;
        friend class Scene;
    };

    template<typename T>
    void ScriptComponent::bind() {
        initialize = [&](entt::entity entity, Scene* scene){ 
            _instance = static_cast<Behavior*>(new T); 
            _instance->gameObject = { entity, scene };
        };
        destroy = [&](){ 
            delete _instance;
            _instance = nullptr;
        };
    }
}

#endif