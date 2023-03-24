#ifndef GUI_COMPONENTS_PANEL_HPP
#define GUI_COMPONENTS_PANEL_HPP

#include "scene/iscene.hpp"
#include "scene/shape.hpp"

#include "gui/gui-components.hpp"
#include "gui/gui-types.hpp"
#include "gui/igui-panel.hpp"

#include "registry/registry.hpp"
#include "undo-redo/ur-commands/ur-commands.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui-filedialog/ImGuiFileDialog.h>

namespace TWE {
    class GUIComponentsPanel: public IGUIPanel {
    public:
        GUIComponentsPanel();
        void showPanel() override;
        void setGUIState(GUIStateSpecification* guiState) override;
    private:
        void showFileDialog();
        void showNameComponent();
        void showTransformComponent();
        void showMeshComponent();
        void showMeshRendererComponent();
        void showCameraComponent();
        void showLightComponent();
        void showPhysicsComponent();
        void showScriptComponent();
        void showAudioComponent();
        void showAddComponentMenu();
        void showSeparator();
        GUIStateSpecification* _guiState;
    };
}

#endif