#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>

#include "engine.hpp"
#include "scene/scene-serializer.hpp"
#include "stream/registry-recorder.hpp"
#include "stream/dll-creator.hpp"

namespace TWE {
    class Test final: public Engine {
    public:
        Test(int wndWidth = 800, int wndHeight = 600, const char* title = "The Worst Engine", GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);
    };
}

#endif