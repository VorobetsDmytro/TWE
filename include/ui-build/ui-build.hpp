#ifndef UI_BUILD_HPP
#define UI_BUILD_HPP

#include <glfw3.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

namespace TWE {
    class UIBuild {
    public:
        UIBuild(GLFWwindow *window);
        ~UIBuild();
        void begin();
        void end();
    };
}

#endif