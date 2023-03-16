#include "gui/gui-start-panel.hpp"

namespace TWE {
    void GUIStartPanel::initialize() {
        title = "The Worst Engine";
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        fontRobotoBold16 = io.Fonts->AddFontFromFileTTF("../../fonts/Roboto-Bold.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        fontRobotoBold32 = io.Fonts->AddFontFromFileTTF("../../fonts/Roboto-Bold.ttf", 32.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        fontRobotoBold64 = io.Fonts->AddFontFromFileTTF("../../fonts/Roboto-Bold.ttf", 64.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".project", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
    }

    void GUIStartPanel::setScene(IScene* scene) {
        _scene = scene;
    }

    void GUIStartPanel::showPanel() {
        ImGui::Begin("Start panel");

        static float titleWidth = 1.f;
        auto& windowSize = ImGui::GetWindowSize();
        float titlePosX = (windowSize.x * 0.5f) - (titleWidth - (titleWidth * 0.5f));

        ImGui::PushFont(fontRobotoBold64);
        ImGui::SetCursorPos({titlePosX, windowSize.y / 4});
        ImGui::Text(title.c_str());
        titleWidth = ImGui::GetItemRectSize().x;
        ImGui::PopFont();

        static float btnWidth = 200.f;
        float btnPosX = (windowSize.x * 0.5f) - (btnWidth - (btnWidth * 0.5f));

        ImGui::PushFont(fontRobotoBold32);
        ImGui::SetCursorPos({btnPosX, windowSize.y / 2.f});
        if(ImGui::Button("New project", { btnWidth, 0.f }))
            ImGuiFileDialog::Instance()->OpenDialog("NewProject", "Choose File", ".project", ".", 1, nullptr);
        ImGui::SetCursorPos({btnPosX, windowSize.y / 2.f + 50.f});
        if(ImGui::Button("Open project", { btnWidth, 0.f }))
            ImGuiFileDialog::Instance()->OpenDialog("OpenProject", "Choose File", ".project", ".", 1, nullptr);
        ImGui::PopFont();
        
        ImGui::End();
    }
}