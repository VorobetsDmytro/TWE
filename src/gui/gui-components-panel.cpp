#include "gui/gui-components-panel.hpp"

namespace TWE {
    GUIComponentsPanel::GUIComponentsPanel() {
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".jpeg", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".jpg", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
    }

    void GUIComponentsPanel::showPanel(Entity& entity) {
        ImGui::Begin("Components");
        if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) 
            ImGui::SetWindowFocus();
        if(entity.getSource() != entt::null) {
            showNameComponent(entity);
            showTransformComponent(entity);
            showMeshComponent(entity);
            showMeshRendererComponent(entity);
            showCameraComponent(entity);
            showLightComponent(entity);
            showPhysicsComponent(entity);
            showScriptComponent(entity);
            showAddComponentMenu(entity);
            showFileDialog(entity);
        }
        ImGui::End();
    }

    void GUIComponentsPanel::showFileDialog(Entity& entity) {
        if(ImGuiFileDialog::Instance()->Display("BaseTexture"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                if(entity.hasComponent<MeshComponent>()){
                    auto& meshComponent = entity.getComponent<MeshComponent>();
                    TextureSpecification textureSpecification(filePathName, 0, TextureType::Texture2D, TextureInOutFormat::RGBA);
                    meshComponent.texture->setTexture(textureSpecification);
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
    }

    void GUIComponentsPanel::setScene(Scene* scene) {
        _scene = scene;
    }

    void GUIComponentsPanel::showAddComponentMenu(Entity& entity) {
        ImGui::Dummy({0.f, 5.f});
        std::string popUpId = "Add component";
        if(ImGui::Button("Add component", { ImGui::GetContentRegionAvail().x, 20.f }))
            ImGui::OpenPopup(popUpId.c_str());
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popUpId.c_str())) {
            auto& availSize = ImGui::GetContentRegionAvail();
            if(!entity.hasComponent<MeshComponent>() || !entity.hasComponent<MeshRendererComponent>()) {
                if(ImGui::Button("Mesh component", {availSize.x, 0.f})) {
                    std::string meshId = "Cube mesh";
                    std::string meshRendererId = "Default renderer";
                    auto mesh = Shape::meshRegistry->get(meshId);
                    auto meshRenderer = Shape::meshRendererRegistry->get(meshRendererId);
                    if(mesh && meshRenderer) {
                        auto& creationType = entity.getComponent<CreationTypeComponent>();
                        creationType.setType(EntityCreationType::Cube);
                        if(entity.hasComponent<MeshComponent>())
                            entity.removeComponent<MeshComponent>();
                        if(entity.hasComponent<MeshRendererComponent>())
                            entity.removeComponent<MeshRendererComponent>();
                        entity.addComponent<MeshComponent>(mesh->vao, mesh->vbo, mesh->ebo, meshId);
                        auto& meshRendererComponent = entity.addComponent<MeshRendererComponent>(meshRenderer->vertexShaderPath.c_str(), 
                            meshRenderer->fragmentShaderPath.c_str(), (int)entity.getSource(), meshRendererId);
                        if(entity.hasComponent<PhysicsComponent>())
                            meshRendererComponent.showCollider = true;
                    }      
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!entity.hasComponent<CameraComponent>()) {
                if(ImGui::Button("Camera component", {availSize.x, 0.f})) {
                    auto& cameraComponent = entity.addComponent<CameraComponent>();
                    auto fboSize = _scene->getFrameBuffer()->getSize();
                    cameraComponent.getSource()->setPerspective(90.f, fboSize.first, fboSize.second);
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!entity.hasComponent<LightComponent>()) {
                if(ImGui::Button("Light component", {availSize.x, 0.f})) {
                    entity.addComponent<LightComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!entity.hasComponent<PhysicsComponent>()) {
                if(ImGui::Button("Physics component", {availSize.x, 0.f})) {
                    auto& transformComponent = entity.getComponent<TransformComponent>();
                    auto& physicsComponent = entity.addComponent<PhysicsComponent>(_scene->getDynamicWorld(), ColliderType::Box, 
                        glm::vec3{1.f, 1.f, 1.f}, transformComponent.size, transformComponent.position, transformComponent.rotation, 0.f);
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!entity.hasComponent<ScriptComponent>()) {
                if(ImGui::Button("Script component", {availSize.x, 0.f})) {
                    entity.addComponent<ScriptComponent>().bind<Behavior>();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }

    void GUIComponentsPanel::showNameComponent(Entity& entity) {
        if(entity.hasComponent<NameComponent>()) {
            auto& nameComponent = entity.getComponent<NameComponent>();
            auto& name = nameComponent.getName();
            ImGui::Text("Name");
            ImGui::SameLine();
            if(ImGui::InputText("##Name", &name))
                nameComponent.setName(name);
        }
    }

    void GUIComponentsPanel::showTransformComponent(Entity& entity) {
        if(entity.hasComponent<TransformComponent>()) {
            auto id = (void*)(typeid(TransformComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Transform")) {
                auto& transformComponent = entity.getComponent<TransformComponent>();
                bool hasPhysics = entity.hasComponent<PhysicsComponent>();
                if(GUIComponents::dragFloat3("Position", transformComponent.position, 0.05f, 0.f)) {
                    transformComponent.setPosition(transformComponent.position);
                    if(hasPhysics)
                        entity.getComponent<PhysicsComponent>().setPosition(transformComponent.position);
                }
                auto rotation = transformComponent.rotation * 180.f / PI;
                if(GUIComponents::dragFloat3("Rotation", rotation, hasPhysics ? 1.5f : 0.5f, 0.f)) {
                    rotation *= PI / 180.f;
                    transformComponent.setRotation(rotation);
                    if(hasPhysics)
                        entity.getComponent<PhysicsComponent>().setRotation(rotation);
                }
                if(GUIComponents::dragFloat3("Scale", transformComponent.size, 0.01f, 1.f, 0.01f, 999999.f)) {
                    transformComponent.setSize(transformComponent.size);
                    if(hasPhysics)
                        entity.getComponent<PhysicsComponent>()
                            .setSize(transformComponent.size);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponentsPanel::showMeshComponent(Entity& entity) {
        if(entity.hasComponent<MeshComponent>()) {
            auto id = (void*)(typeid(MeshComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh")) {
                std::string popUpId = "Mesh";
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& meshComponent = entity.getComponent<MeshComponent>();
                std::vector<std::string> meshRegistryKeys = Shape::meshRegistry->getKeys();
                int meshIndex = GUIComponents::combo("Mesh", meshComponent.registryId, meshRegistryKeys);
                if(meshIndex != -1) {
                    std::string meshId = meshRegistryKeys[meshIndex];
                    auto meshSpecification = Shape::meshRegistry->get(meshId);
                    meshComponent.setMesh(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId);
                    meshComponent.setModelPath(meshSpecification->modelPath);
                    entity.getComponent<CreationTypeComponent>().setType(meshSpecification->creationType);
                }
                auto textureId = (void*)(typeid(Texture).hash_code());
                if(ImGui::TreeNodeEx(id, flags, "Texture")) {
                    auto baseTexture = meshComponent.texture->getTextureSpecByTexNumber(0);
                    auto baseDragAndDropFunc = [&]() {
                        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::DirectoryItem].c_str())) {
                            const wchar_t* item = static_cast<const wchar_t*>(payload->Data);
                            std::filesystem::path path(item);
                            TextureSpecification textureSpecification(path.string(), 0, TextureType::Texture2D, TextureInOutFormat::RGBA);
                            meshComponent.texture->setTexture(textureSpecification);
                        }
                    };
                    int baseBtnPressed = GUIComponents::imageButton("Base", baseTexture ? (void*)(uint64_t)baseTexture->id : (void*)(uint64_t)0, { 20.f, 20.f }, baseDragAndDropFunc);
                    if(baseBtnPressed == 0)
                        ImGuiFileDialog::Instance()->OpenDialog("BaseTexture", "Choose File", ".png,.jpeg,.jpg", ".", 1, nullptr);
                    else if(baseBtnPressed == 1)
                        meshComponent.texture->removeTexture(0);
                    ImGui::TreePop();
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        entity.removeComponent<MeshComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponentsPanel::showMeshRendererComponent(Entity& entity) {
        if(entity.hasComponent<MeshRendererComponent>()) {
            auto id = (void*)(typeid(MeshRendererComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh Renderer")) {
                std::string popUpId = "MeshRenderer";
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& meshRendererComponent = entity.getComponent<MeshRendererComponent>();
                std::vector<std::string> meshRendererRegistryKeys = Shape::meshRendererRegistry->getKeys();
                int meshRendererIndex = GUIComponents::combo("Shader", meshRendererComponent.registryId, meshRendererRegistryKeys);
                if(meshRendererIndex != -1) {
                    std::string meshRendererId = meshRendererRegistryKeys[meshRendererIndex];
                    auto meshRendererSpecification = Shape::meshRendererRegistry->get(meshRendererId);
                    meshRendererComponent.setShader(meshRendererSpecification->vertexShaderPath.c_str(), 
                        meshRendererSpecification->fragmentShaderPath.c_str(), meshRendererId);
                }
                if(ImGui::TreeNodeEx(id, flags, "Material")) {
                    GUIComponents::colorEdit3("Color", meshRendererComponent.material.objColor);
                    GUIComponents::dragFloat("Ambient", meshRendererComponent.material.ambient, 0.01f);
                    GUIComponents::dragFloat("Diffuse", meshRendererComponent.material.diffuse, 0.1f, 0.1f, 999999.f);
                    GUIComponents::dragFloat("Specular", meshRendererComponent.material.specular, 0.1f, 0.f, 999999.f);
                    GUIComponents::dragFloat("Shininess", meshRendererComponent.material.shininess, 1.f, 12.f, 999999.f);
                    ImGui::TreePop();
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        entity.removeComponent<MeshRendererComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponentsPanel::showCameraComponent(Entity& entity) {
        if(entity.hasComponent<CameraComponent>()) {
            auto id = (void*)(typeid(CameraComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Camera")) {
                std::string popUpId = "Camera";
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& cameraComponent = entity.getComponent<CameraComponent>();
                auto* cameraSource = cameraComponent.getSource();
                int typeIndex = GUIComponents::combo("Projection", cameraProjectionTypes[cameraSource->getType()], cameraProjectionTypes, 105.f);
                if(typeIndex == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    cameraSource->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                        perspectiveSpecification.wndHeight, perspectiveSpecification.nearDepth, perspectiveSpecification.farDepth);
                } else if(typeIndex == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    cameraSource->setOrthographic(orthographicSpecification.left, orthographicSpecification.right, 
                        orthographicSpecification.bottom, orthographicSpecification.top, orthographicSpecification.nearDepth, orthographicSpecification.farDepth);
                }
                auto isFocusedOn = cameraComponent.isFocusedOn();
                if(GUIComponents::checkBox("Is focused on", isFocusedOn, 105.f))
                    cameraComponent.setFocuse(isFocusedOn);
                if(cameraSource->getType() == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    if(GUIComponents::dragFloat("FOV", perspectiveSpecification.fov, 1.f, 1.f, 140.f, 105.f))
                        cameraSource->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                            perspectiveSpecification.wndHeight, perspectiveSpecification.nearDepth, perspectiveSpecification.farDepth);
                } else if(cameraSource->getType() == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    auto aspect = orthographicSpecification.right;
                    if(GUIComponents::dragFloat("Aspect", aspect, 0.2f, 0.2f, 100.f, 105.f))
                        cameraSource->setOrthographic(-aspect, aspect, -aspect, aspect, 
                            orthographicSpecification.nearDepth, orthographicSpecification.farDepth);
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        entity.removeComponent<CameraComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponentsPanel::showLightComponent(Entity& entity) {
        if(entity.hasComponent<LightComponent>()) {
            auto id = (void*)(typeid(LightComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Light")) {
                std::string popUpId = "Light";
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& lightComponent = entity.getComponent<LightComponent>();
                std::string selectedType = lightTypes[lightComponent.type];
                int typeIndex = GUIComponents::combo("Type", lightTypes[lightComponent.type], lightTypes, 100.f);
                if(typeIndex > -1)
                    lightComponent.setType(static_cast<LightType>(typeIndex));
                GUIComponents::colorEdit3("Color", lightComponent.color, 100.f);
                if(lightComponent.type == LightType::Spot) {
                    GUIComponents::dragFloat("Inner radius", lightComponent.innerRadius, 0.2f, 0.f, 999999.f, 100.f);
                    GUIComponents::dragFloat("Outer radius", lightComponent.outerRadius, 0.2f, 0.f, 999999.f, 100.f);
                }
                if(lightComponent.type == LightType::Spot || lightComponent.type == LightType::Point) {
                    GUIComponents::dragFloat("Constant", lightComponent.constant, 0.05f, 0.01f, 999999.f, 100.f);
                    GUIComponents::dragFloat("Linear", lightComponent.linear, 0.01f, 0.01f, 999999.f, 100.f);
                    GUIComponents::dragFloat("Quadratic", lightComponent.quadratic, 0.001f, 0.001f, 999999.f, 100.f);
                }
                if(lightComponent.type == LightType::Dir) {
                    if(GUIComponents::checkBox("Cast shadows", lightComponent.castShadows, 100.f))
                        lightComponent.setCastShadows(lightComponent.castShadows);
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        entity.removeComponent<LightComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponentsPanel::showPhysicsComponent(Entity& entity) {
        if(entity.hasComponent<PhysicsComponent>()) {
            auto id = (void*)(typeid(PhysicsComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Physics")) {
                std::string popUpId = "Physics";
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& physicsComponent = entity.getComponent<PhysicsComponent>();
                auto mass = physicsComponent.getMass();
                if(GUIComponents::dragFloat("Mass", mass, 0.01f, 0.f, 999999.f))
                    physicsComponent.setMass(mass);
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                auto& shapeDimensions = physicsComponent.getShapeDimensions();
                if(GUIComponents::dragFloat3("Scale", shapeDimensions, 0.01f, 1.f, 0.01f, 999999.f))
                    physicsComponent.setShapeDimensions(shapeDimensions);
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        physicsComponent.getDynamicsWorld()->removeRigidBody(physicsComponent.getRigidBody());
                        entity.removeComponent<PhysicsComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponentsPanel::showScriptComponent(Entity& entity) {
        if(entity.hasComponent<ScriptComponent>()) {
            auto id = (void*)(typeid(ScriptComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Script")) {
                std::string popUpId = "Script";
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& scriptComponent = entity.getComponent<ScriptComponent>();
                GUIComponents::checkBox("Enabled", scriptComponent.isEnabled);
                auto& scriptRegistryKeys = _scene->_scriptDLLRegistry->getKeys();
                int scriptIndex = GUIComponents::combo("Name", scriptComponent.getBehaviorClassName(), scriptRegistryKeys);
                if(scriptIndex != -1) {
                    entity.getComponent<ScriptComponent>().unbind();
                    auto behaviorFactory = DLLCreator::loadDLLFunc(*_scene->_scriptDLLRegistry->get(scriptRegistryKeys[scriptIndex])); 
                    Behavior* behavior = (Behavior*)behaviorFactory();
                    entity.getComponent<ScriptComponent>().bind(behavior, scriptRegistryKeys[scriptIndex]);
                }
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    bool validateScriptFlag = _scene->_sceneState != SceneState::Edit;
                    if(validateScriptFlag) {
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                    }
                    if(ImGui::Button("Validate", { availSize.x, 0.f })) {
                        _scene->validateScript(scriptComponent.getBehaviorClassName());
                        ImGui::CloseCurrentPopup();
                    }
                    if(validateScriptFlag) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleVar();
                    }
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        entity.getComponent<ScriptComponent>().unbind();
                        entity.removeComponent<ScriptComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
        }
    }
}