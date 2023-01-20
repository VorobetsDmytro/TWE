#include "model-loader/model-loader-data.hpp"

namespace TWE {
    ModelLoaderData::ModelLoaderData(const std::vector<MeshComponent>& meshComponents, const std::string& fullPath)
    : meshComponents(meshComponents), fullPath(fullPath) {}
}