#ifndef GUI_CONSOLE_PANEL_HPP
#define GUI_CONSOLE_PANEL_HPP

#include "gui/igui-panel.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

#include <iostream>
#include <sstream>

namespace TWE {
    class GUIConsolePanel: public IGUIPanel {
    public:
        GUIConsolePanel();
        void showPanel() override;
        void setGUIState(GUIStateSpecification* guiState) override;
    private:
        GUIStateSpecification* _guiState;
        std::ostringstream _coutBuffer;
    };
}

#endif