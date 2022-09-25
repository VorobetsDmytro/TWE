#include "test.hpp"

Test::Test(int wndWidth, int wndHeight, const char* title, GLFWmonitor *monitor, GLFWwindow *share)
:Engine(wndWidth, wndHeight, title, monitor, share){
    Object* obj = Shape::createCube({"../../src/textures/box.png"});
    obj->move({0.f, 0.01f, 3.f});
    obj->setName("box");
    Object* obj2 = Shape::createCube({"../../src/textures/ok.png"});
    obj2->move({0.9f, 10.f, 0.9f});
    obj2->setName("ok");
    obj2->getPhysics().create(obj2->getTransform().scale, obj2->getTransform().position, 1.f, true);
    Object* obj3 = Shape::createCube();
    obj3->move({-1.5f, 1.f, 0.f});
    obj3->setName("cube");
    obj3->scale({0.5f, 0.5f, 0.5f});
    Object* obj4 = Shape::createCube({"../../src/textures/ok.png"});
    obj4->move({0.9f, 11.f, 0.9f});
    obj4->setName("ok");
    obj4->getPhysics().create(obj4->getTransform().scale, obj4->getTransform().position, 1.f, true);
    Object* floor = Shape::createPlate();
    floor->setName("floor");

    curScene->addObject(obj);
    curScene->addObject(obj2);
    curScene->addObject(obj3);
    curScene->addObject(obj4);
    curScene->addObject(floor);

    Light* spotLight = Shape::createSpotLight();
    spotLight->move({6.5f, 2.f, -4.5f});
    spotLight->rotate(45.f, {0.f, -1.f, 0.f});
    spotLight->rotate(15.f, {1.f, 0.f, 0.f});
    spotLight->setColor({0.8f, 0.25f, 0.25f});
    curScene->addObject(spotLight);
    Light* spotLight2 = Shape::createSpotLight();
    spotLight2->move({4.f, 2.f, -4.5f});
    spotLight2->rotate(25.f, {0.f, -1.f, 0.f});
    spotLight2->rotate(15.f, {1.f, 0.f, 0.f});
    curScene->addObject(spotLight2);

    Light* pointLight= Shape::createPointLight();
    pointLight->move({-2.5f, 0.25f, -1.f});
    pointLight->setColor({0.f, 1.f, 1.f});
    curScene->addObject(pointLight);

    Light* dirLight = Shape::createDirLight();
    dirLight->setName("dlight");
    dirLight->move({5.f, 5.f, 3.f});
    dirLight->rotate(90.f, {1.f, 0.f, 0.f});
    curScene->addObject(dirLight);

    ModelLoader mLoader;
    Object* model = mLoader.loadModel("../../src/models/tophat/TopHat.obj");
    if(model){
        model->scale({.2f, .2f, .2f});
        curScene->addObject(model);
    }

    Object* cubemap = Shape::createCubemap({
        "../../src/textures/cubemap/posx.jpg",
        "../../src/textures/cubemap/negx.jpg",
        "../../src/textures/cubemap/posy.jpg",
        "../../src/textures/cubemap/negy.jpg",
        "../../src/textures/cubemap/posz.jpg",
        "../../src/textures/cubemap/negz.jpg",
    });
    curScene->addObject(cubemap);

    camera.setPosition({0.f, 1.f, 5.f});
}

void Test::update() {
    auto obj3 = curScene->getObjectByName("cube");
    obj3->rotate(100.f * Time::deltaTime, {0.f, 0.f, 1.f});
    obj3->getShader().setMaterial({{cos(glfwGetTime()) / 2 + 0.5f, sin(glfwGetTime()) / 2 + 0.5f, -cos(glfwGetTime()) / 2 + 0.5f}});

    auto dlight = curScene->getObjectByName("dlight");
    dlight->rotate(10.f * Time::deltaTime, {1.f, 0.f, 0.f});
    dlight->rotate(5.f * Time::deltaTime, {0.f, 1.f, 0.f});
    dlight->setPos({-dlight->getForward().x * 20.f, -dlight->getForward().y * 20.f, -dlight->getForward().z * 20.f});
}