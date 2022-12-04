#include "input/input.hpp"

namespace TWE {
    bool* Input::keyboardPressedKeys = new bool[512]{0};
    bool* Input::mousePressedButtons = new bool[24]{0};
    float* Input::mouseOffset = new float[2]{0};
    
    bool Input::isKeyPressed(Keyboard key) {
        return keyboardPressedKeys[key];
    }

    bool Input::isMouseButtonPressed(Mouse button) {
        return mousePressedButtons[button];
    }

    void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        static float preViewX = xpos;
        static float preViewY = ypos;
        mouseOffset[0] = xpos - preViewX;
        mouseOffset[1] = preViewY - ypos;
        preViewX = xpos;
        preViewY = ypos;
    }

    void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if(action == GLFW_PRESS)
            mousePressedButtons[button] = true;
        if(action == GLFW_RELEASE)
            mousePressedButtons[button] = false;
    }

    void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if(action == GLFW_PRESS)
            keyboardPressedKeys[key] = true;
        if(action == GLFW_RELEASE)
            keyboardPressedKeys[key] = false;
    }
}