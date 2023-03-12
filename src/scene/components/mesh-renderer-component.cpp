#include "scene/components/mesh-renderer-component.hpp"

namespace TWE {
    MeshRendererComponent::MeshRendererComponent() {}

    MeshRendererComponent::MeshRendererComponent(const char* vertexShaderPath, const char* fragmentShaderPath, int entityId, const std::string& registryId)
    : _registryId(registryId), _entityId(entityId) {
        _shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
        _shader->setUniform("id", entityId);
        _is3D = true;
    }

    MeshRendererComponent::MeshRendererComponent(const MeshRendererComponent& meshRendererComponent) {
        this->_material = meshRendererComponent._material;
        this->_shader = meshRendererComponent._shader;
        this->_registryId = meshRendererComponent._registryId;
        this->_entityId = meshRendererComponent._entityId;
        this->_is3D = meshRendererComponent._is3D;
    }

    void MeshRendererComponent::setShader(const char* vertexShaderPath, const char* fragmentShaderPath, const std::string& registryId) {
        _shader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
        _registryId = registryId;
        _shader->setUniform("id", _entityId);
    }

    void MeshRendererComponent::setShader(std::shared_ptr<Shader> shader) {
        _shader = shader;
    }

    void MeshRendererComponent::setMaterial(const Material& material) {
        _material = material;
    }

    void MeshRendererComponent::setIs3D(bool is3D) {
        _is3D = is3D;
    }

    void MeshRendererComponent::updateMatsUniform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& projectionView) {
        _shader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::MODEL], model);
        _shader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::VIEW], view);
        _shader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::PROJECTION], projection);
        _shader->setUniform(TRANS_MAT_OPTIONS[TransformMatrixOptions::MVP], projectionView * model);
    }

    void MeshRendererComponent::updateMaterialUniform() {
        _shader->setUniform("material.objColor", _material.objColor);
        _shader->setUniform("material.ambient", _material.ambient);
        _shader->setUniform("material.diffuse", _material.diffuse);
        _shader->setUniform("material.specular", _material.specular);
        _shader->setUniform("material.shininess", _material.shininess);
    }

    int MeshRendererComponent::getEntityId() const noexcept { return _entityId; }
    bool MeshRendererComponent::getIs3D() const noexcept { return _is3D; }
    Material& MeshRendererComponent::getMaterial() noexcept { return _material; }
    std::shared_ptr<Shader> MeshRendererComponent::getShader() const noexcept { return _shader; }
    const std::string& MeshRendererComponent::getRegistryId() const noexcept { return _registryId; }
}