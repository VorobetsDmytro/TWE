#include "gui/gui-components-panel.hpp"

namespace TWE {
    GUIComponentsPanel::GUIComponentsPanel() {
        _guiState = nullptr;
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".jpeg", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".jpg", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".mp3", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
    }

    void GUIComponentsPanel::setGUIState(GUIStateSpecification* guiState) {
        _guiState = guiState;
    }

    void GUIComponentsPanel::showPanel() {
        ImGui::Begin("Components");
        if(!_guiState || !_guiState->bgFuncsInRun.empty()) {
            ImGui::End();
            return;
        }
        if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) 
            ImGui::SetWindowFocus();
        if(_guiState->selectedEntity.getSource() != entt::null) {
            showNameComponent();
            showTransformComponent();
            showMeshComponent();
            showMeshRendererComponent();
            showCameraComponent();
            showLightComponent();
            showPhysicsComponent();
            showScriptComponent();
            showAudioComponent();
            showAddComponentMenu();
            showFileDialog();
        }
        ImGui::End();
    }

    void GUIComponentsPanel::showFileDialog() {
        if(ImGuiFileDialog::Instance()->Display("AddAudioSource")) {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                if(_guiState->selectedEntity.hasComponent<AudioComponent>()){
                    auto& audioComponent = _guiState->selectedEntity.getComponent<AudioComponent>();
                    std::filesystem::path soundPath = filePathName;
                    if(soundPath.extension() == ".mp3")
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddSoundSourceAudioComponentCommand(_guiState->selectedEntity, soundPath));
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("BaseTexture")) {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                if(_guiState->selectedEntity.hasComponent<MeshComponent>()){
                    auto& meshComponent = _guiState->selectedEntity.getComponent<MeshComponent>();
                    auto newState = meshComponent;
                    TextureSpecification textureSpecification(filePathName, 0, TextureType::Texture2D, TextureInOutFormat::RGBA);
                    TextureAttachmentSpecification* textureInRegistry = nullptr;
                    auto values = Shape::shapeSpec->textureRegistry->getValues();
                    for(auto& value : values) {
                        if(value->textureSpecifications.empty())
                            continue;
                        if(value->textureSpecifications[0] == textureSpecification) {
                            textureInRegistry = value;
                            break;
                        }
                    }
                    if(!textureInRegistry) {
                        Texture* texture = new Texture({textureSpecification});
                        auto* textureAtttachments = &texture->getAttachments();
                        textureInRegistry = Shape::shapeSpec->textureRegistry->add("Texture-" + std::to_string(Shape::shapeSpec->textureNumber++), textureAtttachments);
                    }
                    Texture* texture = new Texture();
                    texture->setAttachments(*textureInRegistry);
                    newState.setTexture(texture);
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshComponentStateCommand(_guiState->selectedEntity, newState));
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("CubemapTexture")) {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                auto& selection = ImGuiFileDialog::Instance()->GetSelection();
                if(selection.size() == 6) {
                    static std::vector<std::string> positions = {
                        "posx", "negx",
                        "posy", "negy",
                        "posz", "negz",
                    };
                    TextureAttachmentSpecification attachemnts;
                    for(auto& item : positions)
                        for(auto& [key, value] : selection) {
                            auto position = std::filesystem::path(key).stem().string();
                            if(item == position) {
                                TextureSpecification specification(value, 0, TextureType::CubemapTexture, TextureInOutFormat::RGBA);
                                attachemnts.textureSpecifications.push_back(specification);
                                break;
                            }
                        }
                    if(attachemnts.textureSpecifications.size() == 6) {
                        if(_guiState->selectedEntity.hasComponent<MeshComponent>()){
                            auto& meshComponent = _guiState->selectedEntity.getComponent<MeshComponent>();
                            auto newState = meshComponent;
                            TextureAttachmentSpecification* textureInRegistry = nullptr;
                            auto values = Shape::shapeSpec->textureRegistry->getValues();
                            for(auto& value : values) {
                                if(value->textureSpecifications.empty())
                                    continue;
                                if(*value == attachemnts) {
                                    textureInRegistry = value;
                                    break;
                                }
                            }
                            if(!textureInRegistry) {
                                Texture* texture = new Texture(attachemnts);
                                auto* textureAtttachments = &texture->getAttachments();
                                textureInRegistry = Shape::shapeSpec->textureRegistry->add("Texture-" + std::to_string(Shape::shapeSpec->textureNumber++), textureAtttachments);
                            }
                            Texture* texture = new Texture();
                            texture->setAttachments(*textureInRegistry);
                            newState.setTexture(texture);
                            _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshComponentStateCommand(_guiState->selectedEntity, newState));
                        }
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
    }

    void GUIComponentsPanel::showAddComponentMenu() {
        ImGui::Dummy({0.f, 5.f});
        std::string popUpId = guiPopups[GUIPopupIds::AddComponentPopup];
        if(ImGui::Button("Add component", { ImGui::GetContentRegionAvail().x, 20.f }))
            ImGui::OpenPopup(popUpId.c_str());
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popUpId.c_str())) {
            auto& availSize = ImGui::GetContentRegionAvail();
            if(!_guiState->selectedEntity.hasComponent<MeshComponent>() || !_guiState->selectedEntity.hasComponent<MeshRendererComponent>()) {
                if(ImGui::Button("Mesh component", {availSize.x, 0.f})) {
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddMeshComponentCommand(_guiState->selectedEntity));
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!_guiState->selectedEntity.hasComponent<CameraComponent>()) {
                if(ImGui::Button("Camera component", {availSize.x, 0.f})) {
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddCameraComponentCommand(_guiState->selectedEntity, _guiState->window->getFrameBuffer()));
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!_guiState->selectedEntity.hasComponent<LightComponent>()) {
                if(ImGui::Button("Light component", {availSize.x, 0.f})) {
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddLightComponentCommand(_guiState->selectedEntity));
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!_guiState->selectedEntity.hasComponent<PhysicsComponent>()) {
                if(ImGui::Button("Physics component", {availSize.x, 0.f})) {
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddPhysicsComponentCommand(_guiState->selectedEntity));
                    ImGui::CloseCurrentPopup();
                }
            }
            if(!_guiState->selectedEntity.hasComponent<ScriptComponent>()) {
                if(ImGui::Button("Script component", {availSize.x, 0.f})) {
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddScriptComponentCommand(_guiState->selectedEntity));
                    ImGui::CloseCurrentPopup();
                }
            }
             if(!_guiState->selectedEntity.hasComponent<AudioComponent>()) {
                if(ImGui::Button("Audio component", {availSize.x, 0.f})) {
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new AddAudioComponentCommand(_guiState->selectedEntity));
                    ImGui::CloseCurrentPopup();
                }
             }
            ImGui::EndPopup();
        }
    }

    void GUIComponentsPanel::showNameComponent() {
        if(_guiState->selectedEntity.hasComponent<NameComponent>()) {
            auto& nameComponent = _guiState->selectedEntity.getComponent<NameComponent>();
            auto& name = nameComponent.getName();
            ImGui::Text("Name");
            ImGui::SameLine();
            if(ImGui::InputText("##Name", &name)) {
                auto newState = nameComponent;
                newState.setName(name);
                _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeNameComponentStateCommand(_guiState->selectedEntity, newState));
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showTransformComponent() {
        if(_guiState->selectedEntity.hasComponent<TransformComponent>()) {
            auto id = (void*)(typeid(TransformComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Transform")) {
                auto& transformComponent = _guiState->selectedEntity.getComponent<TransformComponent>();
                auto position = transformComponent.getPosition();
                static bool addToURControl = false;
                static TransformComponent oldState;
                static TransformComponent newState;
                if(!addToURControl)
                    oldState = transformComponent;
                if(GUIComponents::dragFloat3("Position", position, 0.05f, 0.f)) {
                    addToURControl = true;
                    transformComponent.setPosition(position);
                    newState = transformComponent;
                }
                auto rotation = transformComponent.getRotation() * 180.f / PI;
                if(GUIComponents::dragFloat3("Rotation", rotation, 0.5f, 0.f)) {
                    addToURControl = true;
                    rotation *= PI / 180.f;
                    transformComponent.setRotation(rotation);
                    newState = transformComponent;
                }
                auto size = transformComponent.getSize();
                if(GUIComponents::dragFloat3("Scale", size, 0.01f, 1.f, 0.01f, 999999.f)) {
                    addToURControl = true;
                    transformComponent.setSize(size);
                    newState = transformComponent;
                }
                if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
                    addToURControl = false;
                    if(oldState.getTransform() != newState.getTransform())
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeTransformComponentState(_guiState->selectedEntity, oldState, newState));
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showSeparator() {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.f);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.2f);
        ImGui::Separator();
        ImGui::PopStyleVar(1);
    }

    void GUIComponentsPanel::showMeshComponent() {
        if(_guiState->selectedEntity.hasComponent<MeshComponent>()) {
            auto id = (void*)(typeid(MeshComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh")) {
                std::string popUpId = guiPopups[GUIPopupIds::MeshPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& meshComponent = _guiState->selectedEntity.getComponent<MeshComponent>();
                std::vector<std::string> meshRegistryKeys = Shape::shapeSpec->meshRegistry->getKeys();
                std::string selectedMesh = meshComponent.getRegistryId();
                int meshIndex = GUIComponents::combo("Mesh", selectedMesh, meshRegistryKeys);
                if(meshIndex != -1) {
                    auto newState = meshComponent;
                    std::string meshId = meshRegistryKeys[meshIndex];
                    auto meshSpecification = Shape::shapeSpec->meshRegistry->get(meshId);
                    newState.setMesh(meshSpecification->vao, meshSpecification->vbo, meshSpecification->ebo, meshId, meshSpecification->modelSpec);
                    _guiState->selectedEntity.getComponent<CreationTypeComponent>().setType(meshSpecification->creationType);
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshComponentStateCommand(_guiState->selectedEntity, newState));
                }
                auto textureId = (void*)(typeid(Texture).hash_code());
                bool isCubemap = false;
                if(_guiState->selectedEntity.hasComponent<MeshRendererComponent>()) {
                    auto& meshRendererComponent = _guiState->selectedEntity.getComponent<MeshRendererComponent>();
                    if(meshRendererComponent.getRegistryId() == "Cubemap renderer")
                        isCubemap = true;
                }
                if(ImGui::TreeNodeEx(id, flags, "Texture")) {
                    auto baseTexture = meshComponent.getTexture()->getTextureSpecByTexNumber(0);
                    std::function<void()> baseDragAndDropFunc;
                    if(isCubemap)
                        baseDragAndDropFunc = [](){};
                    else
                        baseDragAndDropFunc = [&]() {
                            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::DirectoryItem].c_str())) {
                                auto newState = meshComponent;
                                const wchar_t* item = static_cast<const wchar_t*>(payload->Data);
                                std::filesystem::path path(item);
                                TextureSpecification textureSpecification(path.string(), 0, TextureType::Texture2D, TextureInOutFormat::RGBA);
                                TextureAttachmentSpecification* textureInRegistry = nullptr;
                                auto values = Shape::shapeSpec->textureRegistry->getValues();
                                for(auto& value : values) {
                                    if(value->textureSpecifications.empty())
                                        continue;
                                    if(value->textureSpecifications[0] == textureSpecification) {
                                        textureInRegistry = value;
                                        break;
                                    }
                                }
                                if(!textureInRegistry) {
                                    Texture* texture = new Texture({textureSpecification});
                                    auto* textureAtttachments = &texture->getAttachments();
                                    textureInRegistry = Shape::shapeSpec->textureRegistry->add("Texture-" + std::to_string(Shape::shapeSpec->textureNumber++), textureAtttachments);
                                }
                                Texture* texture = new Texture();
                                texture->setAttachments(*textureInRegistry);
                                newState.setTexture(texture);
                                _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshComponentStateCommand(_guiState->selectedEntity, newState));
                            }
                        };
                    int baseBtnPressed = GUIComponents::imageButton("Base", baseTexture ? (void*)(uint64_t)baseTexture->id : (void*)(uint64_t)0, { 20.f, 20.f }, baseDragAndDropFunc);
                    if(baseBtnPressed == 0)
                        if(isCubemap) {
                            ImGuiFileDialog::Instance()->OpenDialog("CubemapTexture", "Choose File", ".png,.jpeg,.jpg", 
                                (_guiState->scene->getProjectData()->rootPath.string() + '/').c_str(), 6, nullptr);
                        } else
                            ImGuiFileDialog::Instance()->OpenDialog("BaseTexture", "Choose File", ".png,.jpeg,.jpg", 
                                (_guiState->scene->getProjectData()->rootPath.string() + '/').c_str(), 1, nullptr);
                    else if(baseBtnPressed == 1) {
                        auto newState = meshComponent;
                        if(isCubemap) {
                            int size = meshComponent.getTexture()->getAttachments().textureSpecifications.size();
                            newState.setTexture(meshComponent.getTexture()->getAttachments());
                            for(int i = 0; i < size; ++i)
                                newState.getTexture()->removeTexture(0);
                            _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshComponentStateCommand(_guiState->selectedEntity, newState));
                        } else {
                            newState.setTexture(meshComponent.getTexture()->getAttachments());
                            newState.getTexture()->removeTexture(0);
                            _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshComponentStateCommand(_guiState->selectedEntity, newState));
                        }
                    }
                    ImGui::TreePop();
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveMeshComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showMeshRendererComponent() {
        if(_guiState->selectedEntity.hasComponent<MeshRendererComponent>()) {
            auto id = (void*)(typeid(MeshRendererComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh Renderer")) {
                std::string popUpId = guiPopups[GUIPopupIds::MeshRendererPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& meshRendererComponent = _guiState->selectedEntity.getComponent<MeshRendererComponent>();
                auto& material = meshRendererComponent.getMaterial();
                std::vector<std::string> meshRendererRegistryKeys = Shape::shapeSpec->meshRendererRegistry->getKeys();
                std::string registryId = meshRendererComponent.getRegistryId();
                int meshRendererIndex = GUIComponents::combo("Shader", registryId, meshRendererRegistryKeys);
                if(meshRendererIndex != -1) {
                    auto newState = meshRendererComponent;
                    std::string meshRendererId = meshRendererRegistryKeys[meshRendererIndex];
                    auto meshRendererSpecification = Shape::shapeSpec->meshRendererRegistry->get(meshRendererId);
                    newState.setShader(meshRendererSpecification->vertexShaderPath.c_str(), 
                        meshRendererSpecification->fragmentShaderPath.c_str(), meshRendererId);
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshRendererComponentState(_guiState->selectedEntity, meshRendererComponent, newState));
                }
                static bool addToURControl = false;
                static MeshRendererComponent oldState;
                static MeshRendererComponent newState;
                if(!addToURControl)
                    oldState = meshRendererComponent;
                if(ImGui::TreeNodeEx(id, flags, "Material")) {
                    glm::vec3 color = material.objColor;
                    if(GUIComponents::colorEdit3("Color", color)) {
                        addToURControl = true;
                        material.objColor = color;
                        newState = meshRendererComponent;
                    }
                    float ambient = material.ambient;
                    if(GUIComponents::dragFloat("Ambient", ambient, 0.01f)) {
                        addToURControl = true;
                        material.ambient = ambient;
                        newState = meshRendererComponent;
                    }
                    float diffuse = material.diffuse;
                    if(GUIComponents::dragFloat("Diffuse", diffuse, 0.1f, 0.1f, 999999.f)) {
                        addToURControl = true;
                        material.diffuse = diffuse;
                        newState = meshRendererComponent;
                    }
                    float specular = material.specular;
                    if(GUIComponents::dragFloat("Specular", specular, 0.1f, 0.f, 999999.f)) {
                        addToURControl = true;
                        material.specular = specular;
                        newState = meshRendererComponent;
                    }
                    float shininess = material.shininess;
                    if(GUIComponents::dragFloat("Shininess", shininess, 1.f, 12.f, 999999.f)) {
                        addToURControl = true;
                        material.shininess = shininess;
                        newState = meshRendererComponent;
                    }
                    auto is3D = meshRendererComponent.getIs3D();
                    if(GUIComponents::checkBox("Is 3D", is3D)) {
                        meshRendererComponent.setIs3D(is3D);
                        newState = meshRendererComponent;
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshRendererComponentState(_guiState->selectedEntity, oldState, newState));
                    }
                    if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
                        addToURControl = false;
                        if(oldState.getMaterial() != newState.getMaterial())
                            _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeMeshRendererComponentState(_guiState->selectedEntity, oldState, newState));
                    }
                    ImGui::TreePop();
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveMeshRendererComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showCameraComponent() {
        if(_guiState->selectedEntity.hasComponent<CameraComponent>()) {
            auto id = (void*)(typeid(CameraComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Camera")) {
                std::string popUpId = guiPopups[GUIPopupIds::CameraPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& cameraComponent = _guiState->selectedEntity.getComponent<CameraComponent>();
                auto* cameraSource = cameraComponent.getSource();
                static bool addToURControl = false;
                static CameraComponent oldState;
                static CameraComponent newState;
                if(!addToURControl)
                    oldState = cameraComponent;
                int typeIndex = GUIComponents::combo("Projection", cameraProjectionTypes[cameraSource->getType()], cameraProjectionTypes, 105.f);
                if(typeIndex == CameraProjectionType::Perspective) {
                    addToURControl = true;
                    auto& perspectiveSpecification = cameraComponent.getSource()->getPerspectiveSpecification();
                    cameraComponent.getSource()->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                        perspectiveSpecification.wndHeight, perspectiveSpecification.nearDepth, perspectiveSpecification.farDepth);
                    newState = cameraComponent;
                } else if(typeIndex == CameraProjectionType::Orthographic) {
                    addToURControl = true;
                    auto& orthographicSpecification = cameraComponent.getSource()->getOrthographicSpecification();
                    cameraComponent.getSource()->setOrthographic(orthographicSpecification.left, orthographicSpecification.right, 
                        orthographicSpecification.bottom, orthographicSpecification.top, orthographicSpecification.nearDepth, 
                        orthographicSpecification.farDepth);
                    newState = cameraComponent;
                }
                auto isFocusedOn = cameraComponent.isFocusedOn();
                if(GUIComponents::checkBox("Is focused on", isFocusedOn, 105.f)) {
                    cameraComponent.setFocuse(isFocusedOn);
                    newState = cameraComponent;
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeCameraComponentStateCommand(_guiState->selectedEntity, oldState, newState));
                }
                if(cameraSource->getType() == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    float fov = perspectiveSpecification.fov;
                    if(GUIComponents::dragFloat("FOV", fov, 1.f, 1.f, 140.f, 105.f)) {
                        addToURControl = true;
                        cameraComponent.getSource()->setPerspective(fov, perspectiveSpecification.wndWidth, 
                            perspectiveSpecification.wndHeight, perspectiveSpecification.nearDepth, perspectiveSpecification.farDepth);
                        newState = cameraComponent;
                    }
                } else if(cameraSource->getType() == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    auto aspect = orthographicSpecification.right;
                    if(GUIComponents::dragFloat("Aspect", aspect, 0.2f, 0.2f, 100.f, 105.f)) {
                        addToURControl = true;
                        cameraComponent.getSource()->setOrthographic(-aspect, aspect, -aspect, aspect, 
                            orthographicSpecification.nearDepth, orthographicSpecification.farDepth);
                        newState = cameraComponent;
                    }
                }
                if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
                    addToURControl = false;
                    if(*oldState.getSource() != *newState.getSource())
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeCameraComponentStateCommand(_guiState->selectedEntity, oldState, newState));
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveCameraComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showLightComponent() {
        if(_guiState->selectedEntity.hasComponent<LightComponent>()) {
            auto id = (void*)(typeid(LightComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Light")) {
                std::string popUpId = guiPopups[GUIPopupIds::LightPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& lightComponent = _guiState->selectedEntity.getComponent<LightComponent>();
                auto lightType = lightComponent.getType();
                std::string selectedType = lightTypes[lightType];
                static bool addToURControl = false;
                static LightComponent oldState;
                static LightComponent newState;
                if(!addToURControl)
                    oldState = lightComponent;
                int typeIndex = GUIComponents::combo("Type", lightTypes[lightType], lightTypes, 120.f);
                if(typeIndex > -1) {
                    addToURControl = true;
                    lightComponent.setType(static_cast<LightType>(typeIndex));
                    newState = lightComponent;
                }
                glm::vec3 color = lightComponent.getColor();
                if(GUIComponents::colorEdit3("Color", color, 120.f)) {
                    addToURControl = true;
                    lightComponent.setColor(color);
                    newState = lightComponent;
                }
                if(lightType == LightType::Spot) {
                    float innderRadius = lightComponent.getInnerRadius();
                    if(GUIComponents::dragFloat("Inner radius", innderRadius, 0.2f, 0.f, 999999.f, 120.f)) {
                        addToURControl = true;
                        lightComponent.setInnerRadius(innderRadius);
                        newState = lightComponent;
                    }
                    float outerRadius = lightComponent.getOuterRadius();
                    if(GUIComponents::dragFloat("Outer radius", outerRadius, 0.2f, 0.f, 999999.f, 120.f)) {
                        addToURControl = true;
                        lightComponent.setOuterRadius(outerRadius);
                        newState = lightComponent;
                    }
                }
                if(lightType == LightType::Spot || lightType == LightType::Point) {
                    float constant = lightComponent.getConstant();
                    if(GUIComponents::dragFloat("Constant", constant, 0.05f, 0.01f, 999999.f, 120.f)) {
                        addToURControl = true;
                        lightComponent.setConstant(constant);
                        newState = lightComponent;
                    }
                    float linear = lightComponent.getLinear();
                    if(GUIComponents::dragFloat("Linear", linear, 0.01f, 0.01f, 999999.f, 120.f)) {
                        addToURControl = true;
                        lightComponent.setLinear(linear);
                        newState = lightComponent;
                    }
                    float quadratic = lightComponent.getQuadratic();
                    if(GUIComponents::dragFloat("Quadratic", quadratic, 0.001f, 0.001f, 999999.f, 120.f)) {
                        addToURControl = true;
                        lightComponent.setQuadratic(quadratic);
                        newState = lightComponent;
                    }
                }
                if(lightType == LightType::Dir) {
                    bool castShadows = lightComponent.getCastShadows();
                    if(GUIComponents::checkBox("Cast shadows", castShadows, 120.f)) {
                        lightComponent.setCastShadows(castShadows);
                        newState = lightComponent;
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeLightComponentStateCommand(_guiState->selectedEntity, oldState, newState));
                    }
                    float projectionAspect = lightComponent.getLightProjectionAspect();
                    if(GUIComponents::dragFloat("Shadow cover", projectionAspect, 0.5f, 1.f, 999999.f, 120.f)) {
                        addToURControl = true;
                        lightComponent.setLightProjectionAspect(projectionAspect);
                        newState = lightComponent;
                    }
                    uint32_t shadomapSize = lightComponent.getFBO()->getSize().width;
                    static std::vector<std::string> shadowMapSizes = { "256", "512", "1024", "2048", "4096", "8192" };
                    std::string selectedShadowmapSize = std::to_string(shadomapSize);
                    int shadowmapIndex = GUIComponents::combo("Shadowmap size", selectedShadowmapSize, shadowMapSizes, 120.f);
                    if(shadowmapIndex != -1) {
                        uint32_t newShadowmapSize = std::stoi(shadowMapSizes[shadowmapIndex]);
                        lightComponent.setShadowMapSize(newShadowmapSize);
                        newState = lightComponent;
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeLightComponentStateCommand(_guiState->selectedEntity, oldState, newState));
                    }
                }
                if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
                    addToURControl = false;
                    if(oldState != newState)
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeLightComponentStateCommand(_guiState->selectedEntity, oldState, newState));
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveLightComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showPhysicsComponent() {
        if(_guiState->selectedEntity.hasComponent<PhysicsComponent>()) {
            auto id = (void*)(typeid(PhysicsComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Physics")) {
                std::string popUpId = guiPopups[GUIPopupIds::PhysicsPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& physicsComponent = _guiState->selectedEntity.getComponent<PhysicsComponent>();
                physicsComponent.setShowCollider(true);
                ColliderType colliderType = physicsComponent.getColliderType();

                int colliderTypeIndex = GUIComponents::combo("Collider type", colliderTypes[static_cast<int>(colliderType)], colliderTypes, 90.f);
                if(colliderTypeIndex != -1) {
                    ColliderType newColliderType = static_cast<ColliderType>(colliderTypeIndex);
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new SetColliderTypePhysicsComponentCommand(_guiState->selectedEntity, newColliderType, 
                    colliderType, _guiState->selectedEntity.getComponent<TransformComponent>().getSize()));
                }

                static bool addToURControl = false;
                static PhysicsComponentProperties oldProperties;
                static PhysicsComponentProperties newProperties;
                if(!addToURControl) {
                    oldProperties = PhysicsComponentProperties{ physicsComponent.getMass(), physicsComponent.getIsRotated(), 
                        physicsComponent.getIsTrigger(), physicsComponent.getShapeDimensions() };
                    newProperties = oldProperties;
                }
                if(colliderType != ColliderType::TriangleMesh) {
                    auto mass = physicsComponent.getMass();
                    if(GUIComponents::dragFloat("Mass", mass, 0.01f, 0.f, 999999.f, 90.f)) {
                        addToURControl = true;
                        physicsComponent.setMass(mass);
                        newProperties.mass = mass;
                    }
                    bool isRotated = physicsComponent.getIsRotated();
                    if(GUIComponents::checkBox("Is rotated", isRotated, 90.f)) {
                        PhysicsComponentProperties newProperties = { physicsComponent.getMass(), isRotated, 
                            physicsComponent.getIsTrigger(), physicsComponent.getShapeDimensions() };
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangePhysicsComponentPropertiesCommand(_guiState->selectedEntity, oldProperties, newProperties));
                    }
                }
                bool isTrigger = physicsComponent.getIsTrigger();
                if(GUIComponents::checkBox("Is trigger", isTrigger, 90.f)) {
                    PhysicsComponentProperties newProperties = { physicsComponent.getMass(), physicsComponent.getIsRotated(), 
                        isTrigger, physicsComponent.getShapeDimensions() };
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangePhysicsComponentPropertiesCommand(_guiState->selectedEntity, oldProperties, newProperties));
                }

                auto shapeDimensions = physicsComponent.getShapeDimensions();
                switch(colliderType) {
                case ColliderType::Box:
                    if(GUIComponents::dragFloat3("Scale", shapeDimensions, 0.01f, 1.f, 0.01f, 999999.f, 90.f)) {
                        addToURControl = true;
                        physicsComponent.setShapeDimensions(shapeDimensions);
                        newProperties.shapeDimensions = shapeDimensions;
                    }
                    break;
                case ColliderType::Sphere:
                    if(GUIComponents::dragFloat("Radius", shapeDimensions.x, 0.01f, 0.01f, 999999.f, 90.f)) {
                        addToURControl = true;
                        physicsComponent.setShapeDimensions(shapeDimensions);
                        newProperties.shapeDimensions = shapeDimensions;
                    }
                    break;
                case ColliderType::Cylinder:
                case ColliderType::Cone:
                case ColliderType::Capsule:
                    if(GUIComponents::dragFloat("Radius", shapeDimensions.x, 0.01f, 0.01f, 999999.f, 90.f)) {
                        addToURControl = true;
                        physicsComponent.setShapeDimensions(shapeDimensions);
                        newProperties.shapeDimensions = shapeDimensions;
                    }
                    if(GUIComponents::dragFloat("Height", shapeDimensions.y, 0.01f, 0.01f, 999999.f, 90.f)) {
                        addToURControl = true;
                        physicsComponent.setShapeDimensions(shapeDimensions);
                        newProperties.shapeDimensions = shapeDimensions;
                    }
                    break;
                }
                if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
                    addToURControl = false;
                    if(oldProperties != newProperties)
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangePhysicsComponentPropertiesCommand(_guiState->selectedEntity, oldProperties, newProperties));
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemovePhysicsComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showScriptComponent() {
        if(_guiState->selectedEntity.hasComponent<ScriptComponent>()) {
            auto id = (void*)(typeid(ScriptComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Script")) {
                std::string popUpId = guiPopups[GUIPopupIds::ScriptPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& scriptComponent = _guiState->selectedEntity.getComponent<ScriptComponent>();
                auto& scriptsBihaviorName = scriptComponent.getScriptsBehaviorName();
                static int currentScriptIndex = 0;
                if(currentScriptIndex >= scriptsBihaviorName.size())
                    currentScriptIndex = 0;
                std::string selectedScriptsStr = !scriptsBihaviorName.empty() ? scriptsBihaviorName[currentScriptIndex] : "None";
                std::string popUpBindId = guiPopups[GUIPopupIds::ScriptBindPopup];
                bool openPopUpBind = false;
                auto addFunc = [&]() {
                    openPopUpBind = true;
                };
                int selectedScriptIndex = GUIComponents::comboAndButton("Script behavior", selectedScriptsStr, scriptsBihaviorName, addFunc, 110.f);
                if(selectedScriptIndex != -1)
                    currentScriptIndex = selectedScriptIndex;
                if(openPopUpBind)
                    ImGui::OpenPopup(popUpBindId.c_str());
                if(!scriptsBihaviorName.empty()) {
                    auto& script = scriptComponent.getScripts()[currentScriptIndex];
                    bool isEnabled = script.isEnabled;
                    if(GUIComponents::checkBox("Enabled", isEnabled, 110.f))
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new SetScriptComponentEnabledStateCommand(_guiState->selectedEntity, script.behaviorClassName, isEnabled));
                    auto& availSize = ImGui::GetContentRegionAvail();
                    bool validateScriptFlag = _guiState->scene->getSceneState() != SceneState::Edit || !_guiState->bgFuncsInRun.empty();
                    if(validateScriptFlag) {
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                    }
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                    if(ImGui::Button("Validate")) {
                        std::shared_future<void> validScriptsFuture = std::async(std::launch::async, [&]() {
                            _guiState->scene->getSceneScripts()->validateScript(script.behaviorClassName, _guiState->scene);
                        });
                        _guiState->bgFuncsInRun.push_back(validScriptsFuture);
                        ImGui::CloseCurrentPopup();
                    }
                    if(ImGui::Button("Unbind script")) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new UnbindBehaviorScriptComponentCommand(_guiState->selectedEntity, script.behaviorClassName));
                        ImGui::CloseCurrentPopup();
                    }
                    if(validateScriptFlag) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleVar();
                    }
                }
                if(ImGui::BeginPopup(popUpBindId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    auto& scriptRegistryKeys = _guiState->scene->getScriptDLLRegistry()->getKeys();
                    for(auto& behaviorClassName : scriptRegistryKeys) {
                        if(ImGui::Button(behaviorClassName.c_str(), { availSize.x, 0.f })) {
                            _guiState->scene->getSceneRegistry()->current->urControl.execute(new BindBehaviorScriptComponentCommand(_guiState->selectedEntity, behaviorClassName));
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::EndPopup();
                }
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveScriptComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }

    void GUIComponentsPanel::showAudioComponent() {
        if(_guiState->selectedEntity.hasComponent<AudioComponent>()) {
            auto id = (void*)(typeid(AudioComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Audio")) {
                std::string popUpId = guiPopups[GUIPopupIds::AudioPopup];
                if(ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(popUpId.c_str());
                auto& audioComponent = _guiState->selectedEntity.getComponent<AudioComponent>();
                auto soundSources = audioComponent.getPathsOfSoundSources();
                static int currentSoundSourceIndex = 0;
                if(currentSoundSourceIndex >= soundSources.size())
                    currentSoundSourceIndex = 0;
                std::string selectedSoundSourceStr = !soundSources.empty() ? soundSources[currentSoundSourceIndex] : "None";
                auto addFunc = [&]() {
                    ImGuiFileDialog::Instance()->OpenDialog("AddAudioSource", "Choose File", ".mp3", 
                        (_guiState->scene->getProjectData()->rootPath.string() + '/').c_str(), 1, nullptr);
                };
                int selectedSoundSourceIndex = GUIComponents::comboAndButton("Sound source", selectedSoundSourceStr, soundSources, addFunc, 100.f);
                if(selectedSoundSourceIndex != -1)
                    currentSoundSourceIndex = selectedSoundSourceIndex;
                if(!soundSources.empty()) {
                    auto soundSourceSpecification = audioComponent.getSoundSourceSpec(currentSoundSourceIndex);
                    static bool addToURControl = false;
                    static AudioComponentPropertiesSpecification oldState;
                    static AudioComponentPropertiesSpecification newState;
                    if(!addToURControl) {
                        oldState = { soundSourceSpecification->getIs3D(), soundSourceSpecification->getStartPaused(), 
                            soundSourceSpecification->getPlayLooped(), soundSourceSpecification->getVolume(), soundSourceSpecification->getMinDistance(),
                            soundSourceSpecification->getMaxDistance(), soundSourceSpecification->getPlaybackSpeed() };
                        newState = oldState;
                    }
                    bool startPaused = soundSourceSpecification->getStartPaused();
                    if(GUIComponents::checkBox("Is start paused", startPaused, 100.f)) {
                        newState = oldState;
                        newState.startPaused = startPaused;
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeAudioComponentPropertiesCommand(_guiState->selectedEntity, 
                            soundSourceSpecification->getSoundSourcePath(), oldState, newState));
                    }
                    bool looped = soundSourceSpecification->getPlayLooped();
                    if(GUIComponents::checkBox("Is looped", looped, 100.f)) {
                        newState = oldState;
                        newState.playLooped = looped;
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeAudioComponentPropertiesCommand(_guiState->selectedEntity, 
                            soundSourceSpecification->getSoundSourcePath(), oldState, newState));
                    }
                    bool is3D = soundSourceSpecification->getIs3D();
                    if(GUIComponents::checkBox("Is 3D", is3D, 100.f)) {
                        newState = oldState;
                        newState.is3d = is3D;
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeAudioComponentPropertiesCommand(_guiState->selectedEntity, 
                            soundSourceSpecification->getSoundSourcePath(), oldState, newState));
                    }
                    float volume = newState.volume;
                    if(GUIComponents::dragFloat("Volume", volume, 0.003f, 0.f, 1.f, 100.f)) {
                        addToURControl = true;
                        newState.volume = volume;
                        soundSourceSpecification->setVolume(volume);
                    }
                    float playbackSpeed = newState.playbackSpeed;
                    if(GUIComponents::dragFloat("Playback speed", playbackSpeed, 0.005f, 0.f, 10.f, 100.f)) {
                        addToURControl = true;
                        newState.playbackSpeed = playbackSpeed;
                        soundSourceSpecification->setPlaybackSpeed(playbackSpeed);
                    }
                    if(is3D) {
                        float minDistance = newState.minDistance;
                        if(GUIComponents::dragFloat("Min distance", minDistance, 0.05f, 0.f, 999999.f, 100.f)) {
                            addToURControl = true;
                            newState.minDistance = minDistance;
                            soundSourceSpecification->setMinDistance(minDistance);
                        }
                        float maxDistance = newState.maxDistance;
                        if(GUIComponents::dragFloat("Max distance", maxDistance, 0.05f, 0.f, 999999.f, 100.f)) {
                            addToURControl = true;
                            newState.maxDistance = maxDistance;
                            soundSourceSpecification->setMaxDistance(maxDistance);
                        }
                    }
                    if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
                        addToURControl = false;
                        if(oldState != newState)
                            _guiState->scene->getSceneRegistry()->current->urControl.execute(new ChangeAudioComponentPropertiesCommand(_guiState->selectedEntity, 
                                soundSourceSpecification->getSoundSourcePath(), oldState, newState));
                    }
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                    if(ImGui::Button("Remove sound source"))
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveSoundSourceAudioComponentCommand(_guiState->selectedEntity, 
                            soundSourceSpecification->getSoundSourcePath()));
                }
                float popUpWidth = 150.f;
                ImGui::SetNextWindowSize({popUpWidth, 0.f});
                if(ImGui::BeginPopup(popUpId.c_str())) {
                    auto& availSize = ImGui::GetContentRegionAvail();
                    if(ImGui::Button("Remove component", { availSize.x, 0.f })) {
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveAudioComponentCommand(_guiState->selectedEntity));
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                ImGui::TreePop();
            }
            showSeparator();
        }
    }
}