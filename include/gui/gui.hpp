#ifndef GUI_HPP
#define GUI_HPP

#include "scene/iscene.hpp"
#include "scene/scene-serializer.hpp"

#include "gui/gui-components-panel.hpp"
#include "gui/gui-directory-panel.hpp"
#include "gui/gui-start-panel.hpp"
#include "gui/gui-scene-panel.hpp"
#include "gui/gui-viewport-panel.hpp"

#include "registry/registry.hpp"
#include "stream/project-creator.hpp"
#include "stream/build-creator.hpp"

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

    struct GUISpecification {
        GUISpecification() = default;
        IScene* _scene;
        Entity _selectedEntity;
        ProjectData* projectData;
        int readPixelFBOData;
        bool isMouseOnViewport;
        bool isFocusedOnViewport;
    };

    class GUI{
    public:
        GUI(GLFWwindow *window);
        ~GUI();
        void begin();
        void end();
        void addCheckbox(const char* name, bool& var);
        void addInputText(const char* name, std::string& var);
        void addButton(const char* name, std::function<void()> func);
        void setScene(IScene* scene);
        void setWindow(Window* window);
        void setProjectData(ProjectData* projectData);
        [[nodiscard]] bool getIsMouseOnViewport();
        [[nodiscard]] bool getIsFocusedOnViewport();
        [[nodiscard]] bool getIsMouseDisabled();
    private:
        void initImguiFileDialog();
        void showDockSpace();
        void showTestPanel();
        void showFileDialog();
        bool showGizmo();
        void processInput();
        void selectEntity(Entity& entity);
        void unselectEntity();
        GUIStartPanel _start;
        GUIComponentsPanel _components;
        GUIDirectoryPanel _directory;
        GUIScenePanel _scene;
        GUIViewportPanel _viewport;
        GUISpecification _specification;
        std::vector<std::pair<const char*, bool&>> _checkBoxes;
        std::vector<std::pair<const char*, std::string&>> _inputTextes;
        std::vector<std::pair<const char*, std::function<void()>>> _buttons;
    };
}

#endif