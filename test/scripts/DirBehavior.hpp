#pragma once

#include "entity/behavior.hpp"
#include "scene/time.hpp"
#include "scene/components/components.hpp"

using namespace TWE;

class DirBehavior : public Behavior {
private:
    TransformComponent* transform = nullptr;
public:
    void start() override {
        transform = &getComponent<TransformComponent>();
    }

    void update() override {
        transform->rotate(10.f * Time::deltaTime, {1.f, 0.f, 0.f});
        transform->rotate(5.f * Time::deltaTime, {0.f, 1.f, 0.f});
        transform->setPosition({-transform->getForward().x * 300.f, -transform->getForward().y * 300.f, -transform->getForward().z * 300.f});
    }
};