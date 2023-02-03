#ifndef SCENE_HPP
#define SCENE_HPP

#include "renderer/renderer.hpp"
#include "renderer/debug-camera.hpp"
#include "renderer/camera.hpp"

#include "stream/dll-creator.hpp"
#include "stream/project-creator.hpp"

#include "scene/scene-camera-specification.hpp"
#include "scene/scene-bullet-debug-drawer.hpp"
#include "scene/scene-physics.hpp"
#include "scene/scene-scripts.hpp"
#include "scene/scene-audio.hpp"
#include "scene/physics-raycast.hpp"
#include "scene/shape.hpp"
#include "scene/scene-serializer.hpp"

#include "time.hpp"
#include "input/input.hpp"
#include "registry/registry.hpp"
#include "undo-redo/ur-control.hpp"

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

    class Scene {
    public:
        Scene(uint32_t windowWidth, uint32_t windowHeight, const std::filesystem::path& rootPath);
        void update();
        void reset();
        void setDebugCamera(DebugCamera* debugCamera);
        void setName(const std::string& name);
        void setScriptDLLRegistry(Registry<DLLLoadData>* scriptDLLRegistry);
        void setProjectData(ProjectData* projectData);
        void setState(SceneState state);
        void cleanEntity(Entity& entity);
        Entity createEntity(const std::string& name = "Entity");
        Entity copyEntityState(Entity& entity, SceneStateSpecification& to);
        [[nodiscard]] bool& getIsFocusedOnDebugCamera();
        [[nodiscard]] entt::registry* getRegistry() const noexcept;
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept;
        [[nodiscard]] std::string getName() const noexcept;
        [[nodiscard]] FBO* getFrameBuffer() const noexcept;
        [[nodiscard]] Registry<DLLLoadData>* getScriptDLLRegistry() const noexcept;
        [[nodiscard]] int getLightsCount() const noexcept;
        [[nodiscard]] SceneStateSpecification* getSceneStateSpecification();
        [[nodiscard]] SceneAudio* getSceneAudio();
        [[nodiscard]] SceneScripts* getSceneScripts();
        [[nodiscard]] ProjectData* getProjectData();
        [[nodiscard]] SceneCameraSpecification* getSceneCamera();
        [[nodiscard]] SceneState getSceneState();
    private:
        void updateEditState();
        void updateTransforms();
        void updateChildsTransform(Entity& entity, ModelSpecification& ratioTransform, const glm::vec3& centerPositon);
        void updateRunState();
        void updateLight();
        bool updateView();
        void resetEntityRegistry(entt::registry* registry);
        void updateShadows(uint32_t windowWidth, uint32_t windowHeight);
        void setShadows(const LightComponent& lightComponent, const glm::mat4& lightSpaceMat, int index);
        void setLight(const LightComponent& light, TransformComponent& transform, const  uint32_t index);
        void copySceneState(SceneStateSpecification& from, SceneStateSpecification& to);

        SceneState _sceneState;
        SceneAudio _sceneAudio;
        SceneScripts* _sceneScripts;
        SceneCameraSpecification _sceneCamera;
        SceneRegistrySpecification _sceneRegistry;

        std::unique_ptr<FBO> _frameBuffer;
        DebugCamera* _debugCamera;
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
        friend class SceneScripts;
    };
}

#endif