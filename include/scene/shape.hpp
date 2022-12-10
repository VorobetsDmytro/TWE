#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <string>
#include <entt/entt.hpp>

#include "scene/scene.hpp"
#include "model-loader/model-loader-data.hpp"
#include "scene/components/components.hpp"
#include "renderer/renderer.hpp"
#include "registry/registry.hpp"

namespace TWE {
    struct MeshSpecification {
        MeshSpecification() = default;
        MeshSpecification(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo)
            : vao(vao), vbo(vbo), ebo(ebo) {}
        std::shared_ptr<VAO> vao;
        std::shared_ptr<VBO> vbo;
        std::shared_ptr<EBO> ebo;
    };

    struct MeshRendererSpecification {
        MeshRendererSpecification() = default;
        MeshRendererSpecification(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
            : vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath) {}
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
    };

    class Shape{
    public:
        Shape() = default;
        static Entity createCubeEntity(Scene* scene, const std::vector<std::string>& texPaths = {});
        static Entity createPlateEntity(Scene* scene, const std::vector<std::string>& texPaths = {});
        static Entity createCubemapEntity(Scene* scene, const std::vector<std::string>& texPaths);
        static Entity createSpotLightEntity(Scene* scene, const glm::vec3& color = {1.f, 1.f, 1.f}, float innerRadius = 15.f, float outerRadius = 20.f, float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);
        static Entity createPointLightEntity(Scene* scene, const glm::vec3& color = {1.f, 1.f, 1.f}, float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);
        static Entity createDirLightEntity(Scene* scene, const glm::vec3& color = {1.f, 1.f, 1.f});
        static Entity createCameraEntity(Scene* scene);
        static std::vector<Entity> createModelEntity(Scene* scene, ModelLoaderData* modelLoaderData);
        static Registry<MeshSpecification>* meshRegistry;
        static Registry<MeshRendererSpecification>* meshRendererRegistry;
    private:
        static MeshSpecification* registerMeshSpecification(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& id);
        static MeshRendererSpecification* registerMeshRendererSpecification(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& id);
        static float cubeVertices[];
        static uint32_t cubeIndices[];
        static float plateVertices[];
        static uint32_t plateIndices[];
        static uint32_t cubemapIndices[];
    };
}

#endif