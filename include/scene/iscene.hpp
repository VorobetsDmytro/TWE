#ifndef ISCENE_HPP
#define ISCENE_HPP

#define NOMINMAX

#include "scene/scene-camera-specification.hpp"
#include "scene/scene-bullet-debug-drawer.hpp"
#include "scene/iscene-physics.hpp"
#include "scene/iscene-scripts.hpp"
#include "scene/iscene-audio.hpp"
#include "scene/physics-raycast.hpp"

#include "renderer/debug-camera.hpp"
#include "stream/project-creator.hpp"
#include "entity/entity.hpp"
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
        IScenePhysics* physics;
        URControl urControl;
        int lastId = 0;
    };

    struct SceneRegistrySpecification {
        SceneStateSpecification* current = nullptr;
        SceneStateSpecification edit;
        SceneStateSpecification run;
    };

    class IScene {
    public:
        IScene() = default;
        virtual void update() {}
        virtual void reset() {}
        virtual void setDebugCamera(DebugCamera* debugCamera) {}
        virtual void setName(const std::string& name) {}
        virtual void setScriptDLLRegistry(Registry<DLLLoadData>* scriptDLLRegistry) {}
        virtual void setProjectData(ProjectData* projectData) {}
        virtual void setState(SceneState state) {}
        virtual void cleanEntity(Entity& entity) {}
        virtual Entity createEntity(const std::string& name = "Entity") { return {}; }
        virtual Entity copyEntityState(Entity& entity, SceneStateSpecification& to) { return {}; }
        [[nodiscard]] virtual bool& getIsFocusedOnDebugCamera() { return tempBool; }
        [[nodiscard]] virtual entt::registry* getRegistry() const noexcept { return nullptr; }
        [[nodiscard]] virtual btDynamicsWorld* getDynamicWorld() const noexcept { return nullptr; }
        [[nodiscard]] virtual const std::string& getName() const noexcept { return tempStr; }
        [[nodiscard]] virtual Registry<DLLLoadData>* getScriptDLLRegistry() const noexcept { return nullptr; }
        [[nodiscard]] virtual int getLightsCount() const noexcept { return 0; }
        [[nodiscard]] virtual SceneStateSpecification* getSceneStateSpecification() { return nullptr; }
        [[nodiscard]] virtual ISceneAudio* getSceneAudio() { return nullptr; }
        [[nodiscard]] virtual ISceneScripts* getSceneScripts() { return nullptr; }
        [[nodiscard]] virtual IScenePhysics* getScenePhysics() { return nullptr; }
        [[nodiscard]] virtual ProjectData* getProjectData() { return nullptr; }
        [[nodiscard]] virtual SceneCameraSpecification* getSceneCamera() { return nullptr; }
        [[nodiscard]] virtual SceneRegistrySpecification* getSceneRegistry() { return nullptr; }
        [[nodiscard]] virtual SceneState getSceneState() { return SceneState::Edit; }
    private:
        bool tempBool;
        std::string tempStr;
    };
}

#endif