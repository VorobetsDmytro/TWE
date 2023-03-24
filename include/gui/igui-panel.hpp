#ifndef IGUI_PANEL_HPP
#define IGUI_PANEL_HPP

#include "gui/gui-state-specification.hpp"

namespace TWE {
    class IGUIPanel {
    public:
        virtual void showPanel() {}
        virtual void setGUIState(GUIStateSpecification* guiState) {}
    };
}

#endif