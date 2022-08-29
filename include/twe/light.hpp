#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "object.hpp"

class Light: public Object {
public:
    Light(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, GLfloat innerRadius = 15.f, GLfloat outerRadius = 25.f, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f, const char* type = "dir");
    Light(const Light& light);
    void setColor(const glm::vec3& color);
    GLfloat innerRadius;
    GLfloat outerRadius;
    GLfloat constant;
    GLfloat linear; 
    GLfloat quadratic;
    std::string type;
};

#endif