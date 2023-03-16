#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <string>
#include <entt/entt.hpp>

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "scene/shape-specification.hpp"
#include "model-loader/model-loader.hpp"
#include "renderer/renderer.hpp"
#include "registry/registry.hpp"

namespace TWE {
    class Shape{
    public:
        Shape();
        static void reset();
        static void initialize(Registry<MeshSpecification>* meshRegistry, Registry<MeshRendererSpecification>* meshRendererRegistry, 
            Registry<TextureAttachmentSpecification>* textureRegistry, const std::filesystem::path& rootPath);
        static Entity createCubeEntity(IScene* scene, const TextureAttachmentSpecification& textureAtttachments = {});
        static Entity createPlateEntity(IScene* scene, const TextureAttachmentSpecification& textureAtttachments = {});
        static Entity createCubemapEntity(IScene* scene, TextureAttachmentSpecification& textureAtttachments);
        static Entity createSpotLightEntity(IScene* scene, const glm::vec3& color = {1.f, 1.f, 1.f}, float innerRadius = 15.f, float outerRadius = 20.f, float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);
        static Entity createPointLightEntity(IScene* scene, const glm::vec3& color = {1.f, 1.f, 1.f}, float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);
        static Entity createDirLightEntity(IScene* scene, const glm::vec3& color = {1.f, 1.f, 1.f});
        static Entity createCameraEntity(IScene* scene);
        static Entity createModelEntity(IScene* scene, const std::filesystem::path& modelPath);
        static Entity createModelEntity(IScene* scene, const std::filesystem::path& modelPath, int index);
        static MeshSpecification* registerMeshSpecification(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, EntityCreationType creationType, 
            const std::string& id, const ModelMeshSpecification& modelSpec = {});
        static MeshRendererSpecification* registerMeshRendererSpecification(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& id);
        static ShapeSpecification* shapeSpec;
    private:
        static bool registerModel(const std::filesystem::path& modelPath);
        static void fillMeshRegistry();
        static void fillMeshRendererRegistry();
        static float cubeVertices[];
        static uint32_t cubeIndices[];
        static float plateVertices[];
        static uint32_t plateIndices[];
        static uint32_t cubemapIndices[];
    };
}

#endif