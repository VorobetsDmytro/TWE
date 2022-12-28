#include "test.hpp"

namespace TWE {
    Test::Test(int wndWidth, int wndHeight, const char* title, GLFWmonitor *monitor, GLFWwindow *share)
    :Engine(wndWidth, wndHeight, title, monitor, share){
        // DLLCreator::initPaths("../../temp");
        
        // gui->setDirectoryRootPath("../../test");
        debugCamera->setPosition({0.f, 1.f, 6.f});

        // SceneSerializer::deserialize(curScene.get(), "../../test/scenes/testscene.scene");
        setVSync(false);

        // RegistryRecorder::recordScript("../../test/registry-loader.hpp", "RotateRGBCube", "../../test/scripts");
        // RegistryRecorder::recordScript("../../test/registry-loader.hpp", "CameraController", "../../test/scripts");
        // RegistryRecorder::recordScript("../../test/registry-loader.hpp", "RGBLight", "../../test/scripts");

        // auto data = DLLCreator::compileScript("RotateRGBCube", "../../test/scripts");
        // auto data2 = DLLCreator::compileScript("DirBehavior", "../../test/scripts");
        // auto data3 = DLLCreator::compileScript("CameraController", "../../test/scripts");
        // auto data4 = DLLCreator::compileScript("RGBLight", "../../test/scripts");
        
        // auto& objEntity = Shape::createCubeEntity(curScene.get(), TextureAttachmentSpecification{ {"../../test/textures/box.png", 0, TextureType::Texture2D, TextureInOutFormat::RGBA} });
        // auto& transformComponentObj = objEntity.getComponent<TransformComponent>();
        // auto& physicsComponentObj = objEntity.addComponent<PhysicsComponent>(curScene->getDynamicWorld(), ColliderType::Box, transformComponentObj.size, 
        //     transformComponentObj.position, transformComponentObj.rotation, 0.f);
        // transformComponentObj.move({0.f, 0.01f, 3.f});
        // physicsComponentObj.setPosition({0.f, 0.01f, 3.f});
        // objEntity.addComponent<ScriptComponent>().bind(scriptRegistry.get("RotateRGBCube"), "RotateRGBCube");
        // scriptRegistry.erase("RotateRGBCube");
        // RegistryLoader::load(scriptRegistry);

        // auto& obj2Entity = Shape::createCubeEntity(curScene.get(), TextureAttachmentSpecification{ {"../../test/textures/ok.png", 0, TextureType::Texture2D, TextureInOutFormat::RGBA} });
        // auto& transformComponentObj2 = obj2Entity.getComponent<TransformComponent>();
        // auto& physicsComponentObj2 = obj2Entity.addComponent<PhysicsComponent>(curScene->getDynamicWorld(), ColliderType::Box, transformComponentObj2.size, 
        //     transformComponentObj2.position, transformComponentObj2.rotation, 0.f);
        // physicsComponentObj2.setPosition({0.9f, 10.f, 0.9f});
        // physicsComponentObj2.setMass(1.f);

        // auto& obj3Entity = Shape::createCubeEntity(curScene.get());
        // auto& transformComponentObj3 = obj3Entity.getComponent<TransformComponent>();
        // auto& physicsComponentObj3 = obj3Entity.addComponent<PhysicsComponent>(curScene->getDynamicWorld(), ColliderType::Box, transformComponentObj3.size, 
        //     transformComponentObj3.position, transformComponentObj3.rotation, 0.f);
        // physicsComponentObj3.setPosition({-1.5f, 1.f, 0.f});
        // transformComponentObj3.move({-1.5f, 1.f, 0.f});
        // transformComponentObj3.scale({0.5f, 0.5f, 0.5f});
        // obj3Entity.addComponent<ScriptComponent>().bind(scriptRegistry.get("RotateRGBCube"), "RotateRGBCube");
        // scriptRegistry.erase("RotateRGBCube");
        // RegistryLoader::load(scriptRegistry);

        // auto& obj4Entity = Shape::createCubeEntity(curScene.get(), TextureAttachmentSpecification{ {"../../test/textures/ok.png", 0, TextureType::Texture2D, TextureInOutFormat::RGBA} });
        // auto& transformComponentObj4 = obj4Entity.getComponent<TransformComponent>();
        // auto& physicsComponentObj4 = obj4Entity.addComponent<PhysicsComponent>(curScene->getDynamicWorld(), ColliderType::Box, transformComponentObj4.size, 
        //     transformComponentObj4.position, transformComponentObj4.rotation, 0.f);
        // physicsComponentObj4.setPosition({1.7f, 11.f, 0.9f});
        // physicsComponentObj4.setMass(1.f);

        // auto& plateEntity = Shape::createPlateEntity(curScene.get());
        // auto& plateTransform = plateEntity.getComponent<TransformComponent>();
        // plateTransform.move({0.f, -0.5f, 0.f});
        // auto& platePhysics = plateEntity.addComponent<PhysicsComponent>(curScene->getDynamicWorld(), ColliderType::Box, 
        //     glm::vec3{plateTransform.size.x * 10.f, plateTransform.size.y * 0.01f, plateTransform.size.z * 10.f}, 
        //     plateTransform.position, plateTransform.rotation, 0.f);
        // platePhysics.setPosition({0.f, -0.5f, 0.f});

        // auto& spotLightEntity = Shape::createSpotLightEntity(curScene.get());
        // auto& transformComponentSL = spotLightEntity.getComponent<TransformComponent>();
        // auto& meshRendererComponentSL = spotLightEntity.getComponent<MeshRendererComponent>();
        // transformComponentSL.move({6.5f, 2.f, -4.5f});
        // transformComponentSL.rotate(45.f, {0.f, -1.f, 0.f});
        // transformComponentSL.rotate(15.f, {1.f, 0.f, 0.f});
        // spotLightEntity.getComponent<LightComponent>().color = {0.8f, 0.25f, 0.25f};

        // auto& spotLight2Entity = Shape::createSpotLightEntity(curScene.get());
        // auto& transformComponent2SL = spotLight2Entity.getComponent<TransformComponent>();
        // transformComponent2SL.move({4.f, 2.f, -4.5f});
        // transformComponent2SL.rotate(25.f, {0.f, -1.f, 0.f});
        // transformComponent2SL.rotate(15.f, {1.f, 0.f, 0.f});

        // auto& pointLightEntity = Shape::createPointLightEntity(curScene.get());
        // auto& transformComponentPL = pointLightEntity.getComponent<TransformComponent>();
        // auto& meshRendererComponentPL = pointLightEntity.getComponent<MeshRendererComponent>();
        // transformComponentPL.move({-2.5f, 0.25f, -1.f});
        // pointLightEntity.getComponent<LightComponent>().color = {0.f, 1.f, 1.f};
        // pointLightEntity.addComponent<ScriptComponent>().bind(scriptRegistry.get("RGBLight"), "RGBLight");

        // auto& dirLightEntity = Shape::createDirLightEntity(curScene.get());
        // auto& transformComponentDL = dirLightEntity.getComponent<TransformComponent>();
        // transformComponentDL.move({5.f, 5.f, 3.f});
        // transformComponentDL.rotate(90.f, {1.f, 0.f, 0.f});
        // dirLightEntity.addComponent<ScriptComponent>().bind(scriptRegistry.get("DirBehavior"), "DirBehavior");
        // scriptRegistry.erase("DirBehavior");
        // RegistryLoader::load(scriptRegistry);

        // ModelLoader mLoader;
        // ModelLoaderData* model = mLoader.loadModel("../../test/models/tophat/TopHat.obj");
        // if(model){
        //     auto& modelEntities = Shape::createModelEntity(curScene.get(), model);
        //     for(auto& modelEntitie : modelEntities) {
        //         auto& transformComponentM = modelEntitie.getComponent<TransformComponent>();
        //         transformComponentM.scale({0.2f, 0.2f, 0.2f});
        //     }
        // }

        // auto& cubemapEntity = Shape::createCubemapEntity(
        //     curScene.get(),
        //     TextureAttachmentSpecification {
        //         {"../../test/textures/cubemap/posx.jpg", 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA},
        //         {"../../test/textures/cubemap/negx.jpg", 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA},
        //         {"../../test/textures/cubemap/posy.jpg", 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA},
        //         {"../../test/textures/cubemap/negy.jpg", 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA},
        //         {"../../test/textures/cubemap/posz.jpg", 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA},
        //         {"../../test/textures/cubemap/negz.jpg", 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA},
        //     }
        // );

        // auto& cameraEntity = Shape::createCameraEntity(curScene.get());
        // auto& transformComponentCam = cameraEntity.getComponent<TransformComponent>();
        // auto& cameraComponentCam = cameraEntity.getComponent<CameraComponent>();
        // cameraEntity.addComponent<ScriptComponent>().bind(scriptRegistry.get("CameraController"), "CameraController");
        // scriptRegistry.erase("CameraController");
        // RegistryLoader::load(scriptRegistry);
        // transformComponentCam.move({0.f, 1.f, 8.f});
        // cameraComponentCam.getSource()->setPerspective(90.f, wndWidth, wndHeight);

        // ModelLoaderData* model = mLoader.loadModel("../../test/models/mountain/Mountain_Final_fbx_.fbx");
        // if(model) {
        //     auto& modelEntities = Shape::createModelEntity(curScene.get(), model);
        //     for(auto& modelEntitie : modelEntities) {
        //         auto& transformComponentM = modelEntitie.getComponent<TransformComponent>();
        //         auto& meshComponentM = modelEntitie.getComponent<MeshComponent>();
        //         transformComponentM.scale({0.1f, 0.1f, 0.1f});
        //         transformComponentM.rotate(-90.f, {1.f, 0.f, 0.f});
        //         // meshComponentM.setTexture("../../test/models/mountain/internal_ground_ao_texture.jpeg", 0);
        //     }
        // }

        // SceneSerializer::serialize(curScene.get(), "../../test/scenes/testscene.json");
    }
}