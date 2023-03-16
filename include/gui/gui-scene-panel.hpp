#ifndef SCENE_PANEL_HPP
#define SCENE_PANEL_HPP

#include "scene/iscene.hpp"
#include "scene/shape.hpp"
#include "gui/gui-components.hpp"
#include "gui/gui-types.hpp"
#include "undo-redo/ur-commands/ur-commands.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>

namespace TWE {
    class GUIScenePanel {
    public:
        GUIScenePanel() = default;
        void showPanel(Entity& selectedEntity);
        void setScene(IScene* scene);
        void addEntityToSelected(Entity& selectedEntity);
        bool isSceneEntityPopupOpen();
    private:
        void showSceneEntity(Entity& entity, Entity& selectedEntity);
        void showSceneEntityPopup(const std::string& popupId, Entity& selectedEntity);
        void showSceneMenuPopup(const std::string& popupId, Entity& selectedEntity);
        bool showCreateEntityMenu(Entity& selectedEntity);
        void selectEntity(Entity& entity, Entity& selectedEntity);
        void unselectEntity(Entity& entity);
        void showSeparator(Entity& entity, bool isUpper = false);
        IScene* _scene;
        bool _canOpenWindowPopup;
        bool _showSceneEntityPopup;
        Entity _showSceneEntity;
    };
}

#endif