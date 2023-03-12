#include "gui/gui-viewport-panel.hpp"

namespace TWE {
    GUIViewportPanel::GUIViewportPanel() {
        _gizmoOperation = GizmoOperation::Translate;
    }

    void GUIViewportPanel::showPanel(Entity& selectedEntity, bool& isFocusedOnViewport, bool& isMouseOnViewport) {
        ImGui::Begin("Viewport");
        showViewportStatePanel(selectedEntity);
        auto& cursorPos = ImGui::GetCursorPos();
        if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
            ImGui::SetWindowFocus();
        isFocusedOnViewport = ImGui::IsWindowFocused();
        ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
        auto frameSize = _scene->getFrameBuffer()->getSize();
        if(viewPortSize.x != frameSize.first || viewPortSize.y != frameSize.second)
            _scene->getFrameBuffer()->resize(viewPortSize.x, viewPortSize.y);
        auto frameId = (void*)(uint64_t)_scene->getFrameBuffer()->getColorAttachment(0);
        ImGui::Image(frameId, viewPortSize, {0, 1}, {1, 0});

        auto& windowSize = ImGui::GetWindowSize();
        auto& minBound = ImGui::GetWindowPos();
        minBound.x += cursorPos.x;
        minBound.y += cursorPos.y;

        auto& mousePos = ImGui::GetMousePos();
        mousePos.x -= minBound.x;
        mousePos.y -= minBound.y;
        mousePos.y = viewPortSize.y - mousePos.y;
        if(_scene->getIsFocusedOnDebugCamera() && !ImGuiFileDialog::Instance()->IsOpened()) {
            bool isUsing = showGizmo(selectedEntity);
            if(mousePos.x >= 0.f && mousePos.y >= 0.f && mousePos.x < viewPortSize.x && mousePos.y < viewPortSize.y) {
                isMouseOnViewport = true;
                if(!isUsing && !getIsMouseDisabled() && ImGui::IsMouseClicked(0)) {
                    int data = _scene->getFrameBuffer()->readPixel(1, (int)mousePos.x, (int)mousePos.y);
                    if(data == -1 || !_scene->getSceneStateSpecification()->entityRegistry.valid((entt::entity)data))
                        unselectEntity(selectedEntity);
                    else
                        selectEntity(Entity{ (entt::entity)data, _scene }, selectedEntity);
                }
            } else
                isMouseOnViewport = false;
        }
        ImGui::End();
    }

    bool GUIViewportPanel::showGizmo(Entity& selectedEntity) {
        if(_gizmoOperation == GizmoOperation::None || selectedEntity.getSource() == entt::null)
            return false;
        Camera* camera = _scene->getSceneCamera()->camera;
        if(!camera)
            return false;
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        auto& windowPos = ImGui::GetWindowPos();
        auto& windowSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

        const glm::mat4& cameraProjection = _scene->getSceneCamera()->projection;
        glm::mat4 cameraView =  _scene->getSceneCamera()->view;
        auto& selectedEntityTransform = selectedEntity.getComponent<TransformComponent>();
        auto selectedEntityModel = selectedEntityTransform.getModel();

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), static_cast<ImGuizmo::OPERATION>(_gizmoOperation),
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
            switch (_gizmoOperation) {
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
                _scene->getSceneStateSpecification()->urControl.execute(
                    new ChangeTransformComponentState(selectedEntity, oldState, newState));
        }

        return isUsing;
    }

    void GUIViewportPanel::showViewportStatePanel(Entity& selectedEntity) {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("##ViewportState", 0, flags);

        bool playFlag = _scene->getSceneState() != SceneState::Edit;
        if(playFlag) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        if(ImGui::Button("Play")) {
            unselectEntity(selectedEntity);
            _scene->setState(SceneState::Run);
            ImGui::SetWindowFocus("Viewport");
        }
        if(playFlag) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if(!playFlag) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        ImGui::SameLine();
        if(ImGui::Button("Stop")) {
            unselectEntity(selectedEntity);
            _scene->setState(SceneState::Edit);
        }
        ImGui::SameLine();
        bool pauseFlag = !playFlag || _scene->getSceneState() == SceneState::Run;
        if(ImGui::Button(pauseFlag ? "Pause" : "Unpause"))
            if(pauseFlag)
                _scene->setState(SceneState::Pause);
            else
                _scene->setState(SceneState::Unpause);
        if(!playFlag) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        ImGui::End();
    }

    void GUIViewportPanel::selectEntity(Entity& entity, Entity& selectedEntity) {
        unselectEntity(selectedEntity);
        selectedEntity = entity;
        if(selectedEntity.hasComponent<PhysicsComponent>())
            selectedEntity.getComponent<PhysicsComponent>().setShowCollider(true);
    }

    void GUIViewportPanel::unselectEntity(Entity& entity) {
        if(entity.getSource() == entt::null)
            return;
        if(entity.hasComponent<PhysicsComponent>())
            entity.getComponent<PhysicsComponent>().setShowCollider(false);
        entity = {};
    }

    void GUIViewportPanel::setGizmoOperation(GizmoOperation operation) {
        _gizmoOperation = operation;
    }

    void GUIViewportPanel::setScene(Scene* scene) {
        _scene = scene;
    }

    bool GUIViewportPanel::getIsMouseDisabled() { return ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouse; }
}