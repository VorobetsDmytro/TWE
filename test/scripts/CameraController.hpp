#pragma once

#include "entity/behavior.hpp"
#include "scene/time.hpp"
#include "scene/components/components.hpp"
#include "input/input.hpp"

using namespace TWE;

class CameraController : public Behavior {
private:
    TransformComponent* transform = nullptr;
    CameraComponent* camera = nullptr;
    float speed = 5.f;
public:
    void start() override {
        transform = &getComponent<TransformComponent>();
        camera = &getComponent<CameraComponent>();
    }

    void update() override {
        move();
    }

    void move() {
        if(Input::isKeyPressed(Keyboard::KEY_W))
            transform->move({0.f, 0.f, -speed * Time::deltaTime});
        if(Input::isKeyPressed(Keyboard::KEY_S))
            transform->move({0.f, 0.f, speed * Time::deltaTime});
        if(Input::isKeyPressed(Keyboard::KEY_D))
            transform->move({speed * Time::deltaTime, 0.f, 0.f});
        if(Input::isKeyPressed(Keyboard::KEY_A))
            transform->move({-speed * Time::deltaTime, 0.f, 0.f});
    }
};