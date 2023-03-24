#ifndef SCENE_PANEL_HPP
#define SCENE_PANEL_HPP

#include "scene/iscene.hpp"
#include "scene/shape.hpp"

#include "gui/gui-components.hpp"
#include "gui/gui-types.hpp"
#include "gui/igui-panel.hpp"

#include "undo-redo/ur-commands/ur-commands.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>

namespace TWE {
    class GUIScenePanel: public IGUIPanel {
    public:
        GUIScenePanel();
        void showPanel() override;
        void setGUIState(GUIStateSpecification* guiState) override;
    private:
        void showFileDialog();
        void addEntityToSelected();
        void showSceneEntity(Entity& entity);
        void showSceneEntityPopup(const std::string& popupId);
        void showSceneMenuPopup(const std::string& popupId);
        bool showCreateEntityMenu();
        void selectEntity(Entity& entity);
        void unselectEntity(Entity& entity);
        void showSeparator(Entity& entity, bool isUpper = false);
        bool _canOpenWindowPopup;
        bool _showSceneEntityPopup;
        Entity _showSceneEntity;
        GUIStateSpecification* _guiState;
    };
}

#endif