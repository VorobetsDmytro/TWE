#ifndef SCENE_HPP
#define SCENE_HPP

#include "renderer/renderer.hpp"
#include "renderer/debug-camera.hpp"
#include "renderer/camera.hpp"

#include "time.hpp"
#include "input/input.hpp"
#include "registry/registry.hpp"
#include "stream/dll-creator.hpp"
#include "stream/project-creator.hpp"
#include "undo-redo/ur-control.hpp"

#include "scene/scene-bullet-debug-drawer.hpp"
#include "scene/scene-physics.hpp"
#include "scene/scene-audio.hpp"
#include "scene/physics-raycast.hpp"
#include "scene/shape.hpp"
#include "scene/scene-serializer.hpp"

namespace TWE {
    enum class SceneState {
        Edit,
        Run,
        Pause,
        Unpause
    };

    struct SceneStateSpecification {
        entt::registry entityRegistry;
        ScenePhysics physics;
        URControl urControl;
        int lastId = 0;
    };

    struct SceneRegistrySpecification {
        SceneStateSpecification* current = nullptr;
        SceneStateSpecification edit;
        SceneStateSpecification run;
    };

    struct SceneCameraSpecification {
        Camera* camera = nullptr;
        glm::vec3 position = glm::vec3(0.f);
        glm::vec3 forward = glm::vec3(0.f);
        glm::vec3 up = glm::vec3(0.f);
        glm::mat4 projection = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projectionView = glm::mat4(1.f);
    };

    class Scene {
    public:
        Scene(uint32_t windowWidth, uint32_t windowHeight, const std::filesystem::path& rootPath);
        void update();
        void draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& projectionView, const glm::vec3& viewPos);
        void drawUI(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& projectionView, const glm::vec3& viewPos);
        void reset();
        void startAudioOnRun();
        void validateScripts();
        void validateScript(const std::string scriptName);
        void setDebugCamera(DebugCamera* debugCamera);
        void setName(const std::string& name);
        void setScriptDLLRegistry(Registry<DLLLoadData>* scriptDLLRegistry);
        void setProjectData(ProjectData* projectData);
        void setState(SceneState state);
        void cleanEntity(Entity& entity);
        void bindScript(DLLLoadData* dllData, Entity& entity);
        void bindScript(DLLLoadData* dllData, std::vector<Entity>& entities);
        std::vector<Entity> unbindScript(entt::registry* registry, DLLLoadData* dllData);
        Entity createEntity(const std::string& name = "Entity");
        Entity copyEntityState(Entity& entity, SceneStateSpecification& to);
        [[nodiscard]] bool& getIsFocusedOnDebugCamera();
        [[nodiscard]] entt::registry* getRegistry() const noexcept;
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept;
        [[nodiscard]] std::string getName() const noexcept;
        [[nodiscard]] FBO* getFrameBuffer() const noexcept;
        [[nodiscard]] Registry<DLLLoadData>* getScriptDLLRegistry() const noexcept;
        [[nodiscard]] SceneStateSpecification* getSceneStateSpecification();
        [[nodiscard]] SceneAudio* getSceneAudio();
        [[nodiscard]] ProjectData* getProjectData();
    private:
        void updateEditState();
        void updateTransforms();
        void updateChildsTransform(Entity& entity, ModelSpecification& ratioTransform, const glm::vec3& centerPositon);
        void updateRunState();
        void updatePhysics();
        void updateScripts();
        void updateLight();
        void updateAudioListenerPosition();
        bool updateView();
        void resetEntityRegistry(entt::registry* registry);
        void resetScripts(entt::registry* registry);
        void resetPhysics(entt::registry* registry);
        void updateShadows(uint32_t windowWidth, uint32_t windowHeight);
        void setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index);
        void setLight(const LightComponent& light, TransformComponent& transform, const  uint32_t index);
        void setAudioPauseState(bool paused);
        void copySceneState(SceneStateSpecification& from, SceneStateSpecification& to);

        SceneState _sceneState;
        SceneRegistrySpecification _sceneRegistry;
        SceneAudio _sceneAudio;
        std::unique_ptr<FBO> _frameBuffer;
        DebugCamera* _debugCamera;
        SceneCameraSpecification _sceneCamera;
        bool _isFocusedOnDebugCamera;
        std::string _name;
        Registry<DLLLoadData>* _scriptDLLRegistry;
        ProjectData* _projectData;

        friend class Entity;
        friend class Renderer;
        friend class GUI;
        friend class GUIComponentsPanel;
        friend class GUIDirectoryPanel;
        friend class GUIScenePanel;
        friend class SceneSerializer;
    };
}

#endif