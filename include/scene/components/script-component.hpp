#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <vector>

#include "entity/behavior.hpp"
#include "scene/components/script-specifications/script-specification.hpp"

namespace TWE {
    class ScriptComponent {
    public:
        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent& scriptComponent);
        void clean();
        template<typename T>
        ScriptSpecification* bind();
        ScriptSpecification* bind(Behavior* behavior, const std::string& behaviorClassName);
        void unbind(const std::string& behaviorClassName);
        [[nodiscard]] ScriptSpecification* getScript(int index);
        [[nodiscard]] ScriptSpecification* getScript(const std::string& behaviorClassName);
        [[nodiscard]] std::vector<ScriptSpecification>& getScripts();
        [[nodiscard]] std::vector<std::string> getScriptsBehaviorName();
    private:
        std::vector<ScriptSpecification> _scripts;
    };

    template<typename T>
    ScriptSpecification* ScriptComponent::bind() {
        std::string behaviorClassName = std::string(typeid(T).name()).substr(6);
        auto it = std::find_if(_scripts.begin(), _scripts.end(), [&](const ScriptSpecification& scriptSpecification) {
            return scriptSpecification.behaviorClassName == behaviorClassName;
        });
        if(it != _scripts.end())
            return nullptr;
        ScriptSpecification scriptSpecification(static_cast<Behavior*>(new T), behaviorClassName, true);
        _scripts.push_back(scriptSpecification);
        return &_scripts.back();
    }
}

#endif