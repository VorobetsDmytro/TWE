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
    0, 2, 3,
    //back
    4, 6, 5,
    4, 7, 6,
    //left
    8, 9, 10,
    8, 10, 11,
    //right
    12, 13, 14,
    12, 14, 15,
    //top
    16, 17, 18,
    16, 18, 19,
    //bottom
    20, 21, 22,
    20, 22, 23
};
GLfloat Shape::plateVertices[] = {
    -5.f, -0.5f,  5.f,    0.0f,  1.0f,  0.0f,     0.f, 1.f,
    5.f,  -0.5f,  5.f,    0.0f,  1.0f,  0.0f,     1.f, 1.f,
    5.f,  -0.5f, -5.f,    0.0f,  1.0f,  0.0f,     1.f, 0.f,
    -5.f, -0.5f, -5.f,    0.0f,  1.0f,  0.0f,     0.f, 0.f,
};
GLuint Shape::plateIndices[] = {
    0, 1, 2,
    0, 2, 3
};
GLuint Shape::cubemapIndices[] = {
    //front
    0, 2, 1,
    0, 3, 2,
    //back
    4, 5, 6,
    4, 6, 7,
    //left
    8, 10, 9,
    8, 11, 10,
    //right
    12, 14, 13,
    12, 15, 14,
    //top
    16, 18, 17,
    16, 19, 18,
    //bottom
    20, 22, 21,
    20, 23, 22
};

Object* Shape::createCube(const std::vector<std::string>& texPaths) {
    Object* cube;
    if(!texPaths.size())
        cube = new Object(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices));
    else
        cube = new Object(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices), texPaths);
    cube->setName("Cube");
    cube->getPhysics().create(cube->getTransform().scale, cube->getTransform().position, 0.f, false);
    return cube;
}

Object* Shape::createPlate(const std::vector<std::string>& texPaths) {
    Object* plate;
    if(!texPaths.size())
        plate = new Object(Shape::plateVertices, sizeof(Shape::plateVertices), Shape::plateIndices, sizeof(Shape::plateIndices));
    else
        plate = new Object(Shape::plateVertices, sizeof(Shape::plateVertices), Shape::plateIndices, sizeof(Shape::plateIndices), texPaths);
    plate->setName("Plate");
    plate->getPhysics().create({10.f, 0.0000001f, 10.f}, {plate->getTransform().position.x, plate->getTransform().position.y - .5f, plate->getTransform().position.z}, 0.f, false);
    return plate;
}

Object* Shape::createCubemap(const std::vector<std::string>& texPaths) {
    GLuint texId = loadCubemapTexture(texPaths);
    if(!texId)
        return nullptr;
    Texture* texture = new Texture(texId, GL_TEXTURE_CUBE_MAP);
    Object* cubemap = new Object(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubemapIndices, sizeof(Shape::cubemapIndices), 
                                {texture}, ShaderIndices::CUBEMAP_VERT, ShaderIndices::CUBEMAP_FRAG);
    cubemap->setName("Cubemap");
    cubemap->scale({1000.f, 1000.f, 1000.f});
    return cubemap;
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

GLuint Shape::loadCubemapTexture(const std::vector<std::string>& texPaths) {
    if(texPaths.size() != 6) {
        std::cout << "Error loading a cubemap texture.\nTexture paths size has to be 6." << std::endl;
        return 0;
    }
    GLuint id;
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    int width, height, chanInFile;
    int i = 0;
    for(auto& texPath : texPaths){
        auto imgBytes = stbi_load(texPath.c_str(), &width, &height, &chanInFile, 4);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i++, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBytes);
        stbi_image_free(imgBytes);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return id;
}