#include "gui/gui-console-panel.hpp"

namespace TWE {
    GUIConsolePanel::GUIConsolePanel() {
        _guiState = nullptr;
    }

    void GUIConsolePanel::setGUIState(GUIStateSpecification* guiState) {
        _guiState = guiState;
        std::cout.rdbuf(_coutBuffer.rdbuf());
    }

    void GUIConsolePanel::showPanel() {
        ImGui::Begin("Console");
        if(!_guiState) {
            ImGui::End();
            return;
        }
        ImGui::Text(_coutBuffer.str().c_str());
        ImGui::End();
    }
}