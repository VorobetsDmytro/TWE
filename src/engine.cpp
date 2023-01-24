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
        initGLFW(wndWidth, wndHeight, title, monitor, share);
        initGLAD(wndWidth, wndHeight);
        std::filesystem::path rootPath;
        //imgui
        #ifndef TWE_BUILD
        gui = std::make_unique<GUI>(window);
        rootPath = "../../";
        #else
        uiBuild = std::make_unique<UIBuild>(window);
        rootPath = "./";
        #endif
        //initialization vars
        windowTitle = title;
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
        Input::setWindow(window);
        setVSync(false);
    }

    Engine::~Engine(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Engine::initGLFW(int& wndWidth, int& wndHeight, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef TWE_BUILD
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        wndWidth = mode->width;
        wndHeight = mode->height;
        #endif
        window = glfwCreateWindow(wndWidth, wndHeight, title, monitor, share);        
        if(!window){
            std::cout << "Error creating a window.\n";
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, &Engine::keyCallback);
        glfwSetMouseButtonCallback(window, &Engine::mouseButtonCallback);
        glfwSetCursorPos(window, static_cast<GLfloat>(wndWidth / 2), static_cast<GLfloat>(wndHeight / 2));
        glfwSetCursorPosCallback(window, &Engine::mouseCallback);
        glfwSetFramebufferSizeCallback(window, &Engine::framebufferSizeCallback);
    }

    void Engine::initGLAD(int wndWidth, int wndHeight) {
        gladLoadGL();
        glViewport(0, 0, wndWidth, wndHeight);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        Input::keyCallback(window, key, scancode, action, mode);
    }

    void Engine::updateInput(){
        static bool preShowCursor = Input::getShowCursor();
        bool showCursor = Input::getShowCursor();
        if(preShowCursor != showCursor) {
            glfwSetInputMode(window, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            preShowCursor = showCursor;
        }
        if(Input::getCloseApplication())
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        #ifndef TWE_BUILD
        if(Input::isKeyPressed(Keyboard::KEY_ESCAPE) && !curScene->getIsFocusedOnDebugCamera()) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Input::setShowCursor(true);
            preShowCursor = true;
        }
        if(!Input::isMouseButtonPressed(Mouse::MOUSE_BUTTON_RIGHT) || !curScene->getIsFocusedOnDebugCamera())
            return;
        debugCamera->keyInput(Time::getDeltaTime());
        #endif
    }

    void Engine::setVSync(GLboolean isOn) {
        vSync = isOn;
        glfwSwapInterval(vSync ? 1 : 0);
    }

    void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        Input::mouseButtonCallback(window, button, action, mods);
        #ifndef TWE_BUILD
        if(button == GLFW_MOUSE_BUTTON_RIGHT && !gui->getIsMouseDisabled()) {
            bool inputModeDissabled = action == GLFW_PRESS && gui->getIsFocusedOnViewport() && gui->getIsMouseOnViewport() && curScene->getIsFocusedOnDebugCamera();
            Input::setShowCursor(!inputModeDissabled);
        } else if(curScene->getSceneState() != SceneState::Run && (button != GLFW_MOUSE_BUTTON_LEFT))
            Input::setShowCursor(true);
        #endif
    }

    void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        static bool debugCameraFlag = false;
        #ifndef TWE_BUILD
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
        if(projectData->projectName.empty())
            return;
        title = windowTitle + " - " + projectData->projectName + ": " + curScene->getName();
        if(preTitle != title) {
            glfwSetWindowTitle(window, title.c_str());
            preTitle = title;
        }
    }

    void Engine::start(const std::string& buildFilePath){
        #ifndef TWE_BUILD
        gui->addCheckbox("Debug camera focus", curScene->getIsFocusedOnDebugCamera());
        #else
        loadBuild(buildFilePath);
        #endif
        while(!glfwWindowShouldClose(window)){
            Renderer::cleanScreen({0.25f, 0.25f, 0.25f, 0.f});
            glfwPollEvents();
            updateTitle();
            updateInput();
            #ifndef TWE_BUILD
            gui->begin();
            curScene->update();
            gui->end();
            #else
            uiBuild->begin();
            curScene->update();
            uiBuild->end();
            #endif
            glfwSwapBuffers(window);
            Time::calculate();
        }
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