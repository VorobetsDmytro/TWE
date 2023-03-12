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
        void updateMatsUniform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& projectionView);
        void setShader(const char* vertexShaderPath, const char* fragmentShaderPath, const std::string& registryId);
        void setShader(std::shared_ptr<Shader> shader);
        void setMaterial(const Material& material);
        void setIs3D(bool is3D);
        [[nodiscard]] int getEntityId() const noexcept;
        [[nodiscard]] bool getIs3D() const noexcept;
        [[nodiscard]] Material& getMaterial() noexcept;
        [[nodiscard]] std::shared_ptr<Shader> getShader() const noexcept;
        [[nodiscard]] const std::string& getRegistryId() const noexcept;
    private:
        Material _material;
        std::shared_ptr<Shader> _shader;
        std::string _registryId;
        int _entityId;
        bool _is3D;
    };
}

#endif