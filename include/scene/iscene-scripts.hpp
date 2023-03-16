#ifndef ISCENE_SCRITPS_HPP
#define ISCENE_SCRITPS_HPP

#include <filesystem>
#include "entity/entity.hpp"

#include "stream/dll-creator.hpp"

namespace TWE {
    class ISceneScripts {
    public:
        ISceneScripts() = default;
        virtual void reset(entt::registry* registry) {}
        virtual void update(entt::registry* registry, IScene* scene) {}
        virtual void bindScript(DLLLoadData* dllData, Entity& entity) {}
        virtual void bindScript(DLLLoadData* dllData, std::vector<Entity>& entities) {}
        virtual void validateScript(const std::string scriptName, IScene* scene) {}
        virtual void validateScripts(IScene* scene) {}
        virtual std::vector<Entity> unbindScript(entt::registry* registry, DLLLoadData* dllData, IScene* scene) { return {}; }
    };
}

#endif