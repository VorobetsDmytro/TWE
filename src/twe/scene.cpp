#include "scene.hpp"

Scene::Scene() {
    _lightsCount = 0;
    _collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    _dispatcher = std::make_unique<btCollisionDispatcher>(_collisionConfig.get());
    _broadPhase = std::make_unique<btDbvtBroadphase>();
    _solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    _world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfig.get());
    _world->setGravity({0.f, -9.81f, 0.f});
}

void Scene::addObject(Object* obj) {
    _world->addRigidBody(obj->getPhysics().getRigidBody());
    if(Light* light = dynamic_cast<Light*>(obj)){
        _objects.insert({0, light});
        ++_lightsCount;
        return;
    }
    _objects.insert({1, obj});
}

void Scene::removeObject(uint32_t id) {
    auto obj = getObjectById(id);
    if(obj == _objects.end())
        return;
    if(Light* light = dynamic_cast<Light*>(obj->second))
        --_lightsCount;
    _objects.erase(obj);
}

std::multimap<uint32_t, Object*>::iterator Scene::getObjectById(uint32_t id) {
    return std::find_if(_objects.begin(), _objects.end(), [&](std::pair<uint32_t, Object*>  objItem){
        return objItem.second->getId() == id;
    });
}

void Scene::setTransMat(const glm::mat4& transform, TransformMatrixOptions option) {
    for(auto& obj : _objects)
        obj.second->setTransMat(transform, option);
}

void Scene::setLight(const Light* light, uint32_t index) {
    std::string lightIndex = "lights[]";
    lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
    for(auto& obj : _objects){
        if(Light* lightItem = dynamic_cast<Light*>(obj.second))
            continue;
        Shader& shader = obj.second->getShader();
        shader.setUniform("lightCount", _lightsCount);
        shader.setUniform((lightIndex + ".pos").c_str(), light->getTransform().position);
        shader.setUniform((lightIndex + ".direction").c_str(), light->getForward());
        shader.setUniform((lightIndex + ".color").c_str(), light->getShader().getMaterial().objColor);
        shader.setUniform((lightIndex + ".cutOff").c_str(), glm::cos(glm::radians(light->innerRadius)));
        shader.setUniform((lightIndex + ".outerCutOff").c_str(), glm::cos(glm::radians(light->outerRadius)));
        shader.setUniform((lightIndex + ".fading.constant").c_str(), light->constant);
        shader.setUniform((lightIndex + ".fading.linear").c_str(), light->linear);
        shader.setUniform((lightIndex + ".fading.quadratic").c_str(), light->quadratic);
        if(light->type == "spot")
            shader.setUniform((lightIndex + ".type.spot").c_str(), true);
        else if(light->type == "point")
            shader.setUniform((lightIndex + ".type.point").c_str(), true);
        else
            shader.setUniform((lightIndex + ".type.dir").c_str(), true);
    }
}

void Scene::setViewPos(const glm::vec3& pos) {
    for(auto& obj : _objects)
        obj.second->setViewPos(pos);
}

void Scene::updateView(const Camera& cam, int wndWidth, int wndHeight) {
    glm::mat4 view = cam.getViewMat();
    glm::mat4 projection = cam.getProjectionMat(wndWidth, wndHeight);
    setTransMat(view, TransformMatrixOptions::VIEW);
    setTransMat(projection, TransformMatrixOptions::PROJECTION);
    setViewPos(cam.getPosition());
}

void Scene::updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos) {
    setTransMat(view, TransformMatrixOptions::VIEW);
    setTransMat(projection, TransformMatrixOptions::PROJECTION);
    setViewPos(pos);
}

void Scene::generateShadows(uint32_t windowWidth, uint32_t windowHeight) {
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.f);
    int lightIndex = -1;
    for(auto& obj : _objects) {
        ++lightIndex;
        if(Light* light = dynamic_cast<Light*>(obj.second)) {
            if(light->type != "dir")
                continue;
            glm::mat4 lightView = glm::lookAt(light->getTransform().position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
            setShadows(light, lightProjection * lightView, lightIndex);
            generateDepthMap(light, lightProjection, lightView);
        } else
            break;
    }
    glViewport(0, 0, windowWidth, windowHeight);
}

void Scene::setShadows(Light* light, const glm::mat4& lightSpaceMat, int index) {
    for(auto& obj : _objects){
        if(Light* light = dynamic_cast<Light*>(obj.second))
            continue;
        std::string lightIndex = "lights[]";
        lightIndex.insert(lightIndex.length() - 1, std::to_string(index));
        obj.second->getShader().setUniform((lightIndex + ".lightSpaceMat").c_str(), lightSpaceMat);
        obj.second->getShader().setUniform((lightIndex + ".shadowMap").c_str(), 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, light->getDepthTextureId());
    }
}

void Scene::generateDepthMap(Light* light, const glm::mat4& lightProjection, const glm::mat4& lightView) {
    auto depthMapSize = light->getDepthMapSize();
    FBO* fbo = light->getFBO();
    glViewport(0, 0, depthMapSize.first, depthMapSize.second);
    fbo->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    updateView(lightView, lightProjection, light->getTransform().position);
    glCullFace(GL_FRONT);
    draw(false);
    glCullFace(GL_BACK);
    fbo->unbind();
}

void Scene::draw(bool drawLightMeshes) {
    uint32_t lightIndex = 0;
    for(auto& obj : _objects){
        if(Light* light = dynamic_cast<Light*>(obj.second)){
            setLight(light, lightIndex++);
            if(!drawLightMeshes)
                continue;
        }
        obj.second->draw();
    }
}

Object* Scene::getObjectByName(const char* name) {
    auto obj = std::find_if(_objects.begin(), _objects.end(), [&](std::pair<uint32_t, Object*>  objItem){
        return objItem.second->getName() == name;
    });
    if(obj == _objects.end())
        return nullptr;
    return obj->second;
}

void Scene::updatePhysics() {
    for(auto& obj : _objects)
        obj.second->updatePhysics();
    _world->stepSimulation(Time::deltaTime);
}