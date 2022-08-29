#include "test.hpp"

Test::Test(int wndWidth, int wndHeight, const char* title, GLFWmonitor *monitor, GLFWwindow *share)
:Engine(wndWidth, wndHeight, title, monitor, share){
    Object* obj = Shape::createCube("../../src/textures/box.png");
    obj->move({0.f, 0.01f, 3.f});
    obj->setName("box");
    Object* obj2 = Shape::createCube("../../src/textures/ok.png");
    obj2->move(glm::vec3(3.5f, 1.f, -3.f));
    obj2->setName("ok");
    Object* obj3 = Shape::createCube();
    obj3->move(glm::vec3(-1.f, 1.f, 0.f));
    obj3->setName("cube");
    obj3->scale({0.5f, 0.5f, 0.5f});
    Object* floor = Shape::createPlate();
    floor->setName("floor");
    floor->scale({5.f, 1.f, 5.f});

    curScene->addObject(obj);
    curScene->addObject(obj2);
    curScene->addObject(obj3);
    curScene->addObject(floor);

    Light* spotLight = Shape::createSpotLight();
    spotLight->move({13.f, 4.f, -9.f});
    spotLight->rotate(45.f, {0.f, -1.f, 0.f});
    spotLight->rotate(15.f, {1.f, 0.f, 0.f});
    spotLight->setColor({0.8f, 0.25f, 0.25f});
    curScene->addObject(spotLight);
    Light* spotLight2 = Shape::createSpotLight();
    spotLight2->move({8.f, 4.f, -9.f});
    spotLight2->rotate(25.f, {0.f, -1.f, 0.f});
    spotLight2->rotate(15.f, {1.f, 0.f, 0.f});
    curScene->addObject(spotLight2);

    Light* pointLight= Shape::createPointLight();
    pointLight->move({-5.f, 0.5f, -2.f});
    pointLight->setColor({0.f, 1.f, 1.f});
    curScene->addObject(pointLight);

    Light* dirLight = Shape::createDirLight();
    dirLight->setName("dlight");
    dirLight->move({0.f, 10.f, 0.f});
    dirLight->rotate(180.f, {1.f, 0.f, 0.f});
    curScene->addObject(dirLight);

    camera.setPosition({0.f, 1.f, 5.f});
}

void Test::update() {
    auto obj2 = curScene->getObjectByName("ok");
    obj2->rotate(100.f * Time::deltaTime, {0.f, 1.f, 0.f});

    auto obj3 = curScene->getObjectByName("cube");
    obj3->rotate(100.f * Time::deltaTime, {0.f, 0.f, 1.f});
    obj3->getShader().setMaterial({{cos(glfwGetTime()) / 2 + 0.5f, sin(glfwGetTime()) / 2 + 0.5f, -cos(glfwGetTime()) / 2 + 0.5f}});

    auto dlight = curScene->getObjectByName("dlight");
    dlight->rotate(10.f * Time::deltaTime, {1.f, 0.f, 0.f});
    dlight->rotate(5.f * Time::deltaTime, {0.f, 1.f, 0.f});
}