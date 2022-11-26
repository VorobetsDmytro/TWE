#include "scene/scene-serializer.hpp"

namespace TWE {
    void SceneSerializer::serialize(Scene* scene, const std::string& path) {
        nlohmann::json jsonMain;
        jsonMain["Scene"] = scene->getName();
        nlohmann::json jsonEntities = nlohmann::json::array();
        scene->_registry->each([&](entt::entity entity) {
            Entity instance = { entity, scene };
            serializeEntity(instance, jsonEntities);
        });
        jsonMain["Entities"] = jsonEntities;
        File::save(path.c_str(), jsonMain.dump());
    }

    void SceneSerializer::deserialize(Scene* scene, const std::string& path, Registry<Behavior>& registry) {
        std::string jsonBodyStr = File::getBody(path.c_str());
        nlohmann::json jsonMain = nlohmann::json::parse(jsonBodyStr);
        auto& items = jsonMain.items();
        for(auto& [key, value] : items) {
            if(key == "Scene")
                scene->setName(value);
            else if(key == "Entities") {
                auto& entities = value.items();
                for(auto& [index, components] : entities) {        
                    Entity instance = deserializeCreationTypeComponent(scene, components);
                    deserializeEntity(scene, instance, components, registry);
                }
            }
        }
    }

    void SceneSerializer::serializeEntity(Entity& entity, nlohmann::json& jsonEntities) {
        nlohmann::json jsonEntity;
        jsonEntity["Entity ID"] = entity.getSource();
        serializeCreationTypeComponent(entity, jsonEntity);
        serializeNameComponent(entity, jsonEntity);
        serializeTransformComponent(entity, jsonEntity);
        serializeMeshComponent(entity, jsonEntity);
        serializeMeshRendererComponent(entity, jsonEntity);
        serializeCameraComponent(entity, jsonEntity);
        serializeLightComponent(entity, jsonEntity);
        serializePhysicsComponent(entity, jsonEntity);
        serializeScriptComponent(entity, jsonEntity);
        jsonEntities.push_back(jsonEntity);
    }

    void SceneSerializer::deserializeEntity(Scene* scene, Entity& entity, nlohmann::json& jsonComponents, Registry<Behavior>& registry) {
        auto& components = jsonComponents.items();
        for(auto& [key, value] : components) {
            deserializeNameComponent(entity, key, value);
            deserializeTransformComponent(entity, key, value);
            deserializeMeshRendererComponent(entity, key, value);
            deserializeCameraComponent(entity, key, value);
            deserializeLightComponent(entity, key, value);
            deserializePhysicsComponent(scene, entity, key, value);
            deserializeScriptComponent( entity, key, value, registry);
        }
    }

