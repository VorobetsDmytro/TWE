#ifndef MESH_RENDERER_COMPONENT_HPP
#define MESH_RENDERER_COMPONENT_HPP

#include <memory>

#include "renderer/material.hpp"
#include "renderer/shader.hpp"

namespace TWE {
    class MeshRendererComponent {
    public:
        MeshRendererComponent() = default;
        MeshRendererComponent(const char* vertexShaderPath, const char* fragmentShaderPath);
        MeshRendererComponent(const MeshRendererComponent& meshRendererComponent);
        std::shared_ptr<Material> material;
        std::shared_ptr<Shader> shader;
    };
}

#endif