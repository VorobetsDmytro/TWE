#ifndef INPUT_SPECIFICATION_HPP
#define INPUT_SPECIFICATION_HPP

#include <glm.hpp>

#include "mouse.hpp"
#include "keyboard.hpp"
#include "action.hpp"

namespace TWE {
    class InputSpecification {
    public:
        InputSpecification();
        InputSpecification(const InputSpecification& inputSpecification);
        void setWindow(GLFWwindow* window);
        void setShowCursor(bool show);
        void setCloseApplication(bool close);
        [[nodiscard]] bool getShowCursor();
        [[nodiscard]] bool getCloseApplication();
        [[nodiscard]] bool isKeyPressed(Keyboard key);
        [[nodiscard]] bool isKeyPressedOnce(Keyboard key);
        [[nodiscard]] bool isMouseButtonPressed(Mouse button);
        [[nodiscard]] bool isMouseButtonPressedOnce(Mouse button);
        [[nodiscard]] Action keyAction(Keyboard key);
        [[nodiscard]] Action mouseButtonAction(Mouse button);
        [[nodiscard]] glm::vec2 getMouseOffset();
        [[nodiscard]] glm::vec2 getMousePosition();
        [[nodiscard]] GLFWwindow* getWindow();
    private:
        GLFWwindow* _window;
        bool* _keyboardPressedKeys;
        int* _keyboardPressedActions;
        bool* _mousePressedButtons;
        int* _mousePressedActions;
        float* _mouseOffset;
        int* _mousePosition;
        bool _showCursor;
        bool _closeApp;
        friend class Input;
    };
}

#endif