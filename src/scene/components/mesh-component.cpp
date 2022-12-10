#include "scene/components/mesh-component.hpp"

namespace TWE {
    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::string& registryId, const std::vector<std::string>& texPaths)
    : registryId(registryId) {
        create(vertices, vertSize, indices, indSize);
        int texPathsSize = texPaths.size();
        for(uint32_t i = 0; i < texPathsSize; ++i) {
            TextureAttachmentSpecification attachments;
            attachments.textureSpecifications.push_back({texPaths[i], GL_TEXTURE_2D, i, GL_RGBA});
            textures.push_back(std::make_shared<Texture>(attachments));
        }
    }

    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::string& registryId, const std::vector<Texture*>& texs)
    : registryId(registryId) {
        create(vertices, vertSize, indices, indSize);
        int texsSize = texs.size();
        for(int i = 0; i < texsSize; ++i)
            textures.push_back(std::make_shared<Texture>(*texs[i]));
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, const std::vector<std::string>& texPaths)
    : vao(vao), vbo(vbo), ebo(ebo), registryId(registryId) {
        int texPathsSize = texPaths.size();
        for(uint32_t i = 0; i < texPathsSize; ++i) {
            TextureAttachmentSpecification attachments;
            attachments.textureSpecifications.push_back({texPaths[i], GL_TEXTURE_2D, i, GL_RGBA});
            textures.push_back(std::make_shared<Texture>(attachments));
        }
    }

    MeshComponent::MeshComponent(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId, const std::vector<Texture*>& texs)
    : vao(vao), vbo(vbo), ebo(ebo), registryId(registryId) {
        int texsSize = texs.size();
        for(int i = 0; i < texsSize; ++i)
            textures.push_back(std::make_shared<Texture>(*texs[i]));
    }

    MeshComponent::MeshComponent(const MeshComponent& mesh) {
        this->vao = mesh.vao;
        this->vbo = mesh.vbo;
        this->ebo = mesh.ebo;
        this->textures = mesh.textures;
        this->registryId = mesh.registryId;
    }

    void MeshComponent::setMesh(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, const std::string& registryId) {
        this->vao = vao;
        this->vbo = vbo;
        this->ebo = ebo;
        this->registryId = registryId;
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