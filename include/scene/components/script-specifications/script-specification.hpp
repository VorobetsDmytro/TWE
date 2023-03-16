#ifndef SCRIPTS_SPECIFICATION_HPP
#define SCRIPTS_SPECIFICATION_HPP

#include <string>
#include <filesystem>

#include "entity/behavior.hpp"

namespace TWE {
    class ScriptSpecification {
    public:
        ScriptSpecification() = default;
        ScriptSpecification(Behavior* instance, const std::string& behaviorClassName, bool isEnabled)
        : instance(instance), behaviorClassName(behaviorClassName), isEnabled(isEnabled) {}
        ScriptSpecification(const ScriptSpecification& scriptSpecification) {
            this->instance = scriptSpecification.instance;
            this->behaviorClassName = scriptSpecification.behaviorClassName;
            this->isEnabled = scriptSpecification.isEnabled;
            this->isInitialized = scriptSpecification.isInitialized;
        }
        void initialize(entt::entity entity, IScene* scene, const std::filesystem::path& rootPath, ShapeSpecification* shapeSpec) {
            instance->gameObject = { entity, scene };
            instance->setInput(Input::getSource());
            instance->setImguiContext(ImGui::GetCurrentContext());
            instance->setShapeSpecification(shapeSpec);
            instance->setRootPath(rootPath);
            instance->needLoadScene = false;
            isInitialized = true;
        }
        Behavior* instance;
        std::string behaviorClassName;
        bool isEnabled;
        bool isInitialized = false;
    };
}

#endif