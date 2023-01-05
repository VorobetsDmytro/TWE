#include "gui/gui-scene-panel.hpp"

namespace TWE {
    void GUIScenePanel::showPanel(Entity& selectedEntity) {
        ImGui::Begin("Scene");
        if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
            unselectEntity(selectedEntity);
            ImGuiFileDialog::Instance()->Close();
        }
        canOpenWindowPopup = true;
        std::string entityPopup = guiPopups[GUIPopupIds::EntityPopup];
        if(_scene)  
            _scene->_sceneRegistry.current->entityRegistry.view<NameComponent>().each([&](entt::entity entity, NameComponent& nameComponent){
                Entity entityInstance = {entity, _scene};
                auto& parentChildsComponent = entityInstance.getComponent<ParentChildsComponent>();
                if(parentChildsComponent.parent == entt::null)
                    showSceneEntity(entityInstance, selectedEntity);
            });
        std::string menuPopup = guiPopups[GUIPopupIds::MenuPopup];
        if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
            ImGui::SetWindowFocus();
            if(canOpenWindowPopup)
                ImGui::OpenPopup(menuPopup.c_str());
        }
        showSceneMenuPopup(menuPopup, selectedEntity);
        showSceneEntityPopup(entityPopup, selectedEntity);
        ImGui::End();
    }

    void GUIScenePanel::setScene(Scene* scene) {
        _scene = scene;
    }

    void GUIScenePanel::showSceneEntity(Entity& entity, Entity& selectedEntity) {
        auto& nameComponent = entity.getComponent<NameComponent>();
        std::string entityPopup = guiPopups[GUIPopupIds::EntityPopup];
        auto id = (void*)entity.getSource();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
        bool isOpened = ImGui::TreeNodeEx(id, flags, nameComponent.getName().c_str());
        if(ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
            if(entity.getSource() != selectedEntity.getSource())
                selectEntity(entity, selectedEntity);
            ImGuiFileDialog::Instance()->Close();
        }
        if(isOpened) {
            auto& parentChildsComponent = entity.getComponent<ParentChildsComponent>();
            for(auto childEnttity : parentChildsComponent.childs) {
                Entity instance = { childEnttity, _scene };
                showSceneEntity(instance, selectedEntity);
            }
            ImGui::TreePop();
        }
        if(ImGui::IsItemClicked(1)) {
            ImGui::OpenPopup(entityPopup.c_str());
            canOpenWindowPopup = false;
            ImGuiFileDialog::Instance()->Close();
        }
    }

    void GUIScenePanel::showSceneEntityPopup(const std::string& popupId, Entity& selectedEntity) {
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popupId.c_str())) {
            auto& availSize = ImGui::GetContentRegionAvail();
            Entity preSelectedEntity = selectedEntity;
            if(showCreateEntityMenu(selectedEntity)) {
                selectedEntity.getComponent<ParentChildsComponent>().parent = preSelectedEntity.getSource();
                preSelectedEntity.getComponent<ParentChildsComponent>().childs.push_back(selectedEntity.getSource());
            }
            if(ImGui::Button("Remove", {availSize.x, 0.f})) {
                if(selectedEntity.hasComponent<MeshRendererComponent>())
                    selectedEntity.getComponent<MeshRendererComponent>().showCollider = false;
                selectedEntity.destroy();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    bool GUIScenePanel::showCreateEntityMenu(Entity& selectedEntity) {
        bool isInteracted = false;
        auto& availSize = ImGui::GetContentRegionAvail();
        if(ImGui::BeginMenu("Create entity")) {
            if(ImGui::Button("Entity", {availSize.x, 0.f})) {
                selectEntity(_scene->createEntity(), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Cube", {availSize.x, 0.f})) {
                selectEntity(Shape::createCubeEntity(_scene), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Plate", {availSize.x, 0.f})) {
                selectEntity(Shape::createPlateEntity(_scene), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Camera", {availSize.x, 0.f})) {
                selectEntity(Shape::createCameraEntity(_scene), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }       
            if(ImGui::Button("Point Light", {availSize.x, 0.f})) {
                selectEntity(Shape::createPointLightEntity(_scene), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }            
            if(ImGui::Button("Spot Light", {availSize.x, 0.f})) {
                selectEntity(Shape::createSpotLightEntity(_scene), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }            
            if(ImGui::Button("Dir Light", {availSize.x, 0.f})) {
                selectEntity(Shape::createDirLightEntity(_scene), selectedEntity);
                isInteracted = true;
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::Button("Model", {availSize.x, 0.f})) {
                ImGuiFileDialog::Instance()->OpenDialog("LoadModel", "Choose File", ".obj,.fbx", ".", 1, nullptr);
                // isInteracted = true;
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::Button("Cubemap", {availSize.x, 0.f})) {
                ImGuiFileDialog::Instance()->OpenDialog("LoadCubemap", "Choose File", ".png,.jpg,.jpeg", ".", 6, nullptr);
                // isInteracted = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndMenu();
        }
        return isInteracted;
    }

    void GUIScenePanel::showSceneMenuPopup(const std::string& popupId, Entity& selectedEntity) {
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popupId.c_str())) {
            showCreateEntityMenu(selectedEntity);
            ImGui::EndPopup();
        }
    }

    void GUIScenePanel::selectEntity(Entity& entity, Entity& selectedEntity) {
        unselectEntity(selectedEntity);
        selectedEntity = entity;
        if(selectedEntity.hasComponent<MeshRendererComponent>())
            selectedEntity.getComponent<MeshRendererComponent>().showCollider = true;
    }

    void GUIScenePanel::unselectEntity(Entity& entity) {
        if(entity.getSource() == entt::null)
            return;
        if(entity.hasComponent<MeshRendererComponent>())
            entity.getComponent<MeshRendererComponent>().showCollider = false;
        entity = {};
    }
}