#include "shape.hpp"

GLfloat Shape::cubeVertices[] = {
    //front
    -0.5f, -0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      0.f, 1.f, //left down     0
    0.5f,  -0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      1.f, 1.f, //right down    1
    0.5f,   0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      1.f, 0.f, //right up      2
    -0.5f,  0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      0.f, 0.f, //left up       3
    //back
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     1.f, 1.f, //right down    4
    0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     0.f, 1.f, //left down     5
    0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     0.f, 0.f, //left up       6
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     1.f, 0.f, //right up      7
    //left
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.f, 1.f, //left down     8
    -0.5f, -0.5f, 0.5f,   -1.0f,  0.0f,  0.0f,    1.f, 1.f, //right down    9
    -0.5f,  0.5f, 0.5f,   -1.0f,  0.0f,  0.0f,    1.f, 0.f, //right up      10
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.f, 0.f, //left up       11
    //right
    0.5f,  -0.5f, 0.5f,   1.0f,  0.0f,  0.0f,     0.f, 1.f, //left down     12
    0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,     1.f, 1.f, //right down    13
    0.5f,   0.5f, -0.5f,  1.0f,  0.0f,  0.0f,     1.f, 0.f, //right up      14
    0.5f,   0.5f, 0.5f,   1.0f,  0.0f,  0.0f,     0.f, 0.f, //left up       15
    //top
    -0.5f,  0.5f, 0.5f,   0.0f,  1.0f,  0.0f,     0.f, 1.f, //left down     16
    0.5f,   0.5f, 0.5f,   0.0f,  1.0f,  0.0f,     1.f, 1.f, //right down    17
    0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,     1.f, 0.f, //right up      18
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,     0.f, 0.f, //left up       19
    //bottom
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,     0.f, 1.f, //left down     20
    0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,     1.f, 1.f, //right down    21
    0.5f,  -0.5f, 0.5f,   0.0f, -1.0f,  0.0f,     1.f, 0.f, //right up      22
    -0.5f, -0.5f, 0.5f,   0.0f, -1.0f,  0.0f,     0.f, 0.f, //left up       23
};
GLuint Shape::cubeIndices[] = {
    //front
    0, 1, 2,
    0, 3, 2,
    //back
    4, 5, 6,
    4, 7, 6,
    //left
    8, 9, 10,
    8, 10, 11,
    //right
    12, 13, 14,
    12, 15, 14,
    //top
    16, 17, 18,
    16, 19, 18,
    //bottom
    20, 21, 22,
    20, 23, 22
};
GLfloat Shape::plateVertices[] = {
    -1.f, -0.5f,  1.f,    0.0f,  1.0f,  0.0f,     0.f, 1.f,
    1.f,  -0.5f,  1.f,    0.0f,  1.0f,  0.0f,     1.f, 1.f,
    1.f,  -0.5f, -1.f,    0.0f,  1.0f,  0.0f,     1.f, 0.f,
    -1.f, -0.5f, -1.f,    0.0f,  1.0f,  0.0f,     0.f, 0.f,
};
GLuint Shape::plateIndices[] = {
    0, 1, 2,
    0, 3, 2
};

Object* Shape::createCube(const std::vector<std::string>& texPaths) {
    Object* cube;
    if(!texPaths.size())
        cube = new Object(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices));
    else
        cube = new Object(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices), texPaths);
    cube->setName("Cube");
    return cube;
}

Object* Shape::createPlate(const std::vector<std::string>& texPaths) {
    Object* plate;
    if(!texPaths.size())
        plate = new Object(Shape::plateVertices, sizeof(Shape::plateVertices), Shape::plateIndices, sizeof(Shape::plateIndices));
    else
        plate = new Object(Shape::plateVertices, sizeof(Shape::plateVertices), Shape::plateIndices, sizeof(Shape::plateIndices), texPaths);
    plate->setName("Plate");
    return plate;
}

Light* Shape::createSpotLight(GLfloat innerRadius, GLfloat outerRadius, GLfloat constant, GLfloat linear, GLfloat quadratic) {
    Light* light = new Light(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices), 
                     innerRadius, outerRadius, constant, linear, quadratic, "spot");
    light->scale({0.5f, 0.5f, 0.5f});
    light->setName("SpotLight");
    light->getShader().setUniform("type.spot", true);
    return light;
}

Light* Shape::createPointLight(GLfloat constant, GLfloat linear, GLfloat quadratic) {
    Light* light = new Light(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices), 
                     -1.f, -1.f, constant, linear, quadratic, "point");
    light->scale({0.5f, 0.5f, 0.5f});
    light->setName("PointLight");
    light->getShader().setUniform("type.point", true);
    return light;
}

Light* Shape::createDirLight() {
    Light* light = new Light(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices), 
                     -1.f, -1.f, -1.f, -1.f, -1.f, "dir");
    light->scale({0.5f, 0.5f, 0.5f});
    light->setName("DirLight");
    light->getShader().setUniform("type.dir", true);
    return light;
}