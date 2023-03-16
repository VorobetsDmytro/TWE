#include "scene/shape.hpp"

namespace TWE {
    ShapeSpecification* Shape::shapeSpec = new ShapeSpecification();
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

    void Shape::reset() {
        shapeSpec->meshRegistry->clean();
        shapeSpec->meshRendererRegistry->clean();
        shapeSpec->textureRegistry->clean();
        shapeSpec->meshCounter = 0;
        shapeSpec->textureNumber = 0;
        fillMeshRegistry();
        fillMeshRendererRegistry();
    }

    void Shape::initialize(Registry<MeshSpecification>* meshRegistryT, Registry<MeshRendererSpecification>* meshRendererRegistryT, 
    Registry<TextureAttachmentSpecification>* textureRegistryT, const std::filesystem::path& rootPath) {
        shapeSpec->meshRegistry = meshRegistryT;
        shapeSpec->meshRendererRegistry = meshRendererRegistryT;
        shapeSpec->textureRegistry = textureRegistryT;
        shapeSpec->meshCounter = 0;
        shapeSpec->textureNumber = 0;
        shapeSpec->rootPath = rootPath;
        fillMeshRegistry();
        fillMeshRendererRegistry();
    }

    void Shape::fillMeshRegistry() {
        // Cube mesh
        std::string cubeMeshId = "Cube mesh";
        auto cubeMeshComponent = MeshComponent(Shape::cubeVertices, sizeof(Shape::cubeVertices), 
            Shape::cubeIndices, sizeof(Shape::cubeIndices), cubeMeshId, ModelMeshSpecification{}, TextureAttachmentSpecification{});
        registerMeshSpecification(cubeMeshComponent.getVAO(), cubeMeshComponent.getVBO(), cubeMeshComponent.getEBO(), EntityCreationType::Cube, cubeMeshId);
        // Plate mesh
        std::string plateMeshId = "Plate mesh";
        auto plateMeshComponent = MeshComponent(Shape::plateVertices, sizeof(Shape::plateVertices), 
            Shape::plateIndices, sizeof(Shape::plateIndices), plateMeshId, ModelMeshSpecification{}, TextureAttachmentSpecification{});
        registerMeshSpecification(plateMeshComponent.getVAO(), plateMeshComponent.getVBO(), plateMeshComponent.getEBO(), EntityCreationType::Plate, plateMeshId);
        // Cubemap mesh
        std::string cubemapMeshId = "Cubemap mesh";
        auto cubemapMeshComponent = MeshComponent(Shape::cubeVertices, sizeof(Shape::cubeVertices), 
            Shape::cubemapIndices, sizeof(Shape::cubemapIndices), cubemapMeshId, ModelMeshSpecification{}, TextureAttachmentSpecification{});
        registerMeshSpecification(cubemapMeshComponent.getVAO(), cubemapMeshComponent.getVBO(), cubemapMeshComponent.getEBO(), EntityCreationType::Cubemap, cubemapMeshId);
    }

    void Shape::fillMeshRendererRegistry() {
        std::string rootPathStr = shapeSpec->rootPath.string();
        // Default renderer
        std::string defaultMeshRendererId = "Default renderer";
        registerMeshRendererSpecification(rootPathStr + SHADER_PATHS[ShaderIndices::DEFAULT_VERT], rootPathStr + SHADER_PATHS[ShaderIndices::DEFAULT_FRAG], 
            defaultMeshRendererId);
        // Cubemap renderer
        std::string cubemapMeshRendererId = "Cubemap renderer";
        registerMeshRendererSpecification(rootPathStr + SHADER_PATHS[ShaderIndices::CUBEMAP_VERT], rootPathStr + SHADER_PATHS[ShaderIndices::CUBEMAP_FRAG], 
            cubemapMeshRendererId);
        // Collider renderer
        std::string colliderMeshRendererId = "Collider renderer";
        registerMeshRendererSpecification(rootPathStr + SHADER_PATHS[ShaderIndices::COLLIDER_VERT], rootPathStr + SHADER_PATHS[ShaderIndices::COLLIDER_FRAG], 
            colliderMeshRendererId);
        // UI Renderer
        std::string uiRendererId = "UI renderer";
        registerMeshRendererSpecification(rootPathStr + SHADER_PATHS[ShaderIndices::UI_VERT], rootPathStr + SHADER_PATHS[ShaderIndices::UI_FRAG], uiRendererId);
    }

