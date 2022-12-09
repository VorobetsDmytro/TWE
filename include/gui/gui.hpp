#ifndef GUI_HPP
#define GUI_HPP

#include "scene/scene.hpp"
#include "scene/scene-serializer.hpp"
#include "gui/gui-components.hpp"
#include "registry/registry.hpp"

#include <glfw3.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>
#include <ImGuizmo.h>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace TWE {
    enum class GizmoOperation {
        None = -1,
        Translate = ImGuizmo::OPERATION::TRANSLATE,
        Rotate = ImGuizmo::OPERATION::ROTATE,
        Scale = ImGuizmo::OPERATION::SCALE,
    };

    struct GUISpecification {
        GUISpecification() = default;
        Scene* _scene;
        Entity _selectedEntity;
        GizmoOperation _gizmoOperation;
        Registry<Behavior>* scriptRegistry;
        std::function<void(Registry<Behavior>&)> registryLoader;
        int readPixelFBOData;
        bool isFileDialogOpen;
        bool isMouseOnViewport;
        bool isFocusedOnViewport;
    };

    class GUI{
    public:
        GUI(GLFWwindow *window);
        ~GUI();
        void update();
        void addCheckbox(const char* name, bool& var);
        void addInputText(const char* name, std::string& var);
        void addButton(const char* name, std::function<void()> func);
        void setScene(Scene* scene);
        void setScriptRegistry(Registry<Behavior>* scriptRegistry);
        void setRegistryLoader(std::function<void(Registry<Behavior>&)> registryLoader);
        [[nodiscard]] bool getIsMouseOnViewport();
        [[nodiscard]] bool getIsFocusedOnViewport();
    private:
        void showDockSpace();
        void showScenePanel();
        void showTestPanel();
        void showViewportPanel();
        void showDirectoryPanel();
        void showFileDialog();
        void showSceneEntityPopup(const std::string& popupId);
        void showSceneMenuPopup(const std::string& popupId);
        bool showGizmo();
        void processInput();
        GUIComponents _components;
        GUISpecification _specification;
        std::vector<std::pair<const char*, bool&>> _checkBoxes;
        std::vector<std::pair<const char*, std::string&>> _inputTextes;
        std::vector<std::pair<const char*, std::function<void()>>> _buttons;
    };
}

#endif