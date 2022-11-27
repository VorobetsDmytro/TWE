#ifndef SCENE_SERIALIZER_HPP
#define SCENE_SERIALIZER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <single_include/nlohmann/json.hpp>

#include "scene/scene.hpp"
#include "scene/shape.hpp"
#include "stream/file.hpp"
#include "model-loader/model-loader.hpp"
#include "registry/registry.hpp"

namespace TWE {
    class SceneSerializer {
    public:
        static void serialize(Scene* scene, const std::string& path);
        static void deserialize(Scene* scene, const std::string& path, Registry<Behavior>& registry, std::function<void(Registry<Behavior>&)> registryLoader);
    private:
        static void serializeEntity(Entity& entity, nlohmann::json& jsonEntities);
        static void serializeCreationTypeComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeNameComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeTransformComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeMeshComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeMeshRendererComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeCameraComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeLightComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializePhysicsComponent(Entity& entity, nlohmann::json& jsonEntity);
        static void serializeScriptComponent(Entity& entity, nlohmann::json& jsonEntity);

        static void deserializeEntity(Scene* scene, Entity& entity, nlohmann::json& jsonComponents, Registry<Behavior>& registry, std::function<void(Registry<Behavior>&)> registryLoader);
        static void deserializeNameComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent);
        static void deserializeTransformComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent);
        static void deserializeMeshRendererComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent);
        static void deserializeCameraComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent);
        static void deserializeLightComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent);
        static void deserializePhysicsComponent(Scene* scene, Entity& entity, const std::string& key, nlohmann::json& jsonComponent);
        static void deserializeScriptComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent, Registry<Behavior>& registry, std::function<void(Registry<Behavior>&)> registryLoader);
        [[nodiscard]] static Entity deserializeCreationTypeComponent(Scene* scene, nlohmann::json& jsonComponent);

        [[nodiscard]] static std::string deleteInvertedCommas(const std::string& str);
    };
};

#endif