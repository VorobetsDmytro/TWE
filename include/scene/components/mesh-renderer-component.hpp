#ifndef MESH_RENDERER_COMPONENT_HPP
#define MESH_RENDERER_COMPONENT_HPP

#include <memory>
#include "string"

#include "renderer/material.hpp"
#include "renderer/shader.hpp"

namespace TWE {
    class MeshRendererComponent {
    public:
        MeshRendererComponent();
        MeshRendererComponent(const char* vertexShaderPath, const char* fragmentShaderPath, int entityId, const std::string& registryId);
        MeshRendererComponent(const MeshRendererComponent& meshRendererComponent);
        void updateMaterialUniform();
        void setShader(const char* vertexShaderPath, const char* fragmentShaderPath, const std::string& registryId);
        Material material;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Shader> colliderShader;
        std::string registryId;
        int entityId;
        bool showCollider;
    };
}

#endif