#ifndef TEST_HPP
#define TEST_HPP

#include "engine.hpp"
#include "camera-controller.hpp"
#include "rotate-rgb-cube.hpp"
#include "dir-behavior.hpp"

namespace TWE {
    class Test final: public Engine {
    public:
        Test(int wndWidth = 800, int wndHeight = 600, const char* title = "The Worst Engine", GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);
    };
}

#endif