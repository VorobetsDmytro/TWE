#include "scene/components/mesh-renderer-component.hpp"

namespace TWE {
    MeshRendererComponent::MeshRendererComponent(const char* vertexShaderPath, const char* fragmentShaderPath) {
        shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
    }

    MeshRendererComponent::MeshRendererComponent(const MeshRendererComponent& meshRendererComponent) {
        this->material = meshRendererComponent.material;
        this->shader = meshRendererComponent.shader;
    }

    void MeshRendererComponent::updateMaterialUniform() {
        shader->setUniform("material.objColor", material.objColor);
        shader->setUniform("material.ambient", material.ambient);
        shader->setUniform("material.diffuse", material.diffuse);
        shader->setUniform("material.specular", material.specular);
        shader->setUniform("material.shininess", material.shininess);
    }
}