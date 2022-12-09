#include "scene/shape.hpp"

namespace TWE {
    Registry<MeshSpecification>* Shape::meshRegistry = nullptr;
    Registry<MeshRendererSpecification>* Shape::meshRendererRegistry = nullptr;
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
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Cube);
        std::string meshId = "Cube mesh";
        auto meshSpecification = meshRegistry->get(meshId);
        if(!meshSpecification) {
            meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
            meshSpecification->vertices = Shape::cubeVertices;
            meshSpecification->verticesSize = sizeof(Shape::cubeVertices);
            meshSpecification->indices = Shape::cubeIndices;
            meshSpecification->indicesSize = sizeof(Shape::cubeIndices);
        }
        std::string meshRendererId = "Default renderer";
        auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification) {
            meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
            meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::DEFAULT_VERT];
            meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::DEFAULT_FRAG];
        }
        if(!texPaths.size())
            entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize);
        else 
            entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize, texPaths);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), meshRendererSpecification->fragmentShaderPath.c_str());
        auto& transformComponent = entity.getComponent<TransformComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Cube");
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        return entity;
    }

    Entity Shape::createPlateEntity(Scene* scene, const std::vector<std::string>& texPaths) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Plate);
        std::string meshId = "Plate mesh";
        auto meshSpecification = meshRegistry->get(meshId);
        if(!meshSpecification) {
            meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
            meshSpecification->vertices = Shape::plateVertices;
            meshSpecification->verticesSize = sizeof(Shape::plateVertices);
            meshSpecification->indices = Shape::plateIndices;
            meshSpecification->indicesSize = sizeof(Shape::plateIndices);
        }
        std::string meshRendererId = "Default renderer";
        auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification) {
            meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
            meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::DEFAULT_VERT];
            meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::DEFAULT_FRAG];
        }
        if(!texPaths.size())
            entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize);
        else
            entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize, texPaths);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), meshRendererSpecification->fragmentShaderPath.c_str());
        auto& transformComponent = entity.getComponent<TransformComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Plate");
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        return entity;
    }

    Entity Shape::createCubemapEntity(Scene* scene, const std::vector<std::string>& texPaths) {
        Texture* texture = Renderer::generateCubemapTexture(texPaths);
        if(!texture)
            return {};
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Cubemap);
        std::string meshId = "Cubemap mesh";
        auto meshSpecification = meshRegistry->get(meshId);
        if(!meshSpecification) {
            meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
            meshSpecification->vertices = Shape::cubeVertices;
            meshSpecification->verticesSize = sizeof(Shape::cubeVertices);
            meshSpecification->indices = Shape::cubemapIndices;
            meshSpecification->indicesSize = sizeof(Shape::cubemapIndices);
        }
        std::string meshRendererId = "Cubemap renderer";
        auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification) {
            meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
            meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::CUBEMAP_VERT];
            meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::CUBEMAP_FRAG];
        }
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize, std::vector<Texture*>{texture});
        entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), meshRendererSpecification->fragmentShaderPath.c_str());
        transformComponent.scale({1000.f, 1000.f, 1000.f});
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Cubemap");
        return entity;
    }

    Entity Shape::createSpotLightEntity(Scene* scene, const glm::vec3& color, float innerRadius, float outerRadius, float constant, float linear, float quadratic) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::SpotLight);
        std::string meshId = "Cube mesh";
        auto meshSpecification = meshRegistry->get(meshId);
        if(!meshSpecification) {
            meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
            meshSpecification->vertices = Shape::cubeVertices;
            meshSpecification->verticesSize = sizeof(Shape::cubeVertices);
            meshSpecification->indices = Shape::cubeIndices;
            meshSpecification->indicesSize = sizeof(Shape::cubeIndices);
        }
        std::string meshRendererId = "Light renderer";
        auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification) {
            meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
            meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::LIGHT_VERT];
            meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::LIGHT_FRAG];
        }
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), meshRendererSpecification->fragmentShaderPath.c_str());
        entity.addComponent<LightComponent>(color, innerRadius, outerRadius, constant, linear, quadratic, LightType::Spot);
        transformComponent.scale({0.5f, 0.5f, 0.5f});
        meshRendererComponent.shader->setUniform(("type." + lightTypes[LightType::Spot]).c_str(), true);
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Spot light");
        return entity;
    }

    Entity Shape::createPointLightEntity(Scene* scene, const glm::vec3& color, float constant, float linear, float quadratic) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::PointLight);
        std::string meshId = "Cube mesh";
        auto meshSpecification = meshRegistry->get(meshId);
        if(!meshSpecification) {
            meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
            meshSpecification->vertices = Shape::cubeVertices;
            meshSpecification->verticesSize = sizeof(Shape::cubeVertices);
            meshSpecification->indices = Shape::cubeIndices;
            meshSpecification->indicesSize = sizeof(Shape::cubeIndices);
        }
        std::string meshRendererId = "Light renderer";
        auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification) {
            meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
            meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::LIGHT_VERT];
            meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::LIGHT_FRAG];
        }
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), meshRendererSpecification->fragmentShaderPath.c_str());
        entity.addComponent<LightComponent>(color, 15.f, 20.f, constant, linear, quadratic, LightType::Point);
        transformComponent.scale({0.5f, 0.5f, 0.5f});
        meshRendererComponent.shader->setUniform(("type." + lightTypes[LightType::Point]).c_str(), true);
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Point light");
        return entity;
    }

    Entity Shape::createDirLightEntity(Scene* scene, const glm::vec3& color) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::DirLight);
        std::string meshId = "Cube mesh";
        auto meshSpecification = meshRegistry->get(meshId);
        if(!meshSpecification) {
            meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
            meshSpecification->vertices = Shape::cubeVertices;
            meshSpecification->verticesSize = sizeof(Shape::cubeVertices);
            meshSpecification->indices = Shape::cubeIndices;
            meshSpecification->indicesSize = sizeof(Shape::cubeIndices);
        }
        std::string meshRendererId = "Light renderer";
        auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification) {
            meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
            meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::LIGHT_VERT];
            meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::LIGHT_FRAG];
        }
        auto& transformComponent = entity.getComponent<TransformComponent>();
        entity.addComponent<MeshComponent>(meshSpecification->vertices, meshSpecification->verticesSize, meshSpecification->indices, meshSpecification->indicesSize);
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), meshRendererSpecification->fragmentShaderPath.c_str());
        entity.addComponent<LightComponent>(color, 15.f, 20.f, 1.f, 0.045f, 0.0075f, LightType::Dir);
        transformComponent.scale({0.5f, 0.5f, 0.5f});
        meshRendererComponent.shader->setUniform(("type." + lightTypes[LightType::Dir]).c_str(), true);
        meshRendererComponent.shader->setUniform("id", (int)entity.getSource());
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Dir light");
        return entity;
    }

    Entity Shape::createCameraEntity(Scene* scene) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Camera);
        entity.addComponent<CameraComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Camera");
        return entity;
    }

    std::vector<Entity> Shape::createModelEntity(Scene* scene, ModelLoaderData* modelLoaderData) {
        static int meshCounter = 0;
        std::vector<Entity> models;
        entt::registry* registry = scene->getRegistry();
        for(auto& mesh : modelLoaderData->meshComponents){
            Entity entity = scene->createEntity();
            auto& creationType = entity.getComponent<CreationTypeComponent>();
            creationType.setType(EntityCreationType::Model);
            std::string meshId = "Model mesh-" + std::to_string(meshCounter);
            auto meshSpecification = meshRegistry->get(meshId);
            if(!meshSpecification) {
                meshSpecification = meshRegistry->add<MeshSpecification>(meshId);
                meshSpecification->vertices = mesh.vertices;
                meshSpecification->verticesSize = mesh.vertSize;
                meshSpecification->indices = mesh.indices;
                meshSpecification->indicesSize = mesh.indSize;
            }
            std::string meshRendererId = "Default renderer";
            auto meshRendererSpecification = meshRendererRegistry->get(meshRendererId);
            if(!meshRendererSpecification) {
                meshRendererSpecification = meshRendererRegistry->add<MeshRendererSpecification>(meshRendererId);
                meshRendererSpecification->vertexShaderPath = SHADER_PATHS[ShaderIndices::DEFAULT_VERT];
                meshRendererSpecification->fragmentShaderPath = SHADER_PATHS[ShaderIndices::DEFAULT_FRAG];
            }
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