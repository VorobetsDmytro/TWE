#include "scene/components/mesh-renderer-component.hpp"

namespace TWE {
    MeshRendererComponent::MeshRendererComponent(const char* vertexShaderPath, const char* fragmentShaderPath) {
        material = std::make_shared<Material>();
        shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
    }

    MeshRendererComponent::MeshRendererComponent(const MeshRendererComponent& meshRendererComponent) {
        this->material = meshRendererComponent.material;
        this->shader = meshRendererComponent.shader;
    }
}