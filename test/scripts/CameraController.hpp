#pragma once

#include "entity/behavior.hpp"
#include "scene/scene.hpp"
#include "scene/components/components.hpp"

using namespace TWE;

class CameraController : public Behavior {
private:
    TransformComponent* transform = nullptr;
    float speed = 5.f;
public:
    void start() override {
        transform = &getComponent<TransformComponent>();
    }

    void update(float deltaTime) override {
        move(deltaTime);
    }

    void move(float deltaTime) {
        if(Input::isKeyPressed(Keyboard::KEY_W))
            transform->move({0.f, 0.f, -speed * deltaTime});
        if(Input::isKeyPressed(Keyboard::KEY_S))
            transform->move({0.f, 0.f, speed * deltaTime});
        if(Input::isKeyPressed(Keyboard::KEY_D))
            transform->move({speed * deltaTime, 0.f, 0.f});
        if(Input::isKeyPressed(Keyboard::KEY_A))
            transform->move({-speed * deltaTime, 0.f, 0.f});
    }
};

extern "C" __declspec(dllexport) Behavior* createCameraController();