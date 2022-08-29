#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "math.h"
#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "transform.hpp"

class Object {
public:
    Object() = default;
    Object(const Object& obj);
    Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, 
           const char* vertShaderPath = "../../src/shaders/default.vert", 
           const char* fragShaderPath = "../../src/shaders/default.frag");
    Object(const char* texPath, GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize,
           const char* vertShaderPath = "../../src/shaders/default.vert", 
           const char* fragShaderPath = "../../src/shaders/texture.frag");
    virtual void draw();
    void rotate(float angle, const glm::vec3& axis);
    void move(const glm::vec3& pos);
    void scale(const glm::vec3& size);
    void setTransMat(const glm::mat4& transform, TransformMatrixOptions option);
    void setViewPos(const glm::vec3& pos);
    void setName(const char* name);
    [[nodiscard]] Shader& getShader() const noexcept; 
    [[nodiscard]] uint32_t getId() const noexcept;
    [[nodiscard]] glm::mat4 getModelMat();
    [[nodiscard]] Transform& getTransform() const noexcept;
    [[nodiscard]] glm::vec3 getForward() const noexcept;
    [[nodiscard]] glm::vec3 getRight() const noexcept;
    [[nodiscard]] glm::vec3 getUp() const noexcept;
    [[nodiscard]] std::string getName() const noexcept;
protected:
    virtual void create(const char* vertShaderPath, const char* fragShaderPath);
    [[nodiscard]] uint32_t generateId();
    uint32_t id;
    std::string name;
    GLfloat* vertices;
    GLsizei vertSize;
    GLuint* indices;
    GLsizei indSize;
    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<EBO> ebo;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Transform> transform;
};

#endif