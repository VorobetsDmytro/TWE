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
        void flush();
        void setWindow(GLFWwindow* window);
        void setShowCursor(bool show);
        void setCloseApplication(bool close);
        [[nodiscard]] bool getShowCursor();
        [[nodiscard]] bool getCloseApplication();
        [[nodiscard]] bool isMouseMoving();
        [[nodiscard]] bool isKeyPressed(Keyboard key);
        [[nodiscard]] bool isKeyPressedOnce(Keyboard key);
        [[nodiscard]] bool isMouseButtonPressed(Mouse button);
        [[nodiscard]] bool isMouseButtonPressedOnce(Mouse button);
        [[nodiscard]] Action keyAction(Keyboard key);
        [[nodiscard]] Action mouseButtonAction(Mouse button);
        [[nodiscard]] const glm::vec<2, float, glm::qualifier::defaultp>& getMouseOffset() const noexcept;
        [[nodiscard]] const glm::vec<2, int, glm::qualifier::defaultp>& getMousePosition() const noexcept;
        [[nodiscard]] GLFWwindow* getWindow();
    private:
        void setIsMouseMoving(bool isMouseMoving);
        GLFWwindow* _window;
        bool* _keyboardPressedKeys;
        int* _keyboardPressedActions;
        bool* _mousePressedButtons;
        int* _mousePressedActions;
        glm::vec<2, float, glm::qualifier::defaultp> _mouseOffset;
        glm::vec<2, int, glm::qualifier::defaultp> _mousePosition;
        bool _isMouseMoving;
        bool _showCursor;
        bool _closeApp;
        friend class Input;
    };
}

#endif