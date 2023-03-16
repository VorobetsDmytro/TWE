#ifndef SCENE_HPP
#define SCENE_HPP

#include "renderer/renderer.hpp"
#include "renderer/debug-camera.hpp"
#include "renderer/camera.hpp"

#include "stream/dll-creator.hpp"
#include "stream/project-creator.hpp"

#include "scene/iscene.hpp"
#include "scene/scene-physics.hpp"
#include "scene/scene-scripts.hpp"
#include "scene/scene-audio.hpp"

#include "time.hpp"
#include "input/input.hpp"
#include "registry/registry.hpp"
#include "undo-redo/ur-control.hpp"

namespace TWE {
    class Scene: public IScene {
    public:
        Scene();
        void update() override;
        void reset() override;
        void setDebugCamera(DebugCamera* debugCamera) override;
        void setName(const std::string& name) override;
        void setScriptDLLRegistry(Registry<DLLLoadData>* scriptDLLRegistry) override;
        void setProjectData(ProjectData* projectData) override;
        void setState(SceneState state) override;
        void cleanEntity(Entity& entity) override;
        Entity createEntity(const std::string& name = "Entity") override;
        Entity copyEntityState(Entity& entity, SceneStateSpecification& to) override;
        [[nodiscard]] bool& getIsFocusedOnDebugCamera() override;
        [[nodiscard]] entt::registry* getRegistry() const noexcept override;
        [[nodiscard]] btDynamicsWorld* getDynamicWorld() const noexcept override;
        [[nodiscard]] const std::string& getName() const noexcept override;
        [[nodiscard]] Registry<DLLLoadData>* getScriptDLLRegistry() const noexcept override;
        [[nodiscard]] int getLightsCount() const noexcept override;
        [[nodiscard]] SceneStateSpecification* getSceneStateSpecification() override;
        [[nodiscard]] ISceneAudio* getSceneAudio() override;
        [[nodiscard]] ISceneScripts* getSceneScripts() override;
        [[nodiscard]] IScenePhysics* getScenePhysics() override;
        [[nodiscard]] ProjectData* getProjectData() override;
        [[nodiscard]] SceneCameraSpecification* getSceneCamera() override;
        [[nodiscard]] SceneRegistrySpecification* getSceneRegistry() override;
        [[nodiscard]] SceneState getSceneState() override;
    private:
        void updateEditState();
        void updateTransforms();
        void updateChildsTransform(Entity& entity, ModelSpecification& ratioTransform, const glm::vec3& centerPositon);
        void updateRunState();
        void updateLight();
        bool updateView();
        void resetEntityRegistry(entt::registry* registry);
        void setLight(const LightComponent& light, TransformComponent& transform, const glm::mat4& lightSpaceMat, const uint32_t index);
        void setLight(const LightComponent& light, TransformComponent& transform, const uint32_t index);
        void copySceneState(SceneStateSpecification& from, SceneStateSpecification& to);

        SceneState _sceneState;
        SceneCameraSpecification _sceneCamera;
        SceneRegistrySpecification _sceneRegistry;
        std::unique_ptr<ISceneAudio> _sceneAudio;
        std::unique_ptr<ISceneScripts> _sceneScripts;

        DebugCamera* _debugCamera;
        bool _isFocusedOnDebugCamera;
        std::string _name;
        Registry<DLLLoadData>* _scriptDLLRegistry;
        ProjectData* _projectData;
    };
}

#endif