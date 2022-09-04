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
#include <algorithm>

#include "math.h"
#include "mesh.hpp"
#include "shader.hpp"
#include "transform.hpp"

class Object {
public:
    Object() = default;
    Object(const Object& obj);
    Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, 
           ShaderIndices vertShader = ShaderIndices::DEFAULT_VERT, 
           ShaderIndices fragShader = ShaderIndices::DEFAULT_FRAG);
    Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<std::string>& texPaths,
           ShaderIndices vertShader = ShaderIndices::DEFAULT_VERT, 
           ShaderIndices fragShader = ShaderIndices::TEXTURE_FRAG);
    Object(const std::vector<Mesh>& meshes,
           ShaderIndices vertShader = ShaderIndices::DEFAULT_VERT, 
           ShaderIndices fragShader = ShaderIndices::DEFAULT_FRAG);
    virtual void draw();
    void rotate(float angle, const glm::vec3& axis);
    void move(const glm::vec3& pos);
    void scale(const glm::vec3& size);
    void setTransMat(const glm::mat4& transform, TransformMatrixOptions option);
    void setViewPos(const glm::vec3& pos);
    void setName(const char* name);
    void setTexture(const char* texPath, GLuint texNum);
    [[nodiscard]] Shader& getShader() const noexcept; 
    [[nodiscard]] uint32_t getId() const noexcept;
    [[nodiscard]] glm::mat4 getModelMat();
    [[nodiscard]] Transform& getTransform() const noexcept;
    [[nodiscard]] glm::vec3 getForward() const noexcept;
    [[nodiscard]] glm::vec3 getRight() const noexcept;
    [[nodiscard]] glm::vec3 getUp() const noexcept;
    [[nodiscard]] std::string getName() const noexcept;
protected:
    virtual void create(ShaderIndices vertShader, ShaderIndices fragShader);
    [[nodiscard]] uint32_t generateId();
    uint32_t id;
    std::string name;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Transform> transform;
    ShaderIndices vertShader;
    ShaderIndices fragShader;
};

#endif