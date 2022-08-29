#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Transform{
public:
    Transform();
    Transform(const Transform& transform);
    glm::vec3 position;
    glm::vec3 rotate;
    glm::vec3 scale;
    glm::mat4 model;
};

#endif