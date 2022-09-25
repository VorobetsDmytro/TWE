#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform{
public:
    Transform();
    Transform(const Transform& transform);
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::mat4 model;
};

#endif