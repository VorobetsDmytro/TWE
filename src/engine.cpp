#include "engine.hpp"

namespace TWE {
    std::shared_ptr<DebugCamera> Engine::debugCamera = std::make_shared<DebugCamera>(glm::vec3(0.f, 0.f, 0.f), 0.1f);
    std::shared_ptr<Scene> Engine::curScene;
    Registry<DLLLoadData> Engine::scriptDLLRegistry;
    Registry<MeshSpecification> Engine::meshRegistry;
    Registry<MeshRendererSpecification> Engine::meshRendererRegistry;
    Registry<TextureAttachmentSpecification> Engine::textureRegistry;
    std::unique_ptr<ProjectData> Engine::projectData;
    #ifndef TWE_BUILD
    std::unique_ptr<GUI> Engine::gui;
    #else
    std::unique_ptr<UIBuild> Engine::uiBuild;
    #endif

    Engine::Engine(int wndWidth, int wndHeight, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
        window = std::make_unique<Window>();
        window->initGLFW(wndWidth, wndHeight, title, monitor, share);
        window->setKeyCallback(&Engine::keyCallback);
        window->setMouseButtonCallback(&Engine::mouseButtonCallback);
        window->setCursorPosCallback(&Engine::mouseCallback);
        window->setFramebufferSizeCallback(&Engine::framebufferSizeCallback);
        window->initGLAD(wndWidth, wndHeight);
        std::filesystem::path rootPath;
        //imgui
        #ifndef TWE_BUILD
        gui = std::make_unique<GUI>(window->getSource());
        rootPath = "../../";
        #else
        uiBuild = std::make_unique<UIBuild>(window->getSource());
        rootPath = "./";
        #endif
        //initialization vars
        projectData = std::make_unique<ProjectData>();
        debugCamera->setPerspective(90.f, wndWidth, wndHeight);
        curScene = std::make_shared<Scene>(wndWidth, wndHeight, rootPath);
        curScene->setDebugCamera(debugCamera.get());
        curScene->setScriptDLLRegistry(&scriptDLLRegistry);
        curScene->setProjectData(projectData.get());
        #ifndef TWE_BUILD
        gui->setScene(curScene.get());
        gui->setProjectData(projectData.get());
        #endif
        Shape::initialize(&meshRegistry, &meshRendererRegistry, &textureRegistry, rootPath);
        Input::setWindow(window->getSource());
        Renderer::init();
        setVSync(false);
    }

    void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        Input::keyCallback(window, key, scancode, action, mode);
    }

    void Engine::updateInput(){
        if(Input::getCloseApplication())
            glfwSetWindowShouldClose(window->getSource(), GLFW_TRUE);
        static bool preShowCursor = Input::getShowCursor();
        bool showCursor = Input::getShowCursor();
        if(preShowCursor != showCursor) {
            glfwSetInputMode(window->getSource(), GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            preShowCursor = showCursor;
        }
        #ifndef TWE_BUILD
        if(Input::isKeyPressed(Keyboard::KEY_ESCAPE) && !curScene->getIsFocusedOnDebugCamera()) {
            glfwSetInputMode(window->getSource(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Input::setShowCursor(true);
            preShowCursor = true;
        }
        if(!Input::isMouseButtonPressed(Mouse::MOUSE_BUTTON_RIGHT) || !curScene->getIsFocusedOnDebugCamera())
            return;
        debugCamera->keyInput(Time::getDeltaTime());
        #endif
    }

    void Engine::setVSync(GLboolean isOn) {
        window->setVSync(isOn);
    }

    void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        Input::mouseButtonCallback(window, button, action, mods);
        #ifndef TWE_BUILD
        if(button == GLFW_MOUSE_BUTTON_RIGHT && !gui->getIsMouseDisabled()) {
            bool inputModeDissabled = action == GLFW_PRESS && gui->getIsFocusedOnViewport() && gui->getIsMouseOnViewport() && curScene->getIsFocusedOnDebugCamera();
            Input::setShowCursor(!inputModeDissabled);
        } else if(button != GLFW_MOUSE_BUTTON_LEFT && curScene->getIsFocusedOnDebugCamera())
            Input::setShowCursor(true);
        #endif
    }

    void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        #ifndef TWE_BUILD
        static bool debugCameraFlag = false;
        Input::setIsMouseMoving(true);
        if(!gui->getIsFocusedOnViewport())
            return;
        Input::mouseCallback(window, xpos, ypos);
        if(Input::isMouseButtonPressed(Mouse::MOUSE_BUTTON_RIGHT) && gui->getIsFocusedOnViewport() && curScene->getIsFocusedOnDebugCamera()) {
            if(debugCameraFlag) {
                auto mouseOffset = Input::getMouseOffset();
                debugCamera->mouseInput(mouseOffset[0], mouseOffset[1]);
            }
            debugCameraFlag = true;
            Input::setShowCursor(!gui->getIsFocusedOnViewport());
        } else {
            debugCameraFlag = false;
            if(curScene->getSceneState() != SceneState::Run)
                Input::setShowCursor(true);
        }
        #else
        Input::mouseCallback(window, xpos, ypos);
        #endif
    }

    void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Renderer::setViewport(0, 0, width, height);
        curScene->getFrameBuffer()->resize(width, height);
    }

    void Engine::updateTitle() {
        static std::string title;
        static std::string preTitle;
        title = window->getTitle() + " - " + projectData->projectName + ": " + curScene->getName();
        if(std::strcmp(preTitle.c_str(), title.c_str())) {
            glfwSetWindowTitle(window->getSource(), title.c_str());
            preTitle = title;
        }
    }

    void Engine::start(const std::string& buildFilePath){
        #ifndef TWE_BUILD
        gui->addCheckbox("Debug camera focus", curScene->getIsFocusedOnDebugCamera());
        #else
        loadBuild(buildFilePath);
        #endif
        while(!glfwWindowShouldClose(window->getSource())){
            Renderer::cleanScreen({0.25f, 0.25f, 0.25f, 0.f});
            glfwPollEvents();
            updateTitle();
            updateInput();
            render();
            glfwSwapBuffers(window->getSource());
            Time::calculate();
            Input::flush();
        }
    }

    void Engine::render() {
        auto camera = curScene->getSceneCamera();
        #ifndef TWE_BUILD
        gui->begin();
        curScene->update();
        auto frameBuffer = curScene->getFrameBuffer();
        frameBuffer->bind();
        Renderer::cleanScreen({});
        if(!camera->camera) {
            frameBuffer->unbind();
            gui->end();
            return;
        }
        Renderer::renderScene(curScene.get());
        frameBuffer->unbind();
        gui->end();
        #else
        uiBuild->begin();
        curScene->update();
        if(!camera->camera) {
            uiBuild->end();
            return;
        }
        Renderer::renderScene(curScene.get());
        uiBuild->end();
        #endif
    }

    #ifdef TWE_BUILD
    void Engine::loadBuild(const std::string& buildFilePath) {
        auto buildData = BuildCreator::load(buildFilePath);
        if(buildData) {
            std::filesystem::path projectFilePath = "./" + buildData->projectFilePath.string();
            auto projectData = ProjectCreator::load(projectFilePath.string(), &scriptDLLRegistry);
            if(projectData) {
                *this->projectData.get() = *projectData;
                std::filesystem::path startScenePath = "./" + buildData->startScenePath.string();
                SceneSerializer::deserialize(curScene.get(), startScenePath.string());
                curScene->getIsFocusedOnDebugCamera() = false;
                curScene->getSceneAudio()->startAudioOnRun(curScene->getRegistry());
            }
        }
    }
    #endif
}