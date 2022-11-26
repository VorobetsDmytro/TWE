#ifndef TEST_HPP
#define TEST_HPP

#include "engine.hpp"
#include "scene/scene-serializer.hpp"
#include "registry/registry-loader.hpp"
#include "stream/registry-recorder.hpp"

namespace TWE {
    class Test final: public Engine {
    public:
        Test(int wndWidth = 800, int wndHeight = 600, const char* title = "The Worst Engine", GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);
    };
}

#endif