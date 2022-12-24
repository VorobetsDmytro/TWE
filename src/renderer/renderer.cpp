#include "renderer/renderer.hpp"

namespace TWE {
    void Renderer::execute(MeshComponent* meshComponent, MeshRendererComponent* meshRendererComponent, TransformComponent* transformComponent, 
    int lightsCount, PhysicsComponent* physicsComponent) {
        meshRendererComponent->shader->use();
        meshRendererComponent->shader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::MODEL], transformComponent->model);
        meshRendererComponent->shader->setUniform("hasTexture", !meshComponent->texture->getAttachments().textureSpecifications.empty());
        meshRendererComponent->shader->setUniform("lightCount", lightsCount);
        meshRendererComponent->updateMaterialUniform();
        meshComponent->vao->bind();
        meshComponent->texture->bind();
        glDrawElements(GL_TRIANGLES, meshComponent->ebo->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
        meshComponent->vao->unbind();
        if(physicsComponent) {
            auto cubeMesh = Shape::meshRegistry->get("Cube mesh");
            if(!cubeMesh) {
                auto& cubeVertices = Shape::getCubeVertices();
                auto& cubeIndices = Shape::getCubeIndices();
                MeshComponent mc = MeshComponent(cubeVertices.first, cubeVertices.second, cubeIndices.first, cubeIndices.second, "Cube mesh");
                cubeMesh = Shape::registerMeshSpecification(mc.vao, mc.vbo, mc.ebo, EntityCreationType::Cube, "", "Cube mesh");
            }
            cubeMesh->vao->bind();
            meshRendererComponent->colliderShader->use();
            auto& model = physicsComponent->getModel();
            meshRendererComponent->colliderShader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::MODEL], model);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(2.f);
            glDrawElements(GL_TRIANGLES, cubeMesh->ebo->getSize() / sizeof(int), GL_UNSIGNED_INT, (void*)0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            cubeMesh->vao->unbind();
        }
    }

    void Renderer::cleanScreen(const glm::vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static const float red[] = { 1, 0, 0, 1 };
        glClearBufferfv(GL_COLOR, 1, red);
    }

    Texture* Renderer::generateCubemapTexture(const std::vector<std::string>& texPaths) {
        if(texPaths.size() != 6) {
            std::cout << "Error loading a cubemap texture.\nTexture paths size has to be 6." << std::endl;
            return nullptr;
        }
        uint32_t id;
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        int width, height, chanInFile;
        int i = 0;
        TextureAttachmentSpecification attachments;
        for(auto& texPath : texPaths){
            auto imgBytes = stbi_load(texPath.c_str(), &width, &height, &chanInFile, 4);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i++, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBytes);
            stbi_image_free(imgBytes);
            TextureSpecification textureSpecification(texPath, 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA);
            textureSpecification.id = id;
            attachments.textureSpecifications.push_back(textureSpecification);
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        Texture* texture = new Texture();
        texture->setAttachments(attachments);
        return texture;
    }

    void Renderer::setViewport(int startX, int startY, int endX, int endY) {
        glViewport(startX, startY, endX, endY);
    }

    void Renderer::setLight(MeshRendererComponent& meshRendererComponent, const LightComponent& light, const TransformComponent& transform, 
    const MeshRendererComponent& meshRenderer, const std::string& lightIndex) {
        meshRendererComponent.shader->setUniform((lightIndex + ".pos").c_str(), transform.position);
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

    void Renderer::setViewPosition(MeshRendererComponent& meshRendererComponent, const glm::vec3& pos) {
        meshRendererComponent.shader->setUniform("viewPos", pos);
    }

    void Renderer::setShadows(MeshRendererComponent& meshRendererComponent, const glm::mat4& lightSpaceMat, const std::string& lightIndex) {
        meshRendererComponent.shader->setUniform((lightIndex + ".lightSpaceMat").c_str(), lightSpaceMat);
        meshRendererComponent.shader->setUniform((lightIndex + ".shadowMap").c_str(), 31);
    }

    void Renderer::generateDepthMap(LightComponent& lightComponent, const TransformComponent& transformComponent, const glm::mat4& lightProjection, 
    const glm::mat4& lightView, Scene* scene) {
        scene->updateView(lightView, lightProjection, transformComponent.position);
        auto& depthMapSize = lightComponent.getDepthMapSize();
        FBO* fbo = lightComponent.getFBO();
        glActiveTexture(GL_TEXTURE31);
        glBindTexture(GL_TEXTURE_2D, lightComponent.getDepthTextureId());
        Renderer::setViewport(0, 0, depthMapSize.first, depthMapSize.second);
        glCullFace(GL_FRONT);
        fbo->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        scene->draw();
        fbo->unbind();
        glCullFace(GL_BACK);
    }
}