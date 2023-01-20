#include "scene/components/mesh-component.hpp"

namespace TWE {
    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, const TextureAttachmentSpecification& textureAtttachments)
    : registryId(registryId), modelSpec(modelSpec) {
        create(vertices, vertSize, indices, indSize);
        texture = std::make_shared<Texture>(textureAtttachments);
    }

    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, Texture* texture)
    : registryId(registryId), modelSpec(modelSpec) {
        create(vertices, vertSize, indices, indSize);
        this->texture = std::make_shared<Texture>(*texture);
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, const TextureAttachmentSpecification& textureAtttachments)
    : vao(vao), vbo(vbo), ebo(ebo), registryId(registryId), modelSpec(modelSpec) {
        texture = std::make_shared<Texture>(textureAtttachments);
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, Texture* texture)
    : vao(vao), vbo(vbo), ebo(ebo), registryId(registryId), modelSpec(modelSpec) {
        this->texture = std::make_shared<Texture>(*texture);
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, 
        const ModelMeshSpecification& modelSpec, std::shared_ptr<Texture> texture) 
     : vao(vao), vbo(vbo), ebo(ebo), registryId(registryId), texture(texture), modelSpec(modelSpec) {}

    MeshComponent::MeshComponent(const MeshComponent& mesh) {
        this->vao = mesh.vao;
        this->vbo = mesh.vbo;
        this->ebo = mesh.ebo;
        this->texture = mesh.texture;
        this->registryId = mesh.registryId;
        this->modelSpec = mesh.modelSpec;
    }

    void MeshComponent::setMesh(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, const ModelMeshSpecification& modelSpec) {
        this->vao = vao;
        this->vbo = vbo;
        this->ebo = ebo;
        this->registryId = registryId;
        this->modelSpec = modelSpec;
    }

    void MeshComponent::create(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize) {
        vao = std::make_shared<VAO>();
        vao->bind();
        vbo = std::make_shared<VBO>(vertices, vertSize, GL_STATIC_DRAW);
        vbo->bind();
        ebo = std::make_shared<EBO>(indices, indSize, GL_STATIC_DRAW);
        ebo->bind();
        vao->setAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0, *vbo.get());
        vao->setAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)), *vbo.get());
        vao->setAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)), *vbo.get());
        vao->unbind();
        vbo->unbind();
        ebo->unbind();
    }
}