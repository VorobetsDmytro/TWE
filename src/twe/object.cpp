#include "object.hpp"

Object::Object(const Object& obj) {
    this->meshes = obj.meshes;
    this->shader = obj.shader;
    this->transform = obj.transform;
    this->physics = obj.physics;
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

Object::Object(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<Texture*>& texs, 
ShaderIndices vertShader, ShaderIndices fragShader)
:vertShader(vertShader), fragShader(fragShader) {
    create(vertShader, fragShader);
    meshes.push_back(std::make_shared<Mesh>(vertices, vertSize, indices, indSize, texs));
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
    physics = std::make_shared<Physics>(transform->scale, transform->position, 0.f);
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
    transform->model = glm::translate(transform->model, pos / transform->scale);
    transform->position = transform->model[3];
    physics->setWorldTransform(transform->position);
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::rotate(float angle, const glm::vec3& axis) {
    transform->model = glm::rotate(transform->model, glm::radians<GLfloat>(angle), axis);
    transform->rotation = glm::angleAxis(angle, axis);
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::scale(const glm::vec3& size) {
    transform->model = glm::scale(transform->model, size);
    transform->scale.x = glm::length(transform->model[0]);
    transform->scale.y = glm::length(transform->model[1]);
    transform->scale.z = glm::length(transform->model[2]);
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

void Object::setPos(const glm::vec3& pos) {
    transform->model[3].x = pos.x;
    transform->model[3].y = pos.y;
    transform->model[3].z = pos.z;
    transform->position = transform->model[3];
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::setRotation(float angle, const glm::vec3& axis) {
    glm::mat4 translation = glm::translate(glm::mat4(1.f), transform->position);
    glm::mat4 rotation = glm::rotate(translation, glm::radians<GLfloat>(angle), axis);
    glm::mat4 scale = glm::scale(rotation, transform->scale);
    transform->model = scale;
    transform->rotation = glm::angleAxis(angle, axis);
    setTransMat(transform->model, TransformMatrixOptions::MODEL);
}

void Object::updatePhysics() {
    btRigidBody* rigidBody = physics->getRigidBody();
    rigidBody->activate();
    if(!physics->isDynamic())
        return;
    btTransform worldTransform;
    rigidBody->getMotionState()->getWorldTransform(worldTransform);
    btVector3 pos = worldTransform.getOrigin();
    btQuaternion quat = worldTransform.getRotation();
    float angle = quat.getAngle();
    btVector3 axis = quat.getAxis();
    setPos(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
    setRotation(glm::degrees(angle), {axis.getX(), axis.getY() ,axis.getZ()});
}

uint32_t Object::generateId() {
    return static_cast<uint32_t>(rand() % 1000001);
}

Shader& Object::getShader() const noexcept { return *shader.get(); }

uint32_t Object::getId() const noexcept { return id; }
std::string Object::getName() const noexcept { return name; }

Transform& Object::getTransform() const noexcept { return *transform.get(); }
Physics& Object::getPhysics() const noexcept { return *physics.get(); }

glm::vec3 Object::getForward() const noexcept { return glm::normalize(glm::vec3(transform->model[2])); }
glm::vec3 Object::getRight() const noexcept { return glm::normalize(glm::vec3(transform->model[0])); }
glm::vec3 Object::getUp() const noexcept { return glm::normalize(glm::vec3(transform->model[1])); }