    void SceneSerializer::serializeCreationTypeComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<CreationTypeComponent>())
            return;
        auto& creationTypeComponent = entity.getComponent<CreationTypeComponent>();
        nlohmann::json jsonCreationTypeComponent;

        jsonCreationTypeComponent["Type"] = creationTypeComponent.getType();

        jsonEntity["CreationTypeComponent"] = jsonCreationTypeComponent;
    }

    Entity SceneSerializer::deserializeCreationTypeComponent(Scene* scene, nlohmann::json& jsonComponent) {
        auto creationTypeComponent = static_cast<EntityCreationType>(jsonComponent["CreationTypeComponent"]["Type"]);
        Entity entity;
        auto jsonMeshComponent = jsonComponent.find("MeshComponent");
        std::vector<std::string> textures;
        if(jsonMeshComponent != jsonComponent.end()) {
            nlohmann::json jsonTextures = jsonComponent["MeshComponent"]["Textures"];
            if(!jsonTextures.empty())
                textures = jsonTextures[0];
        }
        switch (creationTypeComponent) {
        case EntityCreationType::Cube:
            entity = Shape::createCubeEntity(scene, textures);
            break;
        case EntityCreationType::Plate:
            entity = Shape::createPlateEntity(scene, textures);
            break;
        case EntityCreationType::Cubemap:
            entity = Shape::createCubemapEntity(scene, textures);
            break;
        case EntityCreationType::SpotLight:
            entity = Shape::createSpotLightEntity(scene);
            break;
        case EntityCreationType::PointLight:
            entity = Shape::createPointLightEntity(scene);
            break;
        case EntityCreationType::DirLight:
            entity = Shape::createDirLightEntity(scene);
            break;
        case EntityCreationType::Camera:
            entity = Shape::createCameraEntity(scene);
            break;
        case EntityCreationType::Model:
            ModelLoader mLoader;
            ModelLoaderData* modelData = mLoader.loadModel(jsonComponent["MeshComponent"]["ModelPath"]);
            entity = Shape::createModelEntity(scene, modelData)[0];
            break;
        }
        return entity;
    }

    void SceneSerializer::serializeNameComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<NameComponent>())
            return;
        auto& nameComponent = entity.getComponent<NameComponent>();
        nlohmann::json jsonNameComponent;

        jsonNameComponent["Name"] = nameComponent.getName();
            
        jsonEntity["NameComponent"] = jsonNameComponent;
    }

    void SceneSerializer::deserializeNameComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent) {
        if(key != "NameComponent")
            return;
        std::string name = deleteInvertedCommas(jsonComponent["Name"]);
        if(!entity.hasComponent<NameComponent>())
            entity.addComponent<NameComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName(name);
    }

    void SceneSerializer::serializeTransformComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<TransformComponent>())
            return;
        auto& transformComponent = entity.getComponent<TransformComponent>();
        nlohmann::json jsonTransformComponent;

        nlohmann::json jsonPosition = nlohmann::json::array();
        jsonPosition.push_back(transformComponent.position.x);
        jsonPosition.push_back(transformComponent.position.y);
        jsonPosition.push_back(transformComponent.position.z);
        jsonTransformComponent["Position"] = jsonPosition;

        nlohmann::json jsonRotation = nlohmann::json::array();
        jsonRotation.push_back(transformComponent.rotation.x);
        jsonRotation.push_back(transformComponent.rotation.y);
        jsonRotation.push_back(transformComponent.rotation.z);
        jsonTransformComponent["Rotation"] = jsonRotation;

        nlohmann::json jsonSize = nlohmann::json::array();
        jsonSize.push_back(transformComponent.size.x);
        jsonSize.push_back(transformComponent.size.y);
        jsonSize.push_back(transformComponent.size.z);
        jsonTransformComponent["Size"] = jsonSize;

        jsonEntity["TransformComponent"] = jsonTransformComponent;
    }

    void SceneSerializer::deserializeTransformComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent) {
        if(key != "TransformComponent")
            return;
        if(!entity.hasComponent<TransformComponent>())
            entity.addComponent<TransformComponent>();
        auto& transformComponent = entity.getComponent<TransformComponent>();

        nlohmann::json jsonPosition = jsonComponent["Position"];
        transformComponent.setPosition({jsonPosition[0], jsonPosition[1], jsonPosition[2]});

        nlohmann::json jsonRotation = jsonComponent["Rotation"];
        transformComponent.setRotation({jsonRotation[0], jsonRotation[1], jsonRotation[2]});

        nlohmann::json jsonSize = jsonComponent["Size"];
        transformComponent.setSize({jsonSize[0], jsonSize[1], jsonSize[2]});
    }

    void SceneSerializer::serializeMeshComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<MeshComponent>())
            return;
        auto& meshComponent = entity.getComponent<MeshComponent>();
        nlohmann::json jsonMeshComponent;
        jsonMeshComponent["ModelPath"] = meshComponent.modelPath;
            
        nlohmann::json jsonMeshTextures = nlohmann::json::array();
        for(auto& texture : meshComponent.textures) {
            nlohmann::json jsonMeshTexture = nlohmann::json::array();
            auto& attachments = texture->getAttachments();
            for(auto& specification : attachments.textureSpecifications)
                jsonMeshTexture.push_back(specification.imgPath);
            jsonMeshTextures.push_back(jsonMeshTexture);
        }
        jsonMeshComponent["Textures"] = jsonMeshTextures;

        jsonEntity["MeshComponent"] = jsonMeshComponent;
    }

    void SceneSerializer::serializeMeshRendererComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<MeshRendererComponent>())
            return;
        auto& meshRendererComponent = entity.getComponent<MeshRendererComponent>();
        nlohmann::json jsonMeshRendererComponent;

        nlohmann::json jsonMaterial = nlohmann::json::object();
        jsonMaterial["Ambient"] = meshRendererComponent.material->ambient;
        jsonMaterial["Diffuse"] = meshRendererComponent.material->diffuse;
        jsonMaterial["Shininess"] = meshRendererComponent.material->shininess;
        jsonMaterial["Specular"] = meshRendererComponent.material->specular;
        nlohmann::json jsonMaterialColor = nlohmann::json::array();
        jsonMaterialColor.push_back(meshRendererComponent.material->objColor.x);
        jsonMaterialColor.push_back(meshRendererComponent.material->objColor.y);
        jsonMaterialColor.push_back(meshRendererComponent.material->objColor.z);
        jsonMaterial["ObjColor"] = jsonMaterialColor;
        jsonMeshRendererComponent["Material"] = jsonMaterial;
            
        jsonEntity["MeshRendererComponent"] = jsonMeshRendererComponent;
    }

    void SceneSerializer::deserializeMeshRendererComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent) {
        if(key != "MeshRendererComponent")
            return;
        if(!entity.hasComponent<MeshRendererComponent>())
            entity.addComponent<MeshRendererComponent>();
        auto& meshRendererComponent = entity.getComponent<MeshRendererComponent>();

        nlohmann::json jsonMaterial = jsonComponent["Material"];
        meshRendererComponent.material->ambient = jsonMaterial["Ambient"];
        meshRendererComponent.material->diffuse = jsonMaterial["Diffuse"];
        meshRendererComponent.material->shininess = jsonMaterial["Shininess"];
        meshRendererComponent.material->specular = jsonMaterial["Specular"];
        nlohmann::json jsonObjColor = jsonMaterial["ObjColor"];
        meshRendererComponent.material->objColor = { jsonObjColor[0], jsonObjColor[1], jsonObjColor[2] };
    }

    void SceneSerializer::serializeCameraComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<CameraComponent>())
            return;
        auto& cameraComponent = entity.getComponent<CameraComponent>();
        Camera* camera = cameraComponent.getSource();
        nlohmann::json jsonCameraComponent;

        jsonCameraComponent["IsFocusedOn"] = cameraComponent.isFocusedOn();
        jsonCameraComponent["Type"] = camera->getType();

        nlohmann::json jsonPerspectiveSpecification;
        auto& perspectiveSpecification = camera->getPerspectiveSpecification();
        jsonPerspectiveSpecification["FOV"] = perspectiveSpecification.fov;
        jsonPerspectiveSpecification["WndWidth"] = perspectiveSpecification.wndWidth;
        jsonPerspectiveSpecification["WndHeight"] = perspectiveSpecification.wndHeight;
        jsonPerspectiveSpecification["Near"] = perspectiveSpecification.near;
        jsonPerspectiveSpecification["Far"] = perspectiveSpecification.far;
        jsonCameraComponent["PerspectiveSpecification"] = jsonPerspectiveSpecification;

        nlohmann::json jsonOrthographicSpecification;
        auto& orthographicSpecification = camera->getOrthographicSpecification();
        jsonOrthographicSpecification["Left"] = orthographicSpecification.left;
        jsonOrthographicSpecification["Right"] = orthographicSpecification.right;
        jsonOrthographicSpecification["Bottom"] = orthographicSpecification.bottom;
        jsonOrthographicSpecification["Top"] = orthographicSpecification.top;
        jsonOrthographicSpecification["Near"] = orthographicSpecification.near;
        jsonOrthographicSpecification["Far"] = orthographicSpecification.far;
        jsonCameraComponent["OrthographicSpecification"] = jsonOrthographicSpecification;

        jsonEntity["CameraComponent"] = jsonCameraComponent;
    }

    void SceneSerializer::deserializeCameraComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent) {
        if(key != "CameraComponent")
            return;
        if(!entity.hasComponent<CameraComponent>())
            entity.addComponent<CameraComponent>();
        auto& cameraComponent = entity.getComponent<CameraComponent>();
        Camera* cameraSouce = cameraComponent.getSource();

        cameraComponent.setFocuse(jsonComponent["IsFocusedOn"]);
        
        nlohmann::json jsonPerspectiveSpecification = jsonComponent["PerspectiveSpecification"];
        nlohmann::json jsonOrthographicSpecification = jsonComponent["OrthographicSpecification"];
        auto type = static_cast<CameraProjectionType>(jsonComponent["Type"]);
        switch (type) {
        case CameraProjectionType::Perspective:
            cameraSouce->setOrthographic(
                jsonOrthographicSpecification["Left"],
                jsonOrthographicSpecification["Right"],
                jsonOrthographicSpecification["Bottom"],
                jsonOrthographicSpecification["Top"],
                jsonOrthographicSpecification["Near"],
                jsonOrthographicSpecification["Far"]
            );
            cameraSouce->setPerspective(
                jsonPerspectiveSpecification["FOV"], 
                jsonPerspectiveSpecification["WndWidth"],
                jsonPerspectiveSpecification["WndHeight"],
                jsonPerspectiveSpecification["Near"],
                jsonPerspectiveSpecification["Far"]
            );
            break;
        case CameraProjectionType::Orthographic:
            cameraSouce->setPerspective(
                jsonPerspectiveSpecification["FOV"], 
                jsonPerspectiveSpecification["WndWidth"],
                jsonPerspectiveSpecification["WndHeight"],
                jsonPerspectiveSpecification["Near"],
                jsonPerspectiveSpecification["Far"]
            );
            cameraSouce->setOrthographic(
                jsonOrthographicSpecification["Left"],
                jsonOrthographicSpecification["Right"],
                jsonOrthographicSpecification["Bottom"],
                jsonOrthographicSpecification["Top"],
                jsonOrthographicSpecification["Near"],
                jsonOrthographicSpecification["Far"]
            );
            break;
        }
    }

    void SceneSerializer::serializeLightComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<LightComponent>())
            return;
        auto& lightComponent = entity.getComponent<LightComponent>();
        nlohmann::json jsonLightComponent;

        jsonLightComponent["CastShadows"] = lightComponent.castShadows;
        jsonLightComponent["Type"] = lightComponent.type;
        jsonLightComponent["Constant"] = lightComponent.constant;
        jsonLightComponent["Linear"] = lightComponent.linear;
        jsonLightComponent["Quadratic"] = lightComponent.quadratic;
        jsonLightComponent["InnerRadius"] = lightComponent.innerRadius;
        jsonLightComponent["OuterRadius"] = lightComponent.outerRadius;

        nlohmann::json jsonLightColor = nlohmann::json::array();
        jsonLightColor.push_back(lightComponent.color.x);
        jsonLightColor.push_back(lightComponent.color.y);
        jsonLightColor.push_back(lightComponent.color.z);
        jsonLightComponent["Color"] = jsonLightColor;
            
        jsonEntity["LightComponent"] = jsonLightComponent;
    }

    void SceneSerializer::deserializeLightComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent) {
        if(key != "LightComponent")
            return;
        if(!entity.hasComponent<LightComponent>())
            entity.addComponent<LightComponent>();
        auto& lightComponent = entity.getComponent<LightComponent>();

        lightComponent.castShadows = jsonComponent["CastShadows"];
        lightComponent.constant = jsonComponent["Constant"];
        lightComponent.linear = jsonComponent["Linear"];
        lightComponent.quadratic = jsonComponent["Quadratic"];
        lightComponent.innerRadius = jsonComponent["InnerRadius"];
        lightComponent.outerRadius = jsonComponent["OuterRadius"];
        lightComponent.type = jsonComponent["Type"];
        nlohmann::json jsonLightColor = jsonComponent["Color"];
        lightComponent.color = { jsonLightColor[0], jsonLightColor[1], jsonLightColor[2] };
    }

    void SceneSerializer::serializePhysicsComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<PhysicsComponent>())
            return;
        auto& physicsComponent = entity.getComponent<PhysicsComponent>();
        auto& transform = physicsComponent.getRigidBody()->getWorldTransform();
        nlohmann::json jsonPhysicsComponent;

        jsonPhysicsComponent["Type"] = physicsComponent.getColliderType();
        jsonPhysicsComponent["Mass"] = physicsComponent.getMass();

        auto& position = transform.getOrigin();
        nlohmann::json jsonPosition = nlohmann::json::array();
        jsonPosition.push_back(position.x());
        jsonPosition.push_back(position.y());
        jsonPosition.push_back(position.z());
        jsonPhysicsComponent["Position"] = jsonPosition;

        auto& rotation = transform.getRotation();
        nlohmann::json jsonRotation = nlohmann::json::array();
        jsonRotation.push_back(rotation.x());
        jsonRotation.push_back(rotation.y());
        jsonRotation.push_back(rotation.z());
        jsonRotation.push_back(rotation.w());
        jsonPhysicsComponent["Rotation"] = jsonRotation;

        auto& size = physicsComponent.getRigidBody()->getCollisionShape()->getLocalScaling();
        nlohmann::json jsonSize = nlohmann::json::array();
        jsonSize.push_back(size.x());
        jsonSize.push_back(size.y());
        jsonSize.push_back(size.z());
        jsonPhysicsComponent["Size"] = jsonSize;

        jsonEntity["PhysicsComponent"] = jsonPhysicsComponent;
    }

    void SceneSerializer::deserializePhysicsComponent(Scene* scene, Entity& entity, const std::string& key, nlohmann::json& jsonComponent) {
        if(key != "PhysicsComponent")
            return;
        if(!entity.hasComponent<PhysicsComponent>())
            entity.addComponent<PhysicsComponent>();
        auto& physicsComponent = entity.getComponent<PhysicsComponent>();

        physicsComponent.setMass(scene->getDynamicWorld(), jsonComponent["Mass"]);
        physicsComponent.setColliderType(jsonComponent["Type"]);

        nlohmann::json jsonPosition = jsonComponent["Position"];
        physicsComponent.setPosition({jsonPosition[0], jsonPosition[1], jsonPosition[2]});

        nlohmann::json jsonRotation = jsonComponent["Rotation"];
        physicsComponent.setRotation({jsonRotation[0], jsonRotation[1], jsonRotation[2]});

        nlohmann::json jsonSize = jsonComponent["Size"];
        physicsComponent.setSize(scene->getDynamicWorld(), {jsonSize[0], jsonSize[1], jsonSize[2]});
    }

    void SceneSerializer::serializeScriptComponent(Entity& entity, nlohmann::json& jsonEntity) {
        if(!entity.hasComponent<ScriptComponent>())
            return;
        auto& scriptComponent = entity.getComponent<ScriptComponent>();
        nlohmann::json jsonScriptComponent;

        jsonScriptComponent["BehaviorClassName"] = scriptComponent.getBehaviorClassName();

        jsonEntity["ScriptComponent"] = jsonScriptComponent;
    }

    void SceneSerializer::deserializeScriptComponent(Entity& entity, const std::string& key, nlohmann::json& jsonComponent, Registry<Behavior>& registry) {
        if(key != "ScriptComponent")
            return;
        if(!entity.hasComponent<ScriptComponent>())
            entity.addComponent<ScriptComponent>();
        auto& scriptComponent = entity.getComponent<ScriptComponent>();

        std::string behaviorName = jsonComponent["BehaviorClassName"];
        Behavior* behavior = registry.get(behaviorName);
        if(behavior) {
            scriptComponent.bind(behavior, behaviorName);
            registry.erase(behaviorName);
            RegistryLoader::load(registry);
        } else
            scriptComponent.bind<Behavior>();
    }

    std::string SceneSerializer::deleteInvertedCommas(const std::string& str) {
        std::string result = str;
        result.erase(0, 0);
        result.erase(result.length(), 1);
        return result;
    }
}