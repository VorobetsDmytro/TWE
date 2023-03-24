#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad.h>
#include <glfw3.h>
#include <string>
#include <iostream>
#include <functional>
#include <memory>

#include "renderer/fbo.hpp"

namespace TWE {
    class Window {
    public:
        Window();
        ~Window();
        void initGLFW(int& width, int& height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        void initGLAD(int width, int height);
        void initFBO(int width, int height);
        void setKeyCallback(GLFWkeyfun callback);
        void setMouseButtonCallback(GLFWmousebuttonfun callback);
        void setCursorPosCallback(GLFWcursorposfun callback);
        void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
        void setWindowCloseCallback(GLFWwindowclosefun callback);
        void setVSync(bool isOn);
        void setTitle(const std::string& title);
        [[nodiscard]] GLFWwindow* getSource() const noexcept;
        [[nodiscard]] const std::string& getTitle() const noexcept;
        [[nodiscard]] FBO* getFrameBuffer() const noexcept;
        [[nodiscard]] bool getIsVSyncOn() const noexcept;
    private:
        bool _vSync;
        std::string _title;
        GLFWwindow* _window;
        std::unique_ptr<FBO> _frameBuffer;
        static const FBOAttachmentSpecification _fboAttachments;
    };
}

#endif