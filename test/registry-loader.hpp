#ifndef REGISTRY_LOADER_HPP
#define REGISTRY_LOADER_HPP

#include "registry/registry.hpp"
#include "entity/behavior.hpp"

//DirBehavior
#include "../../test/scripts/DirBehavior.hpp"
//RotateRGBCube
#include "../../test/scripts/RotateRGBCube.hpp"
//CameraController
#include "../../test/scripts/CameraController.hpp"
//IncludeTail

namespace TWE {
    class RegistryLoader {
    public:
        static void load(Registry<Behavior>& registry){
			registry.add<DirBehavior>("DirBehavior");
			registry.add<RotateRGBCube>("RotateRGBCube");
			registry.add<CameraController>("CameraController");
			//InstanceTail
        }
    };
}

#endif