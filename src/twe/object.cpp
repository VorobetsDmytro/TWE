#include "object.hpp"

Object::Object(const Object& obj) {
    this->vertices = obj.vertices;
    this->vertSize = obj.vertSize;
    this->indices = obj.indices;
    this->indSize = obj.indSize;
    this->vao = obj.vao;
    this->vbo = obj.vbo;
    this->ebo = obj.ebo;
    this->shader = obj.shader;
    this->texture = obj.texture;
    this->transform = obj.transform;
    this->id = obj.id;
}

Object::Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const char* vertShaderPath, const char* fragShaderPath)
: vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
    create(vertShaderPath, fragShaderPath);
    texture = nullptr;
}

Object::Object(const char* texPath, GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, 
const char* vertShaderPath, const char* fragShaderPath)
: vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
    create(vertShaderPath, fragShaderPath);
    texture = std::make_shared<Texture>(texPath, 0);
    shader->setUniform("textureImg", 0);
}

void Object::create(const char* vertShaderPath, const char* fragShaderPath){
    vao = std::make_shared<VAO>();
    vao->bind();
    vbo = std::make_shared<VBO>(vertices, vertSize, GL_STATIC_DRAW);
    vbo->bind();
    ebo = std::make_shared<EBO>(indices, indSize, GL_STATIC_DRAW);
    ebo->bind();
    vao->setAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0, *vbo.get());
    vao->setAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)), *vbo.get());
    vao->setAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)), *vbo.get());
    vao->unbind();
    vbo->unbind();
    ebo->unbind();
    shader = std::make_shared<Shader>(vertShaderPath, fragShaderPath);
    id = generateId();
    transform = std::make_shared<Transform>();
    name = "Object";
}

void Object::draw() {
    shader->use();
    vao->bind();
    if(!texture)
        glDrawElements(GL_TRIANGLES, indSize / sizeof(GLsizei), GL_UNSIGNED_INT, (GLvoid*)0);
    else{
        texture->bind();
        glDrawElements(GL_TRIANGLES, indSize / sizeof(GLsizei), GL_UNSIGNED_INT, (GLvoid*)0);
        texture->unbind();
    }
    vao->unbind();
}

glm::mat4 Object::getModelMat() {
    return transform->model;
}

void Object::setTransMat(const glm::mat4& transform, TransformMatrixOptions option) {
    shader->use();
    GLuint locView = glGetUniformLocation(shader->getId(), TRANS_MAT_OPTIONS[option]);
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(transform));
}

void Object::move(const glm::vec3& pos) {
    transform->model = glm::translate(transform->model, pos);
    transform->position = transform->model[3];
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::rotate(float angle, const glm::vec3& axis) {
    transform->model = glm::rotate(transform->model, glm::radians<GLfloat>(angle), axis);
    const glm::vec3 right = getRight();
    const glm::vec3 up = getUp();
    const glm::vec3 forward	= getForward();
    glm::mat3 m(0.f);
    m[0][0] = right.x;    m[0][1] = right.y;    m[0][2] = right.z;
    m[1][0] = up.x;       m[1][1] = up.y;       m[1][2] = up.z;
    m[2][0] = forward.x;  m[2][1] = forward.y;  m[2][2] = forward.z;
    glm::vec3 rotate;
    rotate.x = atan2f( m[1][2], m[2][2]);
    rotate.y = atan2f(-m[0][2], sqrtf(m[1][2] * m[1][2] + m[2][2] * m[2][2]));
    rotate.z = atan2f( m[0][1], m[0][0]);
    transform->rotate = glm::degrees(rotate);
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::scale(const glm::vec3& size) {
    transform->model = glm::scale(transform->model, size);
    transform->scale.x = glm::length(glm::vec3(transform->model[0]));
    transform->scale.y = glm::length(glm::vec3(transform->model[1]));
    transform->scale.z = glm::length(glm::vec3(transform->model[2]));
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::setViewPos(const glm::vec3& pos) {
    shader->setUniform("viewPos", pos);
}

void Object::setName(const char* name) {
    this->name = name;
}

uint32_t Object::generateId() {
    return static_cast<uint32_t>(rand() % 1000001);
}

Shader& Object::getShader() const noexcept { return *shader.get(); }

uint32_t Object::getId() const noexcept { return id; }
std::string Object::getName() const noexcept { return name; }

Transform& Object::getTransform() const noexcept { return *transform.get(); };

glm::vec3 Object::getForward() const noexcept { return glm::normalize(glm::vec3(transform->model[2])); }
glm::vec3 Object::getRight() const noexcept { return glm::normalize(glm::vec3(transform->model[0])); }
glm::vec3 Object::getUp() const noexcept { return glm::normalize(glm::vec3(transform->model[1])); }