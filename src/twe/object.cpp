#include "object.hpp"

Object::Object(const Object& obj) {
    this->meshes = obj.meshes;
    this->shader = obj.shader;
    this->transform = obj.transform;
    this->vertShader = obj.vertShader;
    this->fragShader = obj.fragShader;
    this->id = obj.id;
}

Object::Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, ShaderIndices vertShader, ShaderIndices fragShader)
:vertShader(vertShader), fragShader(fragShader) {
    create(vertShader, fragShader);
    meshes.push_back(std::make_shared<Mesh>(vertices, vertSize, indices, indSize));
}

Object::Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<std::string>& texPaths,
ShaderIndices vertShader, ShaderIndices fragShader)
:vertShader(vertShader), fragShader(fragShader) {
    create(vertShader, fragShader);
    meshes.push_back(std::make_shared<Mesh>(vertices, vertSize, indices, indSize, texPaths));
}

Object::Object(const std::vector<Mesh>& meshes, ShaderIndices vertShader, ShaderIndices fragShader)
:vertShader(vertShader), fragShader(fragShader) {
    create(vertShader, fragShader);
    for(auto& mesh: meshes)
        this->meshes.push_back(std::make_shared<Mesh>(mesh));
}

void Object::create(ShaderIndices vertShader, ShaderIndices fragShader){
    shader = std::make_shared<Shader>(SHADER_PATHS[vertShader], SHADER_PATHS[fragShader]);
    shader->setUniform("textureImg", 0);
    id = generateId();
    transform = std::make_shared<Transform>();
    name = "Object";
}

void Object::draw() {
    shader->use();
    for(auto& mesh : meshes)
        mesh->draw();
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
    transform->rotation = glm::degrees(rotate);
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

void Object::setTexture(const char* texPath, GLuint texNum) {
    shader = std::make_shared<Shader>(SHADER_PATHS[vertShader], SHADER_PATHS[ShaderIndices::TEXTURE_FRAG]);
    for(auto& mesh : meshes)
        mesh->setTexture(texPath, texNum);
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