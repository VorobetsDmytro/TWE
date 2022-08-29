#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "math.h"
#include "object.hpp"
#include "light.hpp"
#include "shape.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "gui.hpp"

class Engine {
public:
    Engine(int wndWidth, int wndHeight, const char* title, GLFWmonitor *monitor, GLFWwindow *share);
    ~Engine();
    void start();
protected:
    virtual void update();
    void draw();
    void keyInput();
    void setVSync(GLboolean isOn);
    GLboolean vSync;
    GLFWwindow* window;
    std::shared_ptr<Scene> curScene;
    std::unique_ptr<GUI> gui;
    bool bDraw;
    static int wndWidth;
    static int wndHeight;
    static Camera camera;
    static bool pressedKeys[1024];
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif