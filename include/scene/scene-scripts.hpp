#ifndef SCENE_SCRIPTS_HPP
#define SCENE_SCRIPTS_HPP

#include "scene/iscene-scripts.hpp"
#include "scene/components/components.hpp"
#include "scene/shape.hpp"
#include "scene/scene-serializer.hpp"
#include "scene/time.hpp"

#include "stream/project-creator.hpp"

namespace TWE {
    class SceneScripts: public ISceneScripts {
    public:
        SceneScripts() = default;
        SceneScripts(ProjectData* projectData);
        void reset(entt::registry* registry) override;
        void update(entt::registry* registry, IScene* scene) override;
        void bindScript(DLLLoadData* dllData, Entity& entity) override;
        void bindScript(DLLLoadData* dllData, std::vector<Entity>& entities) override;
        void validateScript(const std::string scriptName, IScene* scene) override;
        void validateScripts(IScene* scene) override;
        std::vector<Entity> unbindScript(entt::registry* registry, DLLLoadData* dllData, IScene* scene) override;
    private:
        ProjectData* _projectData;
    };
}

#endif