#include "gui/gui-viewport-panel.hpp"

namespace TWE {
    GUIViewportPanel::GUIViewportPanel() {
        _guiState = nullptr;
    }

    void GUIViewportPanel::setGUIState(GUIStateSpecification* guiState) {
        _guiState = guiState;
    }

    void GUIViewportPanel::showPanel() {
        ImGui::Begin("Viewport");
        if(!_guiState) {
            ImGui::End();
            return;
        }
        showViewportStatePanel();
        auto& cursorPos = ImGui::GetCursorPos();
        if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
            ImGui::SetWindowFocus();
        _guiState->isFocusedOnViewport = ImGui::IsWindowFocused();
        ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
        auto& frameSize = _guiState->window->getFrameBuffer()->getSize();
        if(viewPortSize.x != frameSize.width || viewPortSize.y != frameSize.height)
            _guiState->window->getFrameBuffer()->resize(viewPortSize.x, viewPortSize.y);
        auto frameId = (void*)(uint64_t)_guiState->window->getFrameBuffer()->getColorAttachment(0);
        ImGui::Image(frameId, viewPortSize, {0, 1}, {1, 0});

        auto& windowSize = ImGui::GetWindowSize();
        auto& minBound = ImGui::GetWindowPos();
        minBound.x += cursorPos.x;
        minBound.y += cursorPos.y;

        auto& mousePos = ImGui::GetMousePos();
        mousePos.x -= minBound.x;
        mousePos.y -= minBound.y;
        mousePos.y = viewPortSize.y - mousePos.y;
        
        bool bShowGizmo = _guiState->scene->getIsFocusedOnDebugCamera() && !ImGuiFileDialog::Instance()->IsOpened() && _guiState->bgFuncsInRun.empty();
        if(bShowGizmo) {
            bool isUsing = showGizmo();
            if(mousePos.x >= 0.f && mousePos.y >= 0.f && mousePos.x < viewPortSize.x && mousePos.y < viewPortSize.y) {
                _guiState->isMouseOnViewport = true;
                if(!isUsing && !getIsMouseDisabled() && ImGui::IsMouseClicked(0)) {
                    int data = _guiState->window->getFrameBuffer()->readPixel(1, (int)mousePos.x, (int)mousePos.y);
                    if(data == -1 || !_guiState->scene->getSceneStateSpecification()->entityRegistry.valid((entt::entity)data))
                        unselectEntity(_guiState->selectedEntity);
                    else
                        selectEntity(Entity{ (entt::entity)data, _guiState->scene });
                }
            } else
                _guiState->isMouseOnViewport = false;
        }
        ImGui::End();
    }

    bool GUIViewportPanel::showGizmo() {
        if(_guiState->gizmoOperation == GizmoOperation::None || _guiState->selectedEntity.getSource() == entt::null)
            return false;
        Camera* camera = _guiState->scene->getSceneCamera()->camera;
        if(!camera)
            return false;
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        auto& windowPos = ImGui::GetWindowPos();
        auto& windowSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

        const glm::mat4& cameraProjection = _guiState->scene->getSceneCamera()->projection;
        glm::mat4 cameraView =  _guiState->scene->getSceneCamera()->view;
        auto& selectedEntityTransform = _guiState->selectedEntity.getComponent<TransformComponent>();
        auto selectedEntityModel = selectedEntityTransform.getModel();

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), static_cast<ImGuizmo::OPERATION>(_guiState->gizmoOperation),
                             ImGuizmo::LOCAL, glm::value_ptr(selectedEntityModel));

        bool isUsing = ImGuizmo::IsUsing();

        static bool addToURControl = false;
        static TransformComponent oldState;
        static TransformComponent newState;
        if(!addToURControl)
            oldState = selectedEntityTransform;

        if(isUsing) {
            glm::vec3 position, rotation, size;
            Math::decomposeTransform(selectedEntityModel, position, rotation, size);
            switch (_guiState->gizmoOperation) {
            case GizmoOperation::Translate:
                addToURControl = true;
                selectedEntityTransform.setPosition(position);
                newState = selectedEntityTransform;
                break;
            case GizmoOperation::Rotate:
                addToURControl = true;
                selectedEntityTransform.setRotation(rotation);
                newState = selectedEntityTransform;
                break;
            case GizmoOperation::Scale:
                addToURControl = true;
                selectedEntityTransform.setSize(size);
                newState = selectedEntityTransform;
                break;
            }
        }
        if(Input::mouseButtonAction(Mouse::MOUSE_BUTTON_LEFT) == Action::RELEASE && addToURControl) {
            addToURControl = false;
            if(oldState.getTransform() != newState.getTransform())
                _guiState->scene->getSceneStateSpecification()->urControl.execute(
                    new ChangeTransformComponentState(_guiState->selectedEntity, oldState, newState));
        }

        return isUsing;
    }

    void GUIViewportPanel::showViewportStatePanel() {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("##ViewportState", 0, flags);

        bool hasBGFuncsInRun = !_guiState->bgFuncsInRun.empty();
        bool playFlag = _guiState->scene->getSceneState() != SceneState::Edit;
        if(playFlag || hasBGFuncsInRun) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        if(ImGui::Button("Play")) {
            unselectEntity(_guiState->selectedEntity);
            _guiState->scene->setState(SceneState::Run);
            ImGui::SetWindowFocus("Viewport");
        }
        if(playFlag || hasBGFuncsInRun) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if(!playFlag) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        ImGui::SameLine();
        if(ImGui::Button("Stop")) {
            unselectEntity(_guiState->selectedEntity);
            _guiState->scene->setState(SceneState::Edit);
        }
        ImGui::SameLine();
        bool pauseFlag = !playFlag || _guiState->scene->getSceneState() == SceneState::Run;
        if(ImGui::Button(pauseFlag ? "Pause" : "Unpause"))
            if(pauseFlag)
                _guiState->scene->setState(SceneState::Pause);
            else
                _guiState->scene->setState(SceneState::Unpause);
                
        if(!playFlag) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void GUIViewportPanel::selectEntity(Entity& entity) {
        unselectEntity(_guiState->selectedEntity);
        _guiState->selectedEntity = entity;
        if(_guiState->selectedEntity.hasComponent<PhysicsComponent>())
            _guiState->selectedEntity.getComponent<PhysicsComponent>().setShowCollider(true);
    }

    void GUIViewportPanel::unselectEntity(Entity& entity) {
        if(entity.getSource() == entt::null)
            return;
        if(entity.hasComponent<PhysicsComponent>())
            entity.getComponent<PhysicsComponent>().setShowCollider(false);
        entity = {};
    }

    bool GUIViewportPanel::getIsMouseDisabled() { return ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouse; }
}