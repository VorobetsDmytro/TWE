#ifndef GUI_VIEWPORT_PANEL_HPP
#define GUI_VIEWPORT_PANEL_HPP

#include "scene/iscene.hpp"
#include "input/window.hpp"

#include "gui/igui-panel.hpp"

#include "undo-redo/ur-commands/ur-commands.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>

namespace TWE {
    class GUIViewportPanel: public IGUIPanel {
    public:
        GUIViewportPanel();
        void showPanel() override;
        void setGUIState(GUIStateSpecification* guiState) override;
    private:
        void selectEntity(Entity& entity);
        void unselectEntity(Entity& entity);
        void showViewportStatePanel();
        bool getIsMouseDisabled();
        bool showGizmo();
        GUIStateSpecification* _guiState;
    };
}

#endif