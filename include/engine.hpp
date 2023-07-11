#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <glad.h>
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "math.h"
#include "scene/shape.hpp"
#include "scene/scene.hpp"
#include "scene/time.hpp"
#include "model-loader/model-loader.hpp"
#include "entity/entity.hpp"
#include "input/window.hpp"
#include "input/input.hpp"
#include "registry/registry.hpp"
#include "stream/build-creator.hpp"

#ifndef TWE_BUILD
#include "gui/gui.hpp"
#else
#include "ui-build/ui-build.hpp"
#endif

namespace TWE {
    class Engine {
    public:
        Engine(int wndWidth, int wndHeight, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        void start(const std::string& buildFilePath = "");
    protected:
        void updateInput();
        static std::unique_ptr<Window> window;
        static std::unique_ptr<ProjectData> projectData;
        static std::shared_ptr<IScene> curScene;
        static std::shared_ptr<DebugCamera> debugCamera;
        static Registry<DLLLoadData> scriptDLLRegistry;
        static Registry<MeshSpecification> meshRegistry;
        static Registry<MeshRendererSpecification> meshRendererRegistry;
        static Registry<TextureAttachmentSpecification> textureRegistry;
    private:
        void updateTitle();
        void render();
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void windowCloseCallback(GLFWwindow* window);
        #ifndef TWE_BUILD
        static std::unique_ptr<GUI> gui;
        #else
        void loadBuild(const std::string& buildFilePath);
        static std::unique_ptr<UIBuild> uiBuild;
        #endif
    };
}

#endif