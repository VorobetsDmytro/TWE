#include "scene/shape.hpp"

namespace TWE {
    float Shape::cubeVertices[] = {
        //front
        -0.5f, -0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      0.f, 1.f, //left down     0
        0.5f,  -0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      1.f, 1.f, //right down    1
        0.5f,   0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      1.f, 0.f, //right up      2
        -0.5f,  0.5f, 0.5f,   0.0f,  0.0f, 1.0f,      0.f, 0.f, //left up       3
        //back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     1.f, 1.f, //right down    4
        0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     0.f, 1.f, //left down     5
        0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     0.f, 0.f, //left up       6
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     1.f, 0.f, //right up      7
        //left
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.f, 1.f, //left down     8
        -0.5f, -0.5f, 0.5f,   -1.0f,  0.0f,  0.0f,    1.f, 1.f, //right down    9
        -0.5f,  0.5f, 0.5f,   -1.0f,  0.0f,  0.0f,    1.f, 0.f, //right up      10
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.f, 0.f, //left up       11
        //right
        0.5f,  -0.5f, 0.5f,   1.0f,  0.0f,  0.0f,     0.f, 1.f, //left down     12
        0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,     1.f, 1.f, //right down    13
        0.5f,   0.5f, -0.5f,  1.0f,  0.0f,  0.0f,     1.f, 0.f, //right up      14
        0.5f,   0.5f, 0.5f,   1.0f,  0.0f,  0.0f,     0.f, 0.f, //left up       15
        //top
        -0.5f,  0.5f, 0.5f,   0.0f,  1.0f,  0.0f,     0.f, 1.f, //left down     16
        0.5f,   0.5f, 0.5f,   0.0f,  1.0f,  0.0f,     1.f, 1.f, //right down    17
        0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f,     1.f, 0.f, //right up      18
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,     0.f, 0.f, //left up       19
        //bottom
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,     0.f, 1.f, //left down     20
        0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,     1.f, 1.f, //right down    21
        0.5f,  -0.5f, 0.5f,   0.0f, -1.0f,  0.0f,     1.f, 0.f, //right up      22
        -0.5f, -0.5f, 0.5f,   0.0f, -1.0f,  0.0f,     0.f, 0.f, //left up       23
    };
    uint32_t Shape::cubeIndices[] = {
        //front
        0, 1, 2,
        0, 2, 3,
        //back
        4, 6, 5,
        4, 7, 6,
        //left
        8, 9, 10,
        8, 10, 11,
        //right
        12, 13, 14,
        12, 14, 15,
        //top
        16, 17, 18,
        16, 18, 19,
        //bottom
        20, 21, 22,
        20, 22, 23
    };
    float Shape::plateVertices[] = {
        -5.f, 0.f,  5.f,    0.0f,  1.0f,  0.0f,     0.f, 1.f,
        5.f,  0.f,  5.f,    0.0f,  1.0f,  0.0f,     1.f, 1.f,
        5.f,  0.f, -5.f,    0.0f,  1.0f,  0.0f,     1.f, 0.f,
        -5.f, 0.f, -5.f,    0.0f,  1.0f,  0.0f,     0.f, 0.f,
    };
    uint32_t Shape::plateIndices[] = {
        0, 1, 2,
        0, 2, 3
    };
    uint32_t Shape::cubemapIndices[] = {
        //front
        0, 2, 1,
        0, 3, 2,
        //back
        4, 5, 6,
        4, 6, 7,
        //left
        8, 10, 9,
        8, 11, 10,
        //right
        12, 14, 13,
        12, 15, 14,
        //top
        16, 18, 17,
        16, 19, 18,
        //bottom
        20, 22, 21,
        20, 23, 22
    };

