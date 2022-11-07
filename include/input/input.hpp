#ifndef INPUT_HPP
#define INPUT_HPP

#include "mouse.hpp"
#include "keyboard.hpp"
#include "action.hpp"

namespace TWE {
    class Input {
    public:
        [[nodiscard]] static bool isKeyPressed(Keyboard key);
        [[nodiscard]] static bool isMouseButtonPressed(Mouse button);
    private:
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static bool keyboardPressedKeys[512];
        static bool mousePressedButtons[24];
        static float xMouseOffset;
        static float yMouseOffset;
        friend class Engine;
    };
}

#endif