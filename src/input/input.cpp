#include "input/input.hpp"

namespace TWE {
    InputSpecification* Input::inputSpecification = new InputSpecification();

    void Input::flush() {
        inputSpecification->flush();
    }

    void Input::setWindow(GLFWwindow* window) {
        inputSpecification->setWindow(window);
    }

    void Input::setShowCursor(bool show) {
        inputSpecification->setShowCursor(show);
    }

    void Input::setCloseApplication(bool close) {
        inputSpecification->setCloseApplication(close);
    }

    void Input::setIsMouseMoving(bool isMouseMoving) {
        inputSpecification->setIsMouseMoving(isMouseMoving);
    }

    bool Input::getCloseApplication() {
        return inputSpecification->getCloseApplication();
    }

    bool Input::getShowCursor() {
        return inputSpecification->getShowCursor();
    }

    bool Input::isMouseMoving() {
        return inputSpecification->isMouseMoving();
    }
    
    bool Input::isKeyPressed(Keyboard key) {
        return inputSpecification->isKeyPressed(key);
    }

    bool Input::isKeyPressedOnce(Keyboard key) {
        return inputSpecification->isKeyPressedOnce(key);
    }

    bool Input::isMouseButtonPressed(Mouse button) {
        return inputSpecification->isMouseButtonPressed(button);
    }

    bool Input::isMouseButtonPressedOnce(Mouse button) {
        return inputSpecification->isMouseButtonPressedOnce(button);
    }

    Action Input::keyAction(Keyboard key) {
        return inputSpecification->keyAction(key);
    }

    Action Input::mouseButtonAction(Mouse button) {
        return inputSpecification->mouseButtonAction(button);
    }

    const glm::vec<2, float, glm::qualifier::defaultp>& Input::getMouseOffset() noexcept {
        return inputSpecification->getMouseOffset();
    }

    const glm::vec<2, int, glm::qualifier::defaultp>& Input::getMousePosition() noexcept {
        return inputSpecification->getMousePosition();
    }

    GLFWwindow* Input::getWindow() {
        return inputSpecification->getWindow();
    }

    void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        static float preViewX = xpos;
        static float preViewY = ypos;
        inputSpecification->_mouseOffset.x = xpos - preViewX;
        inputSpecification->_mouseOffset.y = preViewY - ypos;
        inputSpecification->_mousePosition.x = xpos;
        inputSpecification->_mousePosition.y = ypos;
        preViewX = xpos;
        preViewY = ypos;
    }

    void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if(action == Action::PRESS)
            inputSpecification->_mousePressedButtons[button] = true;
        if(action == Action::RELEASE)
            inputSpecification->_mousePressedButtons[button] = false;
        inputSpecification->_mousePressedActions[button] = action;
    }

    void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if(action == Action::PRESS)
            inputSpecification->_keyboardPressedKeys[key] = true;
        if(action == Action::RELEASE)
            inputSpecification->_keyboardPressedKeys[key] = false;
        inputSpecification->_keyboardPressedActions[key] = action;
    }

    InputSpecification* Input::getSource() { return inputSpecification; }
}