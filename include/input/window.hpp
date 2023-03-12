#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad.h>
#include <glfw3.h>
#include <string>
#include <iostream>
#include <functional>

namespace TWE {
    class Window {
    public:
        Window();
        ~Window();
        void initGLFW(int& width, int& height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        void initGLAD(int width, int height);
        void setKeyCallback(GLFWkeyfun callback);
        void setMouseButtonCallback(GLFWmousebuttonfun callback);
        void setCursorPosCallback(GLFWcursorposfun callback);
        void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
        void setVSync(GLboolean isOn);
        [[nodiscard]] GLFWwindow* getSource() const noexcept;
        [[nodiscard]] const std::string& getTitle() const noexcept;
    private:
        GLboolean _vSync;
        std::string _title;
        GLFWwindow* _window;
    };
}

#endif