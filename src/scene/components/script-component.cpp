#include "scene/components/script-component.hpp"

namespace TWE {
    ScriptComponent::ScriptComponent(const ScriptComponent& scriptComponent) {
        this->_scripts = scriptComponent._scripts;
    }

    void ScriptComponent::clean() {
        for(auto& script : _scripts)
            if(script.instance)
                delete script.instance;
        _scripts.clear();
    }

    void ScriptComponent::unbind(const std::string& behaviorClassName) {
        auto it = std::find_if(_scripts.begin(), _scripts.end(), [&](const ScriptSpecification& scriptSpecification) {
            return scriptSpecification.behaviorClassName == behaviorClassName;
        });
        if(it == _scripts.end())
            return;
        if(it->instance)
            delete it->instance;
        _scripts.erase(it);
    }

    ScriptSpecification* ScriptComponent::bind(Behavior* behavior, const std::string& behaviorClassName) {
        auto it = std::find_if(_scripts.begin(), _scripts.end(), [&](const ScriptSpecification& scriptSpecification) {
            return scriptSpecification.behaviorClassName == behaviorClassName;
        });
        if(it != _scripts.end())
            return nullptr;
        ScriptSpecification scriptSpecification(behavior, behaviorClassName, true);
        _scripts.push_back(scriptSpecification);
        return &_scripts.back();
    }

    ScriptSpecification* ScriptComponent::getScript(int index) {
        if(_scripts.empty() || index >= _scripts.size())
            return nullptr;
        return &_scripts[index];
    }

    ScriptSpecification* ScriptComponent::getScript(const std::string& behaviorClassName) {
        auto it = std::find_if(_scripts.begin(), _scripts.end(), [&](const ScriptSpecification& scriptSpecification) {
            return scriptSpecification.behaviorClassName == behaviorClassName;
        });
        if(it == _scripts.end())
            return nullptr;
        return &*it;
    }

    std::vector<std::string> ScriptComponent::getScriptsBehaviorName() {
        std::vector<std::string> res;
        for(auto& script : _scripts) 
            res.push_back(script.behaviorClassName);
        return res;
    }

    std::vector<ScriptSpecification>& ScriptComponent::getScripts() { return _scripts; }
}