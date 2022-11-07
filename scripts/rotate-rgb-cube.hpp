#pragma once

#include "entity/behavior.hpp"
#include "scene/time.hpp"
#include "scene/components/components.hpp"

using namespace TWE;

class RotateRGBCube : public Behavior {
private:
    TransformComponent* transform = nullptr;
    MeshRendererComponent* meshRendererComponent = nullptr;
public:
    void start() override {
        transform = &getComponent<TransformComponent>();
        meshRendererComponent = &getComponent<MeshRendererComponent>();
    }

    void update() override {
        transform->rotate(100.f * Time::deltaTime, {0.f, 0.f, 1.f});
        meshRendererComponent->material->objColor = {cos(glfwGetTime()) / 2 + 0.5f, sin(glfwGetTime()) / 2 + 0.5f, -cos(glfwGetTime()) / 2 + 0.5f};
    }
};