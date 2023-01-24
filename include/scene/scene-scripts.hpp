#ifndef SCENE_SCRIPTS_HPP
#define SCENE_SCRIPTS_HPP

#include <filesystem>
#include <entt/entt.hpp>

#include "scene/components/components.hpp"
#include "scene/shape.hpp"

#include "scene/scene.hpp"

namespace TWE {
    class SceneScripts {
    public:
        SceneScripts() = default;
        SceneScripts(ProjectData* projectData);
        void reset(entt::registry* registry);
        void update(entt::registry* registry, Scene* scene);
        void bindScript(DLLLoadData* dllData, Entity& entity);
        void bindScript(DLLLoadData* dllData, std::vector<Entity>& entities);
        std::vector<Entity> unbindScript(entt::registry* registry, DLLLoadData* dllData, Scene* scene);
        void validateScript(const std::string scriptName, Scene* scene);
        void validateScripts(Scene* scene);
    private:
        ProjectData* _projectData;
    };
}

#endif