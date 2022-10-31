#include "renderer/renderer.hpp"

namespace TWE {
    void Renderer::execute(MeshComponent* meshComponent, MeshRendererComponent* meshRendererComponent, TransformComponent* transformComponent) {
        meshRendererComponent->shader->use();
        meshRendererComponent->shader->setUniform("material.objColor", meshRendererComponent->material->objColor);
        meshRendererComponent->shader->setUniform("material.ambient", meshRendererComponent->material->ambient);
        meshRendererComponent->shader->setUniform("material.diffuse", meshRendererComponent->material->diffuse);
        meshRendererComponent->shader->setUniform("material.specular", meshRendererComponent->material->specular);
        meshRendererComponent->shader->setUniform("material.shininess", meshRendererComponent->material->shininess);
        meshRendererComponent->shader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::MODEL], transformComponent->model);
        meshComponent->vao->bind();
        for(auto& texture : meshComponent->textures)
            texture->bind();
        glDrawElements(GL_TRIANGLES, meshComponent->indSize / sizeof(GLsizei), GL_UNSIGNED_INT, (GLvoid*)0);
        meshComponent->vao->unbind();
    }

    void Renderer::cleanScreen(const glm::vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    GLuint Renderer::generateCubemapTexture(const std::vector<std::string>& texPaths) {
        if(texPaths.size() != 6) {
            std::cout << "Error loading a cubemap texture.\nTexture paths size has to be 6." << std::endl;
            return 0;
        }
        GLuint id;
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
        for(auto& texPath : texPaths){
            auto imgBytes = stbi_load(texPath.c_str(), &width, &height, &chanInFile, 4);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i++, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBytes);
            stbi_image_free(imgBytes);
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        return id;
    }

    void Renderer::setViewport(int startX, int startY, int endX, int endY) {
        glViewport(startX, startY, endX, endY);
    }
}