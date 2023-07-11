#include "input/window.hpp"

namespace TWE {
    const FBOAttachmentSpecification Window::_fboAttachments = { FBOTextureFormat::RGBA8, FBOTextureFormat::R32I, FBOTextureFormat::DEPTH24STENCIL8 };

    Window::Window() {
        _window = nullptr;
    }

    Window::~Window() {
        if(!_window)
            return;
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void Window::initGLFW(int& width, int& height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
        _title = title;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef TWE_BUILD
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
        #endif
        _window = glfwCreateWindow(width, height, title, monitor, share);        
        if(!_window){
            std::cout << "Error creating a window.\n";
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_window);
        glfwSetCursorPos(_window, static_cast<GLfloat>(width / 2), static_cast<GLfloat>(height / 2));
    }

    void Window::initGLAD(int width, int height) {
        gladLoadGL();
        glViewport(0, 0, width, height);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Window::initFBO(int width, int height) {
        _frameBuffer = std::make_unique<FBO>(width, height, _fboAttachments);
    }

    void Window::setKeyCallback(GLFWkeyfun callback) {
        glfwSetKeyCallback(_window, callback);
    }

    void Window::setMouseButtonCallback(GLFWmousebuttonfun callback) {
        glfwSetMouseButtonCallback(_window, callback);
    }

    void Window::setCursorPosCallback(GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(_window, callback);
    }

    void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(_window, callback);
    }

    void Window::setWindowCloseCallback(GLFWwindowclosefun callback) {
        glfwSetWindowCloseCallback(_window, callback);
    }

    void Window::setTitle(const std::string& title) {
        glfwSetWindowTitle(_window, title.c_str());
    }

    void Window::setWindowShouldClose(bool shouldClose) {
        glfwSetWindowShouldClose(_window, shouldClose);
    }

    void Window::setInputMode(int mode, int value) {
        glfwSetInputMode(_window, mode, value);
    }

    void Window::setVSync(bool isOn) {
        _vSync = isOn;
        glfwSwapInterval(_vSync ? 1 : 0);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(_window);
    }

    GLFWwindow* Window::getSource() const noexcept { return _window; }
    const std::string& Window::getTitle() const noexcept { return _title; }
    FBO* Window::getFrameBuffer() const noexcept { return _frameBuffer.get(); }
    bool Window::getIsVSyncOn() const noexcept { return _vSync; }
    bool Window::getWindowShouldClose() const noexcept { return glfwWindowShouldClose(_window); }
}