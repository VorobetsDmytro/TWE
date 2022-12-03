#ifndef GUI_COMPONENTS_HPP
#define GUI_COMPONENTS_HPP

#include "scene/scene.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

namespace TWE {
    class GUIComponents {
    public:
        GUIComponents() = default;
        void showComponentPanel(Entity& entity);
        void setScene(Scene* scene);
    private:
        void showNameComponent(Entity& entity);
        void showTransformComponent(Entity& entity);
        void showMeshComponent(Entity& entity);
        void showMeshRendererComponent(Entity& entity);
        void showCameraComponent(Entity& entity);
        void showLightComponent(Entity& entity);
        void showPhysicsComponent(Entity& entity);
        void showScriptComponent(Entity& entity);
        bool dragFloat3(const std::string& label, glm::vec3& values, float step, float resetValue, float min = 0.f, float max = 0.f, float labelColumnWidth = 80.f);
        bool dragFloat(const std::string& label, float& value, float step, float min = 0.f, float max = 0.f, float labelColumnWidth = 80.f);
        bool colorEdit3(const std::string& label, glm::vec3& values, float labelColumnWidth = 80.f);
        bool checkBox(const std::string& label, bool& value, float labelColumnWidth = 80.f);
        void text(const std::string& label, const std::string& value, float labelColumnWidth = 80.f);
        int combo(const std::string& label, std::string& selected, std::vector<std::string>& options, float labelColumnWidth = 80.f);
        Scene* _scene;
    };
}

#endif