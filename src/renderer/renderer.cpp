#include "renderer/renderer.hpp"

namespace TWE {
    void Renderer::render2D(const RendererSpecification& rendererSpec) {
        auto& model = rendererSpec.transformComponent->getModel();
        rendererSpec.meshRendererComponent->shader->setUniform("model", model);
        rendererSpec.meshRendererComponent->shader->setUniform("hasTexture", !rendererSpec.meshComponent->texture->getAttachments().textureSpecifications.empty());
        rendererSpec.meshRendererComponent->updateMaterialUniform();
        rendererSpec.meshComponent->vao->bind();
        rendererSpec.meshComponent->texture->bind();
        glDrawElements(GL_TRIANGLES, rendererSpec.meshComponent->ebo->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
    }

    void Renderer::render3D(const RendererSpecification& rendererSpec, const glm::vec3& cameraPosition, const glm::mat4& cameraView, const glm::mat4& cameraProjection, 
    const glm::mat4& cameraProjectionView, int lightsCount) {
        auto& model = rendererSpec.transformComponent->getModel();
        rendererSpec.meshRendererComponent->updateMatsUniform(model, cameraView, cameraProjection, cameraProjectionView);
        rendererSpec.meshRendererComponent->shader->setUniform("viewPos", cameraPosition);
        rendererSpec.meshRendererComponent->shader->setUniform("hasTexture", !rendererSpec.meshComponent->texture->getAttachments().textureSpecifications.empty());
        rendererSpec.meshRendererComponent->shader->setUniform("lightCount", lightsCount);
        rendererSpec.meshRendererComponent->shader->setUniform("calculateLight", true);
        rendererSpec.meshRendererComponent->updateMaterialUniform();
        rendererSpec.meshComponent->vao->bind();
        rendererSpec.meshComponent->texture->bind();
        glDrawElements(GL_TRIANGLES, rendererSpec.meshComponent->ebo->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
    }

    void Renderer::renderScene(Scene* scene) {
        SceneCameraSpecification* camera = scene->getSceneCamera();
        if(!camera->camera)
            return;
        int lightsCount = scene->getLightsCount();
        bool isFocusedOnDebugCamera = scene->getIsFocusedOnDebugCamera();
        std::vector<RendererSpecification> _2DEntities;
        scene->getRegistry()->view<MeshComponent, MeshRendererComponent, TransformComponent>()
            .each([&](entt::entity entity, MeshComponent& meshComponent, MeshRendererComponent& meshRendererComponent, TransformComponent& transformComponent){
                if(!meshRendererComponent.is3D) {
                    if(!isFocusedOnDebugCamera)
                        _2DEntities.push_back({&meshComponent, &meshRendererComponent, &transformComponent});
                    return;
                }
                render3D({&meshComponent, &meshRendererComponent, &transformComponent}, camera->position, camera->view, camera->projection, camera->projectionView, lightsCount);
            });
        if(isFocusedOnDebugCamera) {
            #ifndef TWE_BUILD
            scene->getSceneStateSpecification()->physics.getDebugDrawer()->setMats(camera->view, camera->projection);
            scene->getSceneStateSpecification()->physics.debugDrawWorld();
            #endif
            return;
        }
        cleanDepth();
        for(auto& rendererSpec : _2DEntities)
            render2D(rendererSpec);
        _2DEntities.clear();
    }

    void Renderer::cleanScreen(const glm::vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static const float red[] = { 1, 0, 0, 1 };
        glClearBufferfv(GL_COLOR, 1, red);
    }

    void Renderer::cleanDepth() {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::setViewport(int startX, int startY, int endX, int endY) {
        glViewport(startX, startY, endX, endY);
    }

    void Renderer::setLight(MeshRendererComponent& meshRendererComponent, const LightComponent& light, TransformComponent& transform, const std::string& lightIndex) {
        meshRendererComponent.shader->setUniform((lightIndex + ".pos").c_str(), transform.transform.position);
        meshRendererComponent.shader->setUniform((lightIndex + ".direction").c_str(), transform.getForward());
        meshRendererComponent.shader->setUniform((lightIndex + ".color").c_str(), light.color);
        meshRendererComponent.shader->setUniform((lightIndex + ".cutOff").c_str(), glm::cos(glm::radians(light.innerRadius)));
        meshRendererComponent.shader->setUniform((lightIndex + ".outerCutOff").c_str(), glm::cos(glm::radians(light.outerRadius)));
        meshRendererComponent.shader->setUniform((lightIndex + ".castShadows").c_str(), light.castShadows);
        meshRendererComponent.shader->setUniform((lightIndex + ".fading.constant").c_str(), light.constant);
        meshRendererComponent.shader->setUniform((lightIndex + ".fading.linear").c_str(), light.linear);
        meshRendererComponent.shader->setUniform((lightIndex + ".fading.quadratic").c_str(), light.quadratic);
        for(auto& type : lightTypes)
            meshRendererComponent.shader->setUniform((lightIndex + ".type." + type).c_str(), lightTypes[light.type] == type);
    }

    void Renderer::setShadows(MeshRendererComponent& meshRendererComponent, const glm::mat4& lightSpaceMat, const std::string& lightIndex) {
        meshRendererComponent.shader->setUniform((lightIndex + ".lightSpaceMat").c_str(), lightSpaceMat);
        meshRendererComponent.shader->setUniform((lightIndex + ".shadowMap").c_str(), 31);
    }

    void Renderer::generateDepthMap(LightComponent& lightComponent, const TransformComponent& transformComponent, const glm::mat4& lightProjection, 
    const glm::mat4& lightView, Scene* scene) {
        auto fbo = lightComponent.getFBO();
        auto& depthMapSize = fbo->getSize();
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_2D, lightComponent.getDepthTextureId());
        Renderer::setViewport(0, 0, depthMapSize.first, depthMapSize.second);
        glCullFace(GL_FRONT);
        fbo->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderShadowMap(scene, transformComponent.transform.position, lightView, lightProjection, lightProjection * lightView);
        fbo->unbind();
        glCullFace(GL_BACK);
    }

    void Renderer::renderShadowMap(Scene* scene, const glm::vec3& position, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& projectionView) {
        scene->getRegistry()->view<MeshComponent, MeshRendererComponent, TransformComponent>()
            .each([&](entt::entity entity, MeshComponent& meshComponent, MeshRendererComponent& meshRendererComponent, TransformComponent& transformComponent){
                if(!meshRendererComponent.is3D)
                    return;
                auto& model = transformComponent.getModel();
                meshRendererComponent.shader->use();
                meshRendererComponent.updateMatsUniform(model, view, projection, projectionView);
                meshRendererComponent.shader->setUniform("viewPos", position);
                meshRendererComponent.shader->setUniform("calculateLight", false);
                meshComponent.vao->bind();
                glDrawElements(GL_TRIANGLES, meshComponent.ebo->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
            });
    }
}