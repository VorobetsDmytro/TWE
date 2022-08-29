#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "object.hpp"
#include "light.hpp"

class Shape{
public:
    Shape() = default;
    static Object* createCube(const char* texPath = nullptr);
    static Object* createPlate(const char* texPath = nullptr);
    static Light* createSpotLight(GLfloat innerRadius = 15.f, GLfloat outerRadius = 20.f, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f);
    static Light* createPointLight(GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f);
    static Light* createDirLight();
private:
    static GLfloat cubeVertices[];
    static GLuint cubeIndices[];
    static GLfloat plateVertices[];
    static GLuint plateIndices[];
};

#endif