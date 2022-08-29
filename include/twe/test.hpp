#ifndef TEST_HPP
#define TEST_HPP

#include "engine.hpp"

class Test final: public Engine {
public:
    Test(int wndWidth = 800, int wndHeight = 600, const char* title = "The Worst Engine", GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);
    void update() override;
};

#endif