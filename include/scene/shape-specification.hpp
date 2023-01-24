#ifndef SHAPE_SPECIFICATION_HPP
#define SHAPE_SPECIFICATION_HPP

#include <memory>
#include <string>
#include <filesystem>

#include "renderer/vao.hpp"
#include "renderer/vbo.hpp"
#include "renderer/ebo.hpp"
#include "renderer/texture.hpp"

#include "scene/components/creation-type-component.hpp"
#include "scene/components/mesh-component.hpp"
#include "registry/registry.hpp"

namespace TWE {
    struct MeshSpecification {
        MeshSpecification() = default;
        MeshSpecification(std::shared_ptr<VAO> vao, std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo, EntityCreationType creationType, 
        const std::string& meshId, const ModelMeshSpecification& modelSpec = {})
            : vao(vao), vbo(vbo), ebo(ebo), creationType(creationType), meshId(meshId), modelSpec(modelSpec) {}
        std::shared_ptr<VAO> vao;
        std::shared_ptr<VBO> vbo;
        std::shared_ptr<EBO> ebo;
        EntityCreationType creationType;
        ModelMeshSpecification modelSpec;
        std::string meshId;
    };

    struct MeshRendererSpecification {
        MeshRendererSpecification() = default;
        MeshRendererSpecification(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
            : vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath) {}
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
    };

    struct ShapeSpecification {
        Registry<MeshSpecification>* meshRegistry;
        Registry<MeshRendererSpecification>* meshRendererRegistry;
        Registry<TextureAttachmentSpecification>* textureRegistry;
        int textureNumber;
        int meshCounter;
        std::filesystem::path rootPath;
    };
}

#endif