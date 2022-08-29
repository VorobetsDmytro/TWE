#include "scene.hpp"

Scene::Scene() {
    _lightsCount = 0;
}

void Scene::addObject(Object* obj) {
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

void Scene::draw() {
    uint32_t lightIndex = 0;
    for(auto& obj : _objects){
        obj.second->draw();
        if(Light* light = dynamic_cast<Light*>(obj.second))
            setLight(light, lightIndex++);
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