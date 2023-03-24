#ifndef START_PANEL_HPP
#define START_PANEL_HPP

#include "gui/igui-panel.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>
#include <string>

namespace TWE {
    class GUIStartPanel: public IGUIPanel {
    public:
        GUIStartPanel();
        void showPanel() override;
    private:
        ImFont* fontRobotoBold16;
        ImFont* fontRobotoBold32;
        ImFont* fontRobotoBold64;
        std::string title;
    };
}

#endif