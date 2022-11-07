#ifndef GUI_HPP
#define GUI_HPP

#include <glfw3.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <functional>

namespace TWE {
    class GUI{
    public:
        GUI(GLFWwindow *window);
        ~GUI();
        void update();
        void addCheckbox(const char* name, bool& var);
        void addInputText(const char* name, std::string& var);
        void addButton(const char* name, std::function<void()> func);
        void setSize(float width, float height);
        ImGuiIO io;
    private:
        bool _isInit;
        float _width;
        float _height;
        std::vector<std::pair<const char*, bool&>> _checkBoxes;
        std::vector<std::pair<const char*, std::string&>> _inputTextes;
        std::vector<std::pair<const char*, std::function<void()>>> _buttons;
    };
}

#endif