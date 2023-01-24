#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>

#include "input/input-specification.hpp"

namespace TWE {
    class Input {
    public:
        static void setWindow(GLFWwindow* window);
        static void setShowCursor(bool show);
        static void setCloseApplication(bool close);
        [[nodiscard]] static bool getShowCursor();
        [[nodiscard]] static bool getCloseApplication();
        [[nodiscard]] static bool isKeyPressed(Keyboard key);
        [[nodiscard]] static bool isKeyPressedOnce(Keyboard key);
        [[nodiscard]] static bool isMouseButtonPressed(Mouse button);
        [[nodiscard]] static bool isMouseButtonPressedOnce(Mouse button);
        [[nodiscard]] static Action keyAction(Keyboard key);
        [[nodiscard]] static Action mouseButtonAction(Mouse button);
        [[nodiscard]] static InputSpecification* getSource();
        [[nodiscard]] static glm::vec2 getMouseOffset();
        [[nodiscard]] static glm::vec2 getMousePosition();
        [[nodiscard]] static GLFWwindow* getWindow();
    private:
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static InputSpecification* inputSpecification;
        friend class Engine;
        friend class Scene;
        friend class Behavior;
        friend class ScriptComponent;
    };
}

#endif