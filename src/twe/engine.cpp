#include "engine.hpp"

bool Engine::pressedKeys[1024] = {};
Camera Engine::camera = Camera(glm::vec3(0.f, 0.f, 0.f), 90.f, 0.1f);
int Engine::wndWidth = 0.f;
int Engine::wndHeight = 0.f;

Engine::Engine(int wndWidth, int wndHeight, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    //glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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
    //glad
    gladLoadGL();
    glViewport(0, 0, wndWidth, wndHeight);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //imgui
    gui = std::make_unique<GUI>(window);
    //initialization vars
    srand(static_cast<unsigned>(time(0)));
    curScene = std::make_shared<Scene>();
    Engine::wndWidth = wndWidth;
    Engine::wndHeight = wndHeight;
    bFillLineMode = true;
    bPreFillLineMode = bFillLineMode;
    setVSync(true);
}

Engine::~Engine(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
        pressedKeys[key] = true;
    if(action == GLFW_RELEASE)
        pressedKeys[key] = false;
}

void Engine::keyInput(){
    if(!pressedKeys[GLFW_MOUSE_BUTTON_RIGHT])
        return;
    if(pressedKeys[GLFW_KEY_W])
        camera.keyInput(CamMovement::FORWARD, Time::deltaTime);
    if(pressedKeys[GLFW_KEY_S])
        camera.keyInput(CamMovement::BACKWARD, Time::deltaTime);
    if(pressedKeys[GLFW_KEY_D])
       camera.keyInput(CamMovement::RIGHT, Time::deltaTime);
    if(pressedKeys[GLFW_KEY_A])
       camera.keyInput(CamMovement::LEFT, Time::deltaTime);
    if(pressedKeys[GLFW_KEY_LEFT_SHIFT])
        camera.keyInput(CamMovement::L_SHIFT, Time::deltaTime);
}

void Engine::setVSync(GLboolean isOn) {
    vSync = isOn;
    glfwSwapInterval(vSync ? 1 : 0);
}

void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        glfwSetInputMode(window, GLFW_CURSOR, action == GLFW_PRESS ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        if(action == GLFW_RELEASE)
            glfwSetCursorPos(window, static_cast<GLfloat>(wndWidth / 2), static_cast<GLfloat>(wndHeight / 2));
    }
    if(action == GLFW_PRESS)
        pressedKeys[button] = true;
    if(action == GLFW_RELEASE)
        pressedKeys[button] = false;
}

void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static GLfloat preViewX = static_cast<GLfloat>(wndWidth / 2);
    static GLfloat preViewY = static_cast<GLfloat>(wndHeight / 2);
    GLfloat xOffset = xpos - preViewX;
    GLfloat yOffset = preViewY - ypos;
    preViewX = xpos;
    preViewY = ypos;
    if(!pressedKeys[GLFW_MOUSE_BUTTON_RIGHT])
        return;
    camera.mouseInput(xOffset, yOffset);
}

void Engine::drawMode() {
    if(bFillLineMode && bPreFillLineMode != bFillLineMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bPreFillLineMode = bFillLineMode;
    }
    else if(bPreFillLineMode != bFillLineMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bPreFillLineMode = bFillLineMode;
    }
}

void Engine::start(){
    gui->addCheckbox("Fill", bFillLineMode);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        update();
        curScene->updatePhysics();
        glClearColor(0.25f, 0.25f, 0.25f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        curScene->generateShadows(wndWidth, wndHeight);
        keyInput();
        drawMode();
        draw();
        gui->draw();
        glfwSwapBuffers(window);
        Time::calculateFPS();
    }
}

void Engine::draw() {
    curScene->updateView(camera, wndWidth, wndHeight);
    curScene->draw(true);
}

void Engine::update(){}