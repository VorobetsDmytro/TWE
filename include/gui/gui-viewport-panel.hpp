#ifndef GUI_VIEWPORT_PANEL_HPP
#define GUI_VIEWPORT_PANEL_HPP

#include "scene/scene.hpp"

#include "undo-redo/ur-commands/ur-commands.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>
#include <ImGuizmo.h>

namespace TWE {
    enum class GizmoOperation {
        None = -1,
        Translate = ImGuizmo::OPERATION::TRANSLATE,
        Rotate = ImGuizmo::OPERATION::ROTATE,
        Scale = ImGuizmo::OPERATION::SCALE,
    };

    class GUIViewportPanel {
    public:
        GUIViewportPanel();
        void showPanel(Entity& selectedEntity, bool& isFocusedOnViewport, bool& isMouseOnViewport);
        void setScene(Scene* scene);
        void setGizmoOperation(GizmoOperation operation);
    private:
        void selectEntity(Entity& entity, Entity& selectedEntity);
        void unselectEntity(Entity& entity);
        void showViewportStatePanel(Entity& selectedEntity);
        bool getIsMouseDisabled();
        bool showGizmo(Entity& selectedEntity);
        Scene* _scene;
        GizmoOperation _gizmoOperation;
    };
}

#endif