    Entity Shape::createCubeEntity(Scene* scene, const std::vector<std::string>& texPaths) {
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::Cube);
        if(!texPaths.size())
            entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices));
        else 
            entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices), texPaths);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(SHADER_PATHS[ShaderIndices::DEFAULT_VERT], SHADER_PATHS[ShaderIndices::DEFAULT_FRAG]);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        auto& physicsComponent = entity.addComponent<PhysicsComponent>(ColliderType::Box, transformComponent.size, transformComponent.position, glm::vec3(0.f), 0.f);
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Cube");
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        scene->linkRigidBody(physicsComponent);
        return entity;
    }

    Entity Shape::createPlateEntity(Scene* scene, const std::vector<std::string>& texPaths) {
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::Plate);
        if(!texPaths.size())
            entity.addComponent<MeshComponent>(Shape::plateVertices, sizeof(Shape::plateVertices), Shape::plateIndices, sizeof(Shape::plateIndices));
        else
            entity.addComponent<MeshComponent>(Shape::plateVertices, sizeof(Shape::plateVertices), Shape::plateIndices, sizeof(Shape::plateIndices), texPaths);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(SHADER_PATHS[ShaderIndices::DEFAULT_VERT], SHADER_PATHS[ShaderIndices::DEFAULT_FRAG]);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        auto& physicsComponent = entity.addComponent<PhysicsComponent>(
            ColliderType::Box,
            glm::vec3{10.f * transformComponent.size.x, 0.0001f * transformComponent.size.y, 10.f * transformComponent.size.z}, 
            glm::vec3{transformComponent.position.x, transformComponent.position.y, transformComponent.position.z},
            glm::vec3(0.f), 
            0.f
        );
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Plate");
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        scene->linkRigidBody(physicsComponent);
        return entity;
    }

    Entity Shape::createCubemapEntity(Scene* scene, const std::vector<std::string>& texPaths) {
        Texture* texture = Renderer::generateCubemapTexture(texPaths);
        if(!texture)
            return {};
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::Cubemap);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubemapIndices, sizeof(Shape::cubemapIndices), std::vector<Texture*>{texture});
        entity.addComponent<MeshRendererComponent>(SHADER_PATHS[ShaderIndices::CUBEMAP_VERT], SHADER_PATHS[ShaderIndices::CUBEMAP_FRAG]);
        transformComponent.scale({1000.f, 1000.f, 1000.f});
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Cubemap");
        return entity;
    }

    Entity Shape::createSpotLightEntity(Scene* scene, const glm::vec3& color, float innerRadius, float outerRadius, float constant, float linear, float quadratic) {
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::SpotLight);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices));
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(SHADER_PATHS[ShaderIndices::LIGHT_VERT], SHADER_PATHS[ShaderIndices::LIGHT_FRAG]);
        entity.addComponent<LightComponent>(color, innerRadius, outerRadius, constant, linear, quadratic, LightType::SPOT);
        transformComponent.scale({0.5f, 0.5f, 0.5f});
        meshRendererComponent.shader->setUniform(("type." + lightTypes[LightType::SPOT]).c_str(), true);
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Spot light");
        return entity;
    }

    Entity Shape::createPointLightEntity(Scene* scene, const glm::vec3& color, float constant, float linear, float quadratic) {
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::PointLight);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices));
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(SHADER_PATHS[ShaderIndices::LIGHT_VERT], SHADER_PATHS[ShaderIndices::LIGHT_FRAG]);
        entity.addComponent<LightComponent>(color, 15.f, 20.f, constant, linear, quadratic, LightType::POINT);
        transformComponent.scale({0.5f, 0.5f, 0.5f});
        meshRendererComponent.shader->setUniform(("type." + lightTypes[LightType::POINT]).c_str(), true);
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Point light");
        return entity;
    }

    Entity Shape::createDirLightEntity(Scene* scene, const glm::vec3& color) {
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::DirLight);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), Shape::cubeIndices, sizeof(Shape::cubeIndices));
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(SHADER_PATHS[ShaderIndices::LIGHT_VERT], SHADER_PATHS[ShaderIndices::LIGHT_FRAG]);
        entity.addComponent<LightComponent>(color, 15.f, 20.f, 1.f, 0.045f, 0.0075f, LightType::DIR);
        transformComponent.scale({0.5f, 0.5f, 0.5f});
        meshRendererComponent.shader->setUniform(("type." + lightTypes[LightType::DIR]).c_str(), true);
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Dir light");
        return entity;
    }

    Entity Shape::createCameraEntity(Scene* scene) {
        Entity entity = scene->createEntity();
        entity.addComponent<CreationTypeComponent>(EntityCreationType::Camera);
        entity.addComponent<CameraComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Camera");
        return entity;
    }

    std::vector<Entity> Shape::createModelEntity(Scene* scene, ModelLoaderData* modelLoaderData) {
        std::vector<Entity> models;
        entt::registry* registry = scene->getRegistry();
        for(auto& mesh : modelLoaderData->meshComponents){
            Entity entity = scene->createEntity();
            entity.addComponent<CreationTypeComponent>(EntityCreationType::Model);
            entity.addComponent<MeshComponent>(mesh);
            auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(SHADER_PATHS[modelLoaderData->vert], SHADER_PATHS[modelLoaderData->frag]);
            auto& nameComponent = entity.getComponent<NameComponent>();
            nameComponent.setName("Model");
            meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
            models.push_back(entity);
            auto& meshComponent = entity.getComponent<MeshComponent>();
            meshComponent.modelPath = modelLoaderData->fullPath;
        }
        return models;
    }
}