    Entity Shape::createCubeEntity(IScene* scene, const TextureAttachmentSpecification& textureAtttachments) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Cube);
        std::string meshRendererId = "Default renderer";
        auto meshRendererSpecification = shapeSpec->meshRendererRegistry->get(meshRendererId);
        std::string meshId = "Cube mesh";
        auto meshSpecification = shapeSpec->meshRegistry->get(meshId);
        if(meshSpecification)
            entity.addComponent<MeshComponent>(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId, ModelMeshSpecification{}, textureAtttachments);
        else {
            auto& meshComponent = entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), 
                Shape::cubeIndices, sizeof(Shape::cubeIndices), meshId, ModelMeshSpecification{}, textureAtttachments);
            registerMeshSpecification(meshComponent.getVAO(), meshComponent.getVBO(), meshComponent.getEBO(), EntityCreationType::Cube, meshId);
        }
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), 
            meshRendererSpecification->fragmentShaderPath.c_str(), (int)entity.getSource(), meshRendererId);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Cube");
        return entity;
    }

    Entity Shape::createPlateEntity(IScene* scene, const TextureAttachmentSpecification& textureAtttachments) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Plate);
        std::string meshRendererId = "Default renderer";
        auto meshRendererSpecification = shapeSpec->meshRendererRegistry->get(meshRendererId);
        std::string meshId = "Plate mesh";
        auto meshSpecification = shapeSpec->meshRegistry->get(meshId);
        if(meshSpecification)
            entity.addComponent<MeshComponent>(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId, ModelMeshSpecification{}, textureAtttachments);
        else {
            auto& meshComponent = entity.addComponent<MeshComponent>(Shape::plateVertices, sizeof(Shape::plateVertices), 
                Shape::plateIndices, sizeof(Shape::plateIndices), meshId, ModelMeshSpecification{}, textureAtttachments);
            registerMeshSpecification(meshComponent.getVAO(), meshComponent.getVBO(), meshComponent.getEBO(), EntityCreationType::Plate, meshId);
        }
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), 
            meshRendererSpecification->fragmentShaderPath.c_str(), (int)entity.getSource(), meshRendererId);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Plate");
        return entity;
    }

    Entity Shape::createCubemapEntity(IScene* scene, TextureAttachmentSpecification& textureAtttachments) {
        Texture* texture = Texture::generateCubemapTexture(textureAtttachments);
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Cubemap);
        std::string meshRendererId = "Cubemap renderer";
        auto meshRendererSpecification = shapeSpec->meshRendererRegistry->get(meshRendererId);
        auto& transformComponent = entity.getComponent<TransformComponent>();
        std::string meshId = "Cubemap mesh";
        auto meshSpecification = shapeSpec->meshRegistry->get(meshId);
        if(meshSpecification) {
            if(texture)
                entity.addComponent<MeshComponent>(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId, ModelMeshSpecification{}, texture);
            else
                entity.addComponent<MeshComponent>(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId, ModelMeshSpecification{});
        } else {
            if(texture) {
                auto& meshComponent = entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), 
                    Shape::cubemapIndices, sizeof(Shape::cubemapIndices), meshId, ModelMeshSpecification{}, texture);
                registerMeshSpecification(meshComponent.getVAO(), meshComponent.getVBO(), meshComponent.getEBO(), EntityCreationType::Cubemap, meshId);
            } else {
                auto& meshComponent = entity.addComponent<MeshComponent>(Shape::cubeVertices, sizeof(Shape::cubeVertices), 
                    Shape::cubemapIndices, sizeof(Shape::cubemapIndices), meshId, ModelMeshSpecification{});
                registerMeshSpecification(meshComponent.getVAO(), meshComponent.getVBO(), meshComponent.getEBO(), EntityCreationType::Cubemap, meshId);
            }
        }
        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), 
            meshRendererSpecification->fragmentShaderPath.c_str(), (int)entity.getSource(), meshRendererId);
        transformComponent.scale({1000.f, 1000.f, 1000.f});
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Cubemap");
        return entity;
    }

    Entity Shape::createSpotLightEntity(IScene* scene, const glm::vec3& color, float innerRadius, float outerRadius, float constant, float linear, float quadratic) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::SpotLight);
        entity.addComponent<LightComponent>(color, innerRadius, outerRadius, constant, linear, quadratic, LightType::Spot);
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Spot light");
        return entity;
    }

    Entity Shape::createPointLightEntity(IScene* scene, const glm::vec3& color, float constant, float linear, float quadratic) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::PointLight);
        entity.addComponent<LightComponent>(color, 15.f, 20.f, constant, linear, quadratic, LightType::Point);
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Point light");
        return entity;
    }

    Entity Shape::createDirLightEntity(IScene* scene, const glm::vec3& color) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::DirLight);
        entity.addComponent<LightComponent>(color, 15.f, 20.f, 1.f, 0.045f, 0.0075f, LightType::Dir);
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Dir light");
        return entity;
    }

    Entity Shape::createCameraEntity(IScene* scene) {
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Camera);
        entity.addComponent<CameraComponent>();
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Camera");
        return entity;
    }

    bool Shape::registerModel(const std::filesystem::path& modelPath) {
        try {
            ModelLoader mloader;
            auto modelLoaderData = mloader.loadModel(modelPath.string());
            int index = 0;
            for(auto& mesh : modelLoaderData->meshComponents){
                std::string registryId = "Model mesh-" + std::to_string(shapeSpec->meshCounter++);
                MeshComponent meshComponent(mesh);
                ModelMeshSpecification modelSpec(true, modelLoaderData->fullPath, index++);
                registerMeshSpecification(meshComponent.getVAO(), meshComponent.getVBO(), meshComponent.getEBO(), EntityCreationType::Model, registryId, modelSpec);
            }
        } catch(const std::exception& error) {
            std::cout << error.what() << std::endl;
            return false; 
        }
        return true;
    }

    Entity Shape::createModelEntity(IScene* scene, const std::filesystem::path& modelPath) {
        auto meshSpecs = shapeSpec->meshRegistry->getValues();
        std::string meshId;
        for(auto& spec : meshSpecs)
            if(std::filesystem::absolute(spec->modelSpec.modelPath) == std::filesystem::absolute(modelPath)) {
                meshId = spec->meshId;
                break;
            }
        if(meshId.empty())
            if(!registerModel(modelPath))
                return {};
        std::string meshRendererId = "Default renderer";
        auto meshRendererSpecification = shapeSpec->meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification)
            return {};
        std::vector<Entity> models;
        meshSpecs = shapeSpec->meshRegistry->getValues();
        for(auto& spec : meshSpecs)
            if(std::filesystem::absolute(spec->modelSpec.modelPath) == std::filesystem::absolute(modelPath)) {
                Entity entity = scene->createEntity();
                auto& creationType = entity.getComponent<CreationTypeComponent>();
                creationType.setType(EntityCreationType::Model);
                entity.addComponent<MeshComponent>(spec->vao, spec->vbo, spec->ebo, spec->meshId, spec->modelSpec);
                entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), 
                        meshRendererSpecification->fragmentShaderPath.c_str(), (int)entity.getSource(), meshRendererId);
                auto& nameComponent = entity.getComponent<NameComponent>();
                nameComponent.setName("Model");
                models.push_back(entity);
            }
        if(models.size() > 1) {
            Entity emptyEntity = scene->createEntity("ModelRoot");
            auto& parentChildsComponent = emptyEntity.getComponent<ParentChildsComponent>();
            for(auto& entityModel : models) {
                parentChildsComponent.childs.push_back(entityModel.getSource());
                auto& parentChildsComponentModel = entityModel.getComponent<ParentChildsComponent>();
                parentChildsComponentModel.parent = emptyEntity.getSource();
            }
            return emptyEntity;
        } else if(models.size() == 1)
            return models.back();
        else
            return {};
    }

    Entity Shape::createModelEntity(IScene* scene, const std::filesystem::path& modelPath, int index) {
        auto meshSpecs = shapeSpec->meshRegistry->getValues();
        std::string meshId;
        for(auto& spec : meshSpecs)
            if(std::filesystem::absolute(spec->modelSpec.modelPath) == std::filesystem::absolute(modelPath)
            && spec->modelSpec.modelIndex == index) {
                meshId = spec->meshId;
                break;
            }
        if(meshId.empty()) {
            if(!registerModel(modelPath))
                return {};
            meshSpecs = shapeSpec->meshRegistry->getValues();
            for(auto& spec : meshSpecs)
                if(std::filesystem::absolute(spec->modelSpec.modelPath) == std::filesystem::absolute(modelPath)
                && spec->modelSpec.modelIndex == index) {
                    meshId = spec->meshId;
                    break;
                }
            if(meshId.empty())
                return {};
        }
        auto meshSpecification = shapeSpec->meshRegistry->get(meshId);
        if(!meshSpecification)
            return {};
        std::string meshRendererId = "Default renderer";
        auto meshRendererSpecification = shapeSpec->meshRendererRegistry->get(meshRendererId);
        if(!meshRendererSpecification)
            return {};
        Entity entity = scene->createEntity();
        auto& creationType = entity.getComponent<CreationTypeComponent>();
        creationType.setType(EntityCreationType::Model);
        entity.addComponent<MeshComponent>(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId, meshSpecification->modelSpec);
        entity.addComponent<MeshRendererComponent>(meshRendererSpecification->vertexShaderPath.c_str(), 
                meshRendererSpecification->fragmentShaderPath.c_str(), (int)entity.getSource(), meshRendererId);
        auto& nameComponent = entity.getComponent<NameComponent>();
        nameComponent.setName("Model");
        return entity;
    }

    MeshSpecification* Shape::registerMeshSpecification(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo,  
    EntityCreationType creationType, const std::string& id, const ModelMeshSpecification& modelSpec) {
        auto meshSpecification = shapeSpec->meshRegistry->add<MeshSpecification>(id);
        meshSpecification->vao = vao;
        meshSpecification->vbo = vbo;
        meshSpecification->ebo = ebo;
        meshSpecification->modelSpec = modelSpec;
        meshSpecification->meshId = id;
        meshSpecification->creationType = creationType;
        return meshSpecification;
    }

    MeshRendererSpecification* Shape::registerMeshRendererSpecification(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& id) {
        auto meshRendererSpecification = shapeSpec->meshRendererRegistry->add<MeshRendererSpecification>(id);
        meshRendererSpecification->vertexShaderPath = vertexShaderPath;
        meshRendererSpecification->fragmentShaderPath = fragmentShaderPath;
        return meshRendererSpecification;
    }
}