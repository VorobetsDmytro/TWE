#include "scene/components/mesh-component.hpp"

namespace TWE {
    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize)
    :vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
        create();
    }

    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<std::string>& texPaths)
    :vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
        create();
        int texPathsSize = texPaths.size();
        for(uint32_t i = 0; i < texPathsSize; ++i) {
            TextureAttachmentSpecification attachments;
            attachments.textureSpecifications.push_back({texPaths[i], GL_TEXTURE_2D, i, GL_RGBA});
            textures.push_back(std::make_shared<Texture>(attachments));
        }
    }

    MeshComponent::MeshComponent(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, const std::vector<Texture*>& texs)
    :vertices(vertices), vertSize(vertSize), indices(indices), indSize(indSize) {
        create();
        int texsSize = texs.size();
        for(int i = 0; i < texsSize; ++i)
            textures.push_back(std::make_shared<Texture>(*texs[i]));
    }

    MeshComponent::MeshComponent(const MeshComponent& mesh) {
        this->vertices = mesh.vertices;
        this->vertSize = mesh.vertSize;
        this->indices = mesh.indices;
        this->indSize = mesh.indSize;
        this->vao = mesh.vao;
        this->vbo = mesh.vbo;
        this->ebo = mesh.ebo;
        this->textures = mesh.textures;
    }

    void MeshComponent::create() {
        vao = std::make_shared<VAO>();
        vao->bind();
        vbo = std::make_shared<VBO>(vertices, vertSize, GL_STATIC_DRAW);
        vbo->bind();
        ebo = std::make_shared<EBO>(indices, indSize, GL_STATIC_DRAW);
        ebo->bind();
        vao->setAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0, *vbo.get());
        vao->setAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)), *vbo.get());
        vao->setAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)), *vbo.get());
        vao->unbind();
        vbo->unbind();
        ebo->unbind();
    }
}