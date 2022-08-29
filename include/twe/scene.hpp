#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <map>
#include <algorithm>
#include <iterator>

#include "object.hpp"
#include "light.hpp"
#include "camera.hpp"

class Scene {
public:
    Scene();
    void addObject(Object* obj);
    void removeObject(uint32_t id);
    [[nodiscard]] std::multimap<uint32_t, Object*>::iterator getObjectById(uint32_t id);
    [[nodiscard]] Object* getObjectByName(const char* name);
    void setTransMat(const glm::mat4& transform, TransformMatrixOptions option);
    void setLight(const Light* light, uint32_t index);
    void setViewPos(const glm::vec3& pos);
    void updateView(const Camera& cam, int wndWidth, int wndHeight);
    void draw();
private:
    std::multimap<uint32_t, Object*> _objects;
    GLint _lightsCount;
};

#endif