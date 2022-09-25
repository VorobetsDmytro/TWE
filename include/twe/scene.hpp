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
#include "time.hpp"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"

class Scene {
public:
    Scene();
    void addObject(Object* obj);
    void removeObject(uint32_t id);
    void setTransMat(const glm::mat4& transform, TransformMatrixOptions option);
    void setLight(const Light* light, uint32_t index);
    void setViewPos(const glm::vec3& pos);
    void updateView(const Camera& cam, int wndWidth, int wndHeight);
    void updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos);
    void draw(bool drawLightMeshes);
    void generateShadows(uint32_t windowWidth, uint32_t windowHeight);
    void updatePhysics();
    [[nodiscard]] std::multimap<uint32_t, Object*>::iterator getObjectById(uint32_t id);
    [[nodiscard]] Object* getObjectByName(const char* name);
private:
    void generateDepthMap(Light* light, const glm::mat4& lightProjection, const glm::mat4& lightView);
    void setShadows(Light* light, const glm::mat4& lightSpaceMat, int index);
    std::multimap<uint32_t, Object*> _objects;
    GLint _lightsCount;
    std::unique_ptr<btDynamicsWorld> _world;
    std::unique_ptr<btDispatcher> _dispatcher;
    std::unique_ptr<btConstraintSolver> _solver;
    std::unique_ptr<btCollisionConfiguration> _collisionConfig;
    std::unique_ptr<btBroadphaseInterface> _broadPhase;
};

#endif