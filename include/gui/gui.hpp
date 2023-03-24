#ifndef GUI_HPP
#define GUI_HPP

#include "scene/iscene.hpp"
#include "scene/scene-serializer.hpp"

#include "gui/gui-components-panel.hpp"
#include "gui/gui-directory-panel.hpp"
#include "gui/gui-start-panel.hpp"
#include "gui/gui-scene-panel.hpp"
#include "gui/gui-viewport-panel.hpp"
#include "gui/gui-console-panel.hpp"
#include "gui/gui-state-specification.hpp"

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
#include <future>
#include <chrono>

namespace TWE {
    class GUI{
    public:
        GUI(GLFWwindow *window);
        ~GUI();
        void begin();
        void end();
        void setScene(IScene* scene);
        void setWindow(Window* window);
        void setProjectData(ProjectData* projectData);
        [[nodiscard]] bool getHasBGFuncs();
        [[nodiscard]] bool getIsMouseOnViewport();
        [[nodiscard]] bool getIsFocusedOnViewport();
        [[nodiscard]] bool getIsMouseDisabled();
        [[nodiscard]] bool getIsURExecuted();
    private:
        void initPanels();
        void showDockSpace();
        void showFileDialog();
        bool showGizmo();
        void showMenuBar();
        void processInput();
        void processBGFuncs();
        void processUR();
        void selectEntity(Entity& entity);
        void unselectEntity();
        bool isSelectedEntityValid();
        std::unique_ptr<IGUIPanel> _startPanel;
        std::vector<std::unique_ptr<IGUIPanel>> _panels;
        GUIStateSpecification _guiState;
    };
}

#endif