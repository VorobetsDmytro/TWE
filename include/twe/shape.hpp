#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <string>

#include "object.hpp"
#include "light.hpp"

class Shape{
public:
    Shape() = default;
    static Object* createCube(const std::vector<std::string>& texPaths = {});
    static Object* createPlate(const std::vector<std::string>& texPaths = {});
    static Object* createCubemap(const std::vector<std::string>& texPaths);
    static Light* createSpotLight(GLfloat innerRadius = 15.f, GLfloat outerRadius = 20.f, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f);
    static Light* createPointLight(GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f);
    static Light* createDirLight();
private:
    static GLuint loadCubemapTexture(const std::vector<std::string>& texPaths);
    static GLfloat cubeVertices[];
    static GLuint cubeIndices[];
    static GLfloat plateVertices[];
    static GLuint plateIndices[];
    static GLuint cubemapIndices[];
};

#endif