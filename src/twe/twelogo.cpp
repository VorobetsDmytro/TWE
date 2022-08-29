#include "twelogo.hpp"

GLfloat TWELogo::vertArr[] = {
    //pos                   //color             //textureCoord
    //BG
    -0.6f,  -0.5f, 0.0f,    1.0f, 0.f, 0.f,     0.f, 0.f,
    0.6f,   -0.5f, 0.0f,    0.0f, 1.f, 0.f,     0.f, 0.f,
    0.6f,    0.5f, 0.0f,    0.0f, 0.f, 1.f,     0.f, 0.f,
    -0.6f,   0.5f, 0.0f,    1.0f, 1.f, 1.f,     0.f, 0.f,
    //T
    -0.5f, 0.25f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    -0.25f,  0.25f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    -0.25f,  0.15f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    -0.5f, 0.15f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,

    -0.35f,  0.15f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    -0.35f,  -0.2f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    -0.4f, -0.2f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    -0.4f, 0.15f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    //W
    -0.18f, 0.25f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    -0.11f, 0.25f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    -0.03f,   -0.2f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
    -0.09f, -0.2f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,

    0.04f,  0.25f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    -0.02f, 0.25f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    -0.03f,   -0.2f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
    -0.0605f, -0.03f, -0.1f, 0.f, 0.f, 0.f,     0.f, 0.f,

    0.0088f, 0.05f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    0.04f, 0.25f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    0.115f,   -0.2f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
    0.05f, -0.2f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,

    0.20f,  0.25f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    0.13f, 0.25f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    0.115f,   -0.2f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
    0.0865f, -0.03f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
    //E
    0.28f,  0.25f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    0.335f,  0.25f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    0.335f,  -0.2f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    0.28f,  -0.2f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,

    0.5f,  0.25f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    0.5f,  0.175f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    0.335f,  0.175f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,

    0.5f,   -0.125f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
    0.5f,   -0.2f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    0.335f,  -0.125f, -0.1f, 0.f, 0.f, 0.f,     0.f, 0.f,
    
    0.335f, 0.062f, -0.1f,   0.f, 0.f, 0.f,     0.f, 0.f,
    0.5f, 0.062f, -0.1f,     0.f, 0.f, 0.f,     0.f, 0.f,
    0.5f, -0.012f, -0.1f,    0.f, 0.f, 0.f,     0.f, 0.f,
    0.335f, -0.012f, -0.1f,  0.f, 0.f, 0.f,     0.f, 0.f,
};
GLuint TWELogo::indices[] = {
    //BG
    0, 1, 2,
    0, 2, 3,
    //T
    4, 5, 6,
    4, 7, 6,
    8, 9, 10,
    8, 11, 10,
    //W
    12, 13, 14,
    12, 15, 14,
    16, 17, 18,
    18, 19, 17,
    20, 21, 22,
    20, 23, 22,
    24, 25, 26,
    26, 27, 25,
    //E
    28, 29, 30,
    28, 31, 30,
    29, 32, 33,
    29, 34, 33,
    30, 35, 36,
    35, 37, 30,
    38, 39, 40,
    38, 41, 40
};

TWELogo::TWELogo(): Object(TWELogo::vertArr, sizeof(TWELogo::vertArr), 
                           TWELogo::indices, sizeof(TWELogo::indices)) {}

void TWELogo::draw() {
    shader->use();
    vao->bind();
    GLfloat timeVal = glfwGetTime();
    GLfloat alphaVal = sin(timeVal) / 2 + 0.5f;
    GLint vertColorLocation = glGetUniformLocation(shader->getId(), "alpha");
    glUniform1f(vertColorLocation, alphaVal);
    glDrawElements(GL_TRIANGLES, indSize / sizeof(GLsizei), GL_UNSIGNED_INT, (GLvoid*)0);
    vao->unbind();
}