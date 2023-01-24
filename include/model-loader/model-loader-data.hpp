#ifndef MODEL_LOADER_DATA_HPP
#define MODEL_LOADER_DATA_HPP

#include <vector>

#include "scene/components/mesh-component.hpp"

namespace TWE {
    struct ModelLoaderData {
        ModelLoaderData(const std::vector<MeshComponent>& meshComponents, const std::string& fullPath);
        std::vector<MeshComponent> meshComponents;
        std::string fullPath;
    };
}

#endif