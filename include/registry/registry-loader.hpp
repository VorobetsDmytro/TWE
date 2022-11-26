#ifndef REGISTRY_LOADER_HPP
#define REGISTRY_LOADER_HPP

#include "registry/registry.hpp"

//DirBehavior
#include "D:\labs\cpp\TWE\scripts\DirBehavior.hpp"
//RotateRGBCube
#include "D:\labs\cpp\TWE\scripts\RotateRGBCube.hpp"
//CameraController
#include "D:\labs\cpp\TWE\scripts\CameraController.hpp"
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
