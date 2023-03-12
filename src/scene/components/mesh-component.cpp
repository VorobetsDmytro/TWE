#include "scene/components/mesh-component.hpp"

namespace TWE {
    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, const TextureAttachmentSpecification& textureAtttachments)
    : _registryId(registryId), _modelSpec(modelSpec) {
        create(vertices, vertSize, indices, indSize);
        _texture = std::make_shared<Texture>(textureAtttachments);
    }

    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, Texture* texture)
    : _registryId(registryId), _modelSpec(modelSpec) {
        create(vertices, vertSize, indices, indSize);
        _texture = std::make_shared<Texture>(*texture);
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, const TextureAttachmentSpecification& textureAtttachments)
    : _vao(vao), _vbo(vbo), _ebo(ebo), _registryId(registryId), _modelSpec(modelSpec) {
        _texture = std::make_shared<Texture>(textureAtttachments);
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, Texture* texture)
    : _vao(vao), _vbo(vbo), _ebo(ebo), _registryId(registryId), _modelSpec(modelSpec) {
        _texture = std::make_shared<Texture>(*texture);
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, std::shared_ptr<Texture> texture) 
     : _vao(vao), _vbo(vbo), _ebo(ebo), _registryId(registryId), _texture(texture), _modelSpec(modelSpec) {}

    MeshComponent::MeshComponent(const MeshComponent& mesh) {
        this->_vao = mesh._vao;
        this->_vbo = mesh._vbo;
        this->_ebo = mesh._ebo;
        this->_texture = mesh._texture;
        this->_registryId = mesh._registryId;
        this->_modelSpec = mesh._modelSpec;
    }

    void MeshComponent::setMesh(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, const ModelMeshSpecification& modelSpec) {
        this->_vao = vao;
        this->_vbo = vbo;
        this->_ebo = ebo;
        this->_registryId = registryId;
        this->_modelSpec = modelSpec;
    }

    void MeshComponent::setTexture(const TextureAttachmentSpecification& textureAtttachments) {
        _texture = std::make_shared<Texture>(textureAtttachments);
    }

    void MeshComponent::setTexture(Texture* texture) {
        _texture = std::make_shared<Texture>(*texture);
    }

    void MeshComponent::setTexture(std::shared_ptr<Texture> texture) {
        _texture = texture;
    }

    void MeshComponent::create(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize) {
        _vao = std::make_shared<VAO>();
        _vao->bind();
        _vbo = std::make_shared<VBO>(vertices, vertSize, GL_STATIC_DRAW);
        _vbo->bind();
        _ebo = std::make_shared<EBO>(indices, indSize, GL_STATIC_DRAW);
        _ebo->bind();
        _vao->setAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0, *_vbo.get());
        _vao->setAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)), *_vbo.get());
        _vao->setAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)), *_vbo.get());
        _vao->unbind();
        _vbo->unbind();
        _ebo->unbind();
    }

    std::shared_ptr<VAO> MeshComponent::getVAO() const noexcept { return _vao; }
    std::shared_ptr<VBO> MeshComponent::getVBO() const noexcept { return _vbo; }
    std::shared_ptr<EBO> MeshComponent::getEBO() const noexcept { return _ebo; }
    std::shared_ptr<Texture> MeshComponent::getTexture() const noexcept { return _texture; }
    const std::string& MeshComponent::getRegistryId() const noexcept { return _registryId; }
    const ModelMeshSpecification& MeshComponent::getModelMeshSpecification() const noexcept { return _modelSpec; }
}