#include "renderer/renderer.hpp"

namespace TWE {
    std::vector<LightShaderNamesSpecification> Renderer::lightShaderNames;

    void Renderer::init(int lightShaderNamesSize) {
        lightShaderNames.clear();
        lightShaderNames.resize(lightShaderNamesSize);
        for(int i = 0; i < lightShaderNamesSize; ++i) {
            std::string lightIndex = "lights[]";
            lightIndex.insert(lightIndex.length() - 1, std::to_string(i));
            lightShaderNames[i].pos = lightIndex + ".pos";
            lightShaderNames[i].direction = lightIndex + ".direction";
            lightShaderNames[i].color = lightIndex + ".color";
            lightShaderNames[i].cutOff = lightIndex + ".cutOff";
            lightShaderNames[i].outerCutOff = lightIndex + ".outerCutOff";
            lightShaderNames[i].castShadows = lightIndex + ".castShadows";
            lightShaderNames[i].fadingConstant = lightIndex + ".fading.constant";
            lightShaderNames[i].fadingLinear = lightIndex + ".fading.linear";
            lightShaderNames[i].fadingQuadratic= lightIndex + ".fading.quadratic";
            lightShaderNames[i].typeDir = lightIndex + ".type.dir";
            lightShaderNames[i].typePoint = lightIndex + ".type.point";
            lightShaderNames[i].typeSpot = lightIndex + ".type.spot";
            lightShaderNames[i].shadowMap = lightIndex + ".shadowMap";
            lightShaderNames[i].lightSpaceMat = lightIndex + ".lightSpaceMat";
        }
    }

    void Renderer::render2D(const RendererSpecification& rendererSpec) {
        auto& model = rendererSpec.transformComponent->getModel();
        auto& texture = rendererSpec.meshComponent->getTexture();
        auto& shader = rendererSpec.meshRendererComponent->getShader();
        shader->setUniform("model", model);
        shader->setUniform("hasTexture", !texture->getAttachments().textureSpecifications.empty());
        rendererSpec.meshRendererComponent->updateMaterialUniform();
        rendererSpec.meshComponent->getVAO()->bind();
        texture->bind();
        glDrawElements(GL_TRIANGLES, rendererSpec.meshComponent->getEBO()->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
    }

    void Renderer::render3D(const RendererSpecification& rendererSpec, const glm::vec3& cameraPosition, const glm::mat4& cameraView, const glm::mat4& cameraProjection, 
    const glm::mat4& cameraProjectionView, int lightsCount) {
        auto& model = rendererSpec.transformComponent->getModel();
        auto& shader = rendererSpec.meshRendererComponent->getShader();
        auto texture = rendererSpec.meshComponent->getTexture();
        rendererSpec.meshRendererComponent->updateMatsUniform(model, cameraView, cameraProjection, cameraProjectionView);
        shader->setUniform("viewPos", cameraPosition);
        shader->setUniform("hasTexture", !texture->getAttachments().textureSpecifications.empty());
        shader->setUniform("lightCount", lightsCount);
        shader->setUniform("calculateLight", true);
        rendererSpec.meshRendererComponent->updateMaterialUniform();
        rendererSpec.meshComponent->getVAO()->bind();
        texture->bind();
        glDrawElements(GL_TRIANGLES, rendererSpec.meshComponent->getEBO()->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
    }

    void Renderer::renderScene(Scene* scene) {
        SceneCameraSpecification* camera = scene->getSceneCamera();
        if(!camera->camera)
            return;
        int lightsCount = scene->getLightsCount();
        bool isFocusedOnDebugCamera = scene->getIsFocusedOnDebugCamera();
        static std::vector<RendererSpecification> _2DEntities;
        scene->getRegistry()->view<MeshComponent, MeshRendererComponent, TransformComponent>()
            .each([&](entt::entity entity, MeshComponent& meshComponent, MeshRendererComponent& meshRendererComponent, TransformComponent& transformComponent){
                if(!meshRendererComponent.getIs3D()) {
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

    void Renderer::setLight(MeshRendererComponent& meshRendererComponent, const LightComponent& light, TransformComponent& transform, int lightIndex) {
        if(lightShaderNames.size() <= lightIndex)
            return;
        auto& shader = meshRendererComponent.getShader();
        shader->setUniform(lightShaderNames[lightIndex].pos.c_str(), transform.getPosition());
        shader->setUniform(lightShaderNames[lightIndex].direction.c_str(), transform.getForward());
        shader->setUniform(lightShaderNames[lightIndex].color.c_str(), light.getColor());
        shader->setUniform(lightShaderNames[lightIndex].cutOff.c_str(), glm::cos(glm::radians(light.getInnerRadius())));
        shader->setUniform(lightShaderNames[lightIndex].outerCutOff.c_str(), glm::cos(glm::radians(light.getOuterRadius())));
        shader->setUniform(lightShaderNames[lightIndex].castShadows.c_str(), light.getCastShadows());
        shader->setUniform(lightShaderNames[lightIndex].fadingConstant.c_str(), light.getConstant());
        shader->setUniform(lightShaderNames[lightIndex].fadingLinear.c_str(), light.getLinear());
        shader->setUniform(lightShaderNames[lightIndex].fadingQuadratic.c_str(), light.getQuadratic());
        shader->setUniform(lightShaderNames[lightIndex].typeDir.c_str(), light.getType() == LightType::Dir);
        shader->setUniform(lightShaderNames[lightIndex].typePoint.c_str(), light.getType() == LightType::Point);
        shader->setUniform(lightShaderNames[lightIndex].typeSpot.c_str(), light.getType() == LightType::Spot);
    }

    void Renderer::setShadows(MeshRendererComponent& meshRendererComponent, const glm::mat4& lightSpaceMat, int lightIndex) {
        if(lightShaderNames.size() <= lightIndex)
            return;
        auto& shader = meshRendererComponent.getShader();
        shader->setUniform(lightShaderNames[lightIndex].lightSpaceMat.c_str(), lightSpaceMat);
        shader->setUniform(lightShaderNames[lightIndex].shadowMap.c_str(), 31);
    }

    void Renderer::generateDepthMap(LightComponent& lightComponent, const TransformComponent& transformComponent, const glm::mat4& lightProjection, 
    const glm::mat4& lightView, const glm::mat4& projectionView, Scene* scene) {
        auto fbo = lightComponent.getFBO();
        auto& depthMapSize = fbo->getSize();
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_2D, lightComponent.getDepthTextureId());
        Renderer::setViewport(0, 0, depthMapSize.first, depthMapSize.second);
        glCullFace(GL_FRONT);
        fbo->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderShadowMap(scene, transformComponent.getPosition(), lightView, lightProjection, projectionView);
        fbo->unbind();
        glCullFace(GL_BACK);
    }

    void Renderer::renderShadowMap(Scene* scene, const glm::vec3& position, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& projectionView) {
        scene->getRegistry()->view<MeshComponent, MeshRendererComponent, TransformComponent>()
            .each([&](entt::entity entity, MeshComponent& meshComponent, MeshRendererComponent& meshRendererComponent, TransformComponent& transformComponent){
                if(!meshRendererComponent.getIs3D())
                    return;
                auto& model = transformComponent.getModel();
                auto& shader = meshRendererComponent.getShader();
                shader->use();
                meshRendererComponent.updateMatsUniform(model, view, projection, projectionView);
                shader->setUniform("viewPos", position);
                shader->setUniform("calculateLight", false);
                meshComponent.getVAO()->bind();
                glDrawElements(GL_TRIANGLES, meshComponent.getEBO()->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
            });
    }
}