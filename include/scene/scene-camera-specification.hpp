#ifndef SCENE_CAMERA_SPECIFICATION_HPP
#define SCENE_CAMERA_SPECIFICATION_HPP

#include <glm/glm.hpp>

#include "renderer/camera.hpp"

namespace TWE {
    struct SceneCameraSpecification {
        Camera* camera = nullptr;
        glm::vec3 position = glm::vec3(0.f);
        glm::vec3 forward = glm::vec3(0.f);
        glm::vec3 up = glm::vec3(0.f);
        glm::mat4 projection = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projectionView = glm::mat4(1.f);
    };
}

#endif