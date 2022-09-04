#include "gui.hpp"

GUI::GUI(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos({0.f, 0.f}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({150.f, 100.f}, ImGuiCond_Once);

    ImGui::Begin("TWEUI");
    ImGui::Text("TWE Version 0.0.2");
    for(auto checkbox : _checkBoxes)
        ImGui::Checkbox(checkbox.first, &checkbox.second);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::addCheckbox(const char* name, bool& var) {
    _checkBoxes.push_back({name, var});
}