#include "model-loader/model-loader-data.hpp"

namespace TWE {
    ModelLoaderData::ModelLoaderData(const std::vector<MeshComponent>& meshComponents, ShaderIndices vert, ShaderIndices frag)
    : meshComponents(meshComponents), vert(vert), frag(frag) {}
}