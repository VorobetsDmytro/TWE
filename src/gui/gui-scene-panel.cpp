#include "gui/gui-scene-panel.hpp"

namespace TWE {
    GUIScenePanel::GUIScenePanel() {
        _guiState = nullptr;
    }

    void GUIScenePanel::showPanel() {
        ImGui::Begin("Scene");
        if(!_guiState) {
            ImGui::End();
            return;
        }
        if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
            unselectEntity(_guiState->selectedEntity);
            _showSceneEntity = {};
            ImGuiFileDialog::Instance()->Close();
        }
        _canOpenWindowPopup = true;
        _showSceneEntityPopup = false;
        bool showSeparatorOnce = true;
        std::string entityPopup = guiPopups[GUIPopupIds::EntityPopup];
        if(_guiState->scene)  
            _guiState->scene->getSceneRegistry()->current->entityRegistry.each([&](entt::entity entity){
                Entity entityInstance = {entity, _guiState->scene};
                if(!entityInstance.hasComponent<NameComponent>())
                    return;
                auto& parentChildsComponent = entityInstance.getComponent<ParentChildsComponent>();
                if(parentChildsComponent.parent == entt::null) {
                    if(showSeparatorOnce) {
                        showSeparatorOnce = false;
                        showSeparator(entityInstance, true);
                    }
                    showSceneEntity(entityInstance);
                }
            });
        std::string menuPopup = guiPopups[GUIPopupIds::MenuPopup];
        if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
            ImGui::SetWindowFocus();
            if(_canOpenWindowPopup)
                ImGui::OpenPopup(menuPopup.c_str());
        }
        if(_showSceneEntityPopup)
            ImGui::OpenPopup(entityPopup.c_str());
        showSceneMenuPopup(menuPopup);
        showSceneEntityPopup(entityPopup);
        auto& availSize = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f);
        ImGui::Button("##Empty", { availSize.x, availSize.y });
        ImGui::PopStyleVar();
        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::EntityItem].c_str())) {
                const wchar_t* item = static_cast<const wchar_t*>(payload->Data);
                std::wstring itemWSTR = item;
                entt::entity entityItem = (entt::entity)std::stoi(itemWSTR);
                if(_guiState->scene->getSceneRegistry()->current->entityRegistry.valid(entityItem)) {
                    Entity entityItemEnt = { entityItem, _guiState->scene };
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new DragAndDropEntityCommand(entityItemEnt, {}));
                }
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::End();
        showFileDialog();
    }

    void GUIScenePanel::setGUIState(GUIStateSpecification* guiState) {
        _guiState = guiState;
    }

    void GUIScenePanel::showSeparator(Entity& entity, bool isUpper) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.2f);
        ImGui::Separator();
        ImGui::PopStyleVar();
    }

    void GUIScenePanel::showSceneEntity(Entity& entity) {
        auto& nameComponent = entity.getComponent<NameComponent>();
        std::string entityPopup = guiPopups[GUIPopupIds::EntityPopup];
        auto id = (void*)entity.getSource();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (_guiState->selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
        bool isOpened = ImGui::TreeNodeEx(id, flags, nameComponent.getName().c_str());
        if(ImGui::IsItemClicked(1)) {
            _canOpenWindowPopup = false;
            _showSceneEntityPopup = true;
            _showSceneEntity = entity;
            ImGuiFileDialog::Instance()->Close();
        }
        if(ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
            if(entity.getSource() != _guiState->selectedEntity.getSource())
                selectEntity(entity);
            ImGuiFileDialog::Instance()->Close();
        }
        if(ImGui::BeginDragDropSource()) {
            auto item = std::to_wstring((int)_guiState->selectedEntity.getSource());
            ImGui::SetDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::EntityItem].c_str(), item.c_str(), (wcslen(item.c_str()) + 1) * sizeof(wchar_t));
            ImGui::EndDragDropSource();
        }
        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::EntityItem].c_str())) {
                const wchar_t* item = static_cast<const wchar_t*>(payload->Data);
                std::wstring itemWSTR = item;
                entt::entity entityItem = (entt::entity)std::stoi(itemWSTR);
                if(_guiState->scene->getSceneRegistry()->current->entityRegistry.valid(entityItem)) {
                    if(entityItem != entity.getSource()) {
                        Entity entityItemEnt = { entityItem, _guiState->scene };
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new DragAndDropEntityCommand(entityItemEnt, entity));
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
        showSeparator(entity);
        if(isOpened) {
            auto& parentChildsComponent = entity.getComponent<ParentChildsComponent>();
            for(auto childEnttity : parentChildsComponent.childs) {
                Entity instance = { childEnttity, _guiState->scene };
                showSceneEntity(instance);
            }
            ImGui::TreePop();
        }
    }

    void GUIScenePanel::showSceneEntityPopup(const std::string& popupId) {
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popupId.c_str())) {
            auto& availSize = ImGui::GetContentRegionAvail();
            if(showCreateEntityMenu()) {
                addEntityToSelected();
                _guiState->scene->getSceneRegistry()->current->urControl.execute(new CreateEntityCommand(_guiState->selectedEntity, 
                    [&](){ unselectEntity(_guiState->selectedEntity); }));
            }
            ImGui::Separator();
            bool hasBGFuncsInRun = !_guiState->bgFuncsInRun.empty();
            if(hasBGFuncsInRun) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            if(ImGui::Button("Remove", {availSize.x, 0.f})) {
                auto removeEntity = _guiState->selectedEntity;
                unselectEntity(_guiState->selectedEntity);
                _guiState->scene->getSceneRegistry()->current->urControl.execute(new RemoveEntityCommand(removeEntity, 
                    [&](){
                        unselectEntity(_guiState->selectedEntity);
                    }
                ));
                ImGui::CloseCurrentPopup();
            }
            if(hasBGFuncsInRun) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }

            ImGui::EndPopup();
        }
    }

    void GUIScenePanel::addEntityToSelected() {
        if(_showSceneEntity.getSource() != entt::null) {
            _guiState->selectedEntity.getComponent<ParentChildsComponent>().parent = _showSceneEntity.getSource();
            _showSceneEntity.getComponent<ParentChildsComponent>().childs.push_back(_guiState->selectedEntity.getSource());
        }
    }

    bool GUIScenePanel::showCreateEntityMenu() {
        bool isInteracted = false;
        auto& availSize = ImGui::GetContentRegionAvail();
        if(ImGui::BeginMenu("Create entity")) {
            if(ImGui::Button("Entity", {availSize.x, 0.f})) {
                selectEntity(_guiState->scene->createEntity());
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Cube", {availSize.x, 0.f})) {
                selectEntity(Shape::createCubeEntity(_guiState->scene));
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Plate", {availSize.x, 0.f})) {
                selectEntity(Shape::createPlateEntity(_guiState->scene));
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Camera", {availSize.x, 0.f})) {
                selectEntity(Shape::createCameraEntity(_guiState->scene));
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Point Light", {availSize.x, 0.f})) {
                selectEntity(Shape::createPointLightEntity(_guiState->scene));
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }            
            if(ImGui::Button("Spot Light", {availSize.x, 0.f})) {
                selectEntity(Shape::createSpotLightEntity(_guiState->scene));
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }            
            if(ImGui::Button("Dir Light", {availSize.x, 0.f})) {
                selectEntity(Shape::createDirLightEntity(_guiState->scene));
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::Button("Model", {availSize.x, 0.f})) {
                ImGuiFileDialog::Instance()->OpenDialog("LoadModel", "Choose File", ".obj,.fbx", 
                    (_guiState->scene->getProjectData()->rootPath.string() + '/').c_str(), 1, nullptr);
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::Button("Cubemap", {availSize.x, 0.f})) {
                ImGuiFileDialog::Instance()->OpenDialog("LoadCubemap", "Choose File", ".png,.jpg,.jpeg", 
                    (_guiState->scene->getProjectData()->rootPath.string() + '/').c_str(), 6, nullptr);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndMenu();
        }  
        return isInteracted;
    }

    void GUIScenePanel::showSceneMenuPopup(const std::string& popupId) {
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popupId.c_str())) {
            if(showCreateEntityMenu())
                _guiState->scene->getSceneRegistry()->current->urControl.execute(new CreateEntityCommand(_guiState->selectedEntity, 
                    [&](){ unselectEntity(_guiState->selectedEntity); }));
            ImGui::EndPopup();
        }
    }

    void GUIScenePanel::selectEntity(Entity& entity) {
        unselectEntity(_guiState->selectedEntity);
        _guiState->selectedEntity = entity;
        if(_guiState->selectedEntity.hasComponent<PhysicsComponent>())
            _guiState->selectedEntity.getComponent<PhysicsComponent>().setShowCollider(true);
    }

    void GUIScenePanel::unselectEntity(Entity& entity) {
        if(entity.getSource() == entt::null)
            return;
        if(entity.hasComponent<PhysicsComponent>())
            entity.getComponent<PhysicsComponent>().setShowCollider(false);
        entity = {};
    }

    void GUIScenePanel::showFileDialog() {
        if(ImGuiFileDialog::Instance()->Display("LoadModel"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                auto& modelEntity = Shape::createModelEntity(_guiState->scene, filePathName);
                if(modelEntity.getSource() != entt::null) {
                    selectEntity(modelEntity);
                    addEntityToSelected();
                    _guiState->scene->getSceneRegistry()->current->urControl.execute(new CreateEntityCommand(_guiState->selectedEntity, 
                        [&](){ unselectEntity(_guiState->selectedEntity); }));
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("LoadCubemap"))  {
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
                        selectEntity(Shape::createCubemapEntity(_guiState->scene, attachemnts));
                        addEntityToSelected();
                        _guiState->scene->getSceneRegistry()->current->urControl.execute(new CreateEntityCommand(_guiState->selectedEntity, 
                            [&](){ unselectEntity(_guiState->selectedEntity); }));
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
    }
}