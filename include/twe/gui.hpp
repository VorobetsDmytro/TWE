#ifndef GUI_HPP
#define GUI_HPP

#include <glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

class GUI{
public:
    GUI(GLFWwindow *window);
    ~GUI();
    void draw();
    void addCheckbox(const char* name, bool& var);
    ImGuiIO io;
private:
    std::vector<std::pair<const char*, bool&>> _checkBoxes;
};

#endif