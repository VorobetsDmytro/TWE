#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <memory>
#include <string>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <entt/entt.hpp>

#include "renderer/debug-camera.hpp"
#include "renderer/camera.hpp"
#include "time.hpp"
#include "renderer/renderer.hpp"
#include "entity/entity.hpp"
#include "input/input.hpp"

namespace TWE {
    struct SceneCameraSpecification {
        Camera* camera = nullptr;
        glm::vec3 position = glm::vec3(0.f);
        glm::vec3 forward = glm::vec3(0.f);
        glm::vec3 up = glm::vec3(0.f);
    };

    class Scene {
    public:
        Scene(uint32_t windowWidth, uint32_t windowHeight);
        ~Scene();
        void update();
        void draw();
        void setTransMat(const glm::mat4& transform, TransformMatrixOptions option);
        void setLight(const LightComponent& light, const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const  uint32_t index);
        void setViewPos(const glm::vec3& pos);
        void updateView(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos);
        bool proccesKeyInput(GLFWwindow* window, int key, int scancode, int action, int mode);
        bool proccesMouseButtonInput(GLFWwindow* window, int button, int action, int mods);
        bool proccesMouseInput(GLFWwindow* window, double xpos, double ypos);
        void generateShadows(uint32_t windowWidth, uint32_t windowHeight);
        void updatePhysics();
        void linkRigidBody(const PhysicsComponent& physicsComponent);
        void setFocusOnDebugCamera(bool isFocusedOnDebugCamera);
        void setFocusOnViewport(bool isFocusedOnViewport);
        void setDrawLightMeshes(bool drawLightMeshes);
        void setDebugCamera(DebugCamera* debugCamera);
        void setName(const std::string& name);
        Entity createEntity(const std::string& name = "Entity");
        [[nodiscard]] bool& getIsFocusedOnDebugCamera();
        [[nodiscard]] bool getIsFocusedOnViewport() const noexcept;
        [[nodiscard]] bool getIsFocusedOnDebugCamera() const noexcept;
        [[nodiscard]] bool getDrawLightMeshes() const noexcept;
        [[nodiscard]] entt::registry* getRegistry() const noexcept;
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept;
        [[nodiscard]] std::string getName() const noexcept;
    private:
        bool updateView();
        void setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index);
        std::unique_ptr<btDynamicsWorld> _world;
        std::unique_ptr<btDispatcher> _dispatcher;
        std::unique_ptr<btConstraintSolver> _solver;
        std::unique_ptr<btCollisionConfiguration> _collisionConfig;
        std::unique_ptr<btBroadphaseInterface> _broadPhase;
        std::unique_ptr<entt::registry> _registry;
        std::unique_ptr<FBO> _frameBuffer;
        DebugCamera* _debugCamera;
        SceneCameraSpecification _sceneCameraSpecification;
        bool _isFocusedOnDebugCamera;
        bool _isFocusedOnViewport;
        bool _drawLightMeshes;
        std::string _name;
        friend class Entity;
        friend class GUI;
        friend class SceneSerializer;
    };
}

#endif