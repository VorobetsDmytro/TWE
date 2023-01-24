#include "input/input-specification.hpp"

namespace TWE {
    InputSpecification::InputSpecification() {
        _keyboardPressedKeys = new bool[512]{0};
        _mousePressedButtons = new bool[24]{0};
        _mouseOffset = new float[2]{0};
        _keyboardPressedActions = new int[512]{0};
        _mousePressedActions = new int[512]{0};
        _mousePosition = new int[2]{0};
        _showCursor = true;
        _closeApp = false;
    }

    InputSpecification::InputSpecification(const InputSpecification& inputSpecification) {
        this->_keyboardPressedKeys = inputSpecification._keyboardPressedKeys;
        this->_mousePressedButtons = inputSpecification._mousePressedButtons;
        this->_mouseOffset = inputSpecification._mouseOffset;
        this->_keyboardPressedActions = inputSpecification._keyboardPressedActions;
        this->_mousePressedActions = inputSpecification._mousePressedActions;
        this->_window = inputSpecification._window;
        this->_mousePosition = inputSpecification._mousePosition;
        this->_showCursor = inputSpecification._showCursor;
        this->_closeApp = inputSpecification._closeApp;
    }

    void InputSpecification::setWindow(GLFWwindow* window) {
        _window = window;
    }

    void InputSpecification::setShowCursor(bool show) {
        _showCursor = show;
    }

    void InputSpecification::setCloseApplication(bool close) {
        _closeApp = close;
    }

    bool InputSpecification::isKeyPressed(Keyboard key) { 
        if(_keyboardPressedKeys[key] && _keyboardPressedActions[key] == Action::PRESS)
            _keyboardPressedActions[key] = Action::REPEAT;
        return _keyboardPressedKeys[key];
    }

    bool InputSpecification::isKeyPressedOnce(Keyboard key) {
        if(_keyboardPressedKeys[key] && _keyboardPressedActions[key] == Action::PRESS) {
            _keyboardPressedActions[key] = Action::REPEAT;
            return true;
        }
        return false;
    }

    bool InputSpecification::isMouseButtonPressed(Mouse button) { 
        if(_mousePressedButtons[button] && _mousePressedActions[button] == Action::PRESS)
            _mousePressedActions[button] = Action::REPEAT;
        return _mousePressedButtons[button];
    }

    bool InputSpecification::isMouseButtonPressedOnce(Mouse button) {
        if(_mousePressedButtons[button] && _mousePressedActions[button] == Action::PRESS) {
            _mousePressedActions[button] = Action::REPEAT;
            return true;
        }
        return false;
    }

    bool InputSpecification::getShowCursor() { return _showCursor; }
    bool InputSpecification::getCloseApplication() { return _closeApp; }
    Action InputSpecification::keyAction(Keyboard key) { return static_cast<Action>(_keyboardPressedActions[key]); }
    Action InputSpecification::mouseButtonAction(Mouse button) { return static_cast<Action>(_mousePressedActions[button]); }
    glm::vec2 InputSpecification::getMouseOffset() { return { _mouseOffset[0], _mouseOffset[1] }; }
    glm::vec2 InputSpecification::getMousePosition() { return { _mousePosition[0], _mousePosition[1] }; }
    GLFWwindow* InputSpecification::getWindow() { return _window; }
}