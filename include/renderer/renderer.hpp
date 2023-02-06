#ifndef REDERER_HPP
#define REDERER_HPP

#include <vector>
#include <map>
#include <string>
#include <entt/entt.hpp>

#include "scene/components/components.hpp"
#include "scene/scene.hpp"

#include "renderer/texture.hpp"

namespace TWE {
    struct RendererSpecification {
        RendererSpecification(MeshComponent* meshComponent, MeshRendererComponent* meshRendererComponent, TransformComponent* transformComponent)
            : meshComponent(meshComponent), meshRendererComponent(meshRendererComponent), transformComponent(transformComponent) {}
        MeshComponent* meshComponent;
        MeshRendererComponent* meshRendererComponent;
        TransformComponent* transformComponent;
    };

    class Renderer {
    public:
        static void render2D(const RendererSpecification& rendererSpec);
        static void render3D(const RendererSpecification& rendererSpec, const glm::vec3& cameraPosition, const glm::mat4& cameraView, const glm::mat4& cameraProjection, 
            const glm::mat4& cameraProjectionView, int lightsCount);
        static void renderScene(Scene* scene);
        static void cleanScreen(const glm::vec4& color);
        static void cleanDepth();
        static void setViewport(int startX, int startY, int endX, int endY);
        static void setLight(MeshRendererComponent& meshRendererComponent, const LightComponent& light, TransformComponent& transform, const std::string& lightIndex);
        static void setShadows(MeshRendererComponent& meshRendererComponent, const glm::mat4& lightSpaceMat, const std::string& lightIndex);
        static void generateDepthMap(LightComponent& lightComponent, const TransformComponent& transformComponent, const glm::mat4& lightProjection, 
                                     const glm::mat4& lightView, Scene* scene);
    private:
        static void renderShadowMap(Scene* scene, const glm::vec3& position, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& projectionView);
    };
}

#endif