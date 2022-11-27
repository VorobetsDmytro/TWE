#include "gui/gui.hpp"

namespace TWE {
    GUI::GUI(GLFWwindow *window) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        _specification._scene = nullptr;
        _specification._gizmoOperation = GizmoOperation::Translate;
        _specification.isFileDialogOpen = false;
    }

    GUI::~GUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUI::update() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        showDockSpace();
        processInput();
        showFileDialog();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUI::showDockSpace() {
        static bool opt_dockspace = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &opt_dockspace, window_flags);
        ImGui::PopStyleVar(3);
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                if(ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
                    dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; 
                if(ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                if(ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                if(ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                    dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        showScenePanel();
        showTestPanel();
        showDirectoryPanel();
        showViewportPanel();
        showComponentPanel();
        ImGui::End();
    }

    void GUI::showScenePanel() {
        ImGui::Begin("Scene");
        if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
            _specification._selectedEntity = {};
        if(_specification._scene) {
            _specification._scene->_registry->view<NameComponent>().each([&](entt::entity entity, NameComponent& nameComponent){
                auto id = (void*)entity;
                Entity entityInstance = {entity, _specification._scene};
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (_specification._selectedEntity == entityInstance ? ImGuiTreeNodeFlags_Selected : 0);
                bool isOpened = ImGui::TreeNodeEx(id, flags, nameComponent.getName().c_str());
                if(ImGui::IsItemClicked())
                    _specification._selectedEntity = entityInstance;
                if(isOpened)
                    ImGui::TreePop();
            });
        }
        ImGui::End();
    }

    void GUI::showTestPanel() {
        ImGui::Begin("Test");
        for(auto inputText : _inputTextes)
            ImGui::InputText(inputText.first, &inputText.second);
        for(auto button : _buttons)
            if(ImGui::Button(button.first))
                button.second();
        for(auto checkbox : _checkBoxes)
            ImGui::Checkbox(checkbox.first, &checkbox.second);
        ImGui::End();
    }

    void GUI::showViewportPanel() {
        ImGui::Begin("Viewport");
        
        auto& cursorPos = ImGui::GetCursorPos();
        _specification._scene->setFocusOnViewport(ImGui::IsWindowFocused());
        ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
        auto frameSize = _specification._scene->_frameBuffer->getSize();
        if(viewPortSize.x != frameSize.first || viewPortSize.y != frameSize.second)
            _specification._scene->_frameBuffer->resize(viewPortSize.x, viewPortSize.y);
        auto frameId = (void*)(uint64_t)_specification._scene->_frameBuffer->getColorAttachment(0);
        ImGui::Image(frameId, viewPortSize, {0, 1}, {1, 0});

        auto& windowSize = ImGui::GetWindowSize();
        auto& minBound = ImGui::GetWindowPos();
        minBound.x += cursorPos.x;
        minBound.y += cursorPos.y;

        auto& mousePos = ImGui::GetMousePos();
        mousePos.x -= minBound.x;
        mousePos.y -= minBound.y;
        mousePos.y = viewPortSize.y - mousePos.y;
        if(_specification._scene->getIsFocusedOnDebugCamera() && !_specification.isFileDialogOpen) {
            bool isUsing = showGizmo();
            if(!isUsing && ImGui::IsMouseClicked(0)) {
                if(mousePos.x >= 0.f && mousePos.y >= 0.f && mousePos.x < viewPortSize.x && mousePos.y < viewPortSize.y) {
                    int data = _specification._scene->_frameBuffer->readPixel(1, (int)mousePos.x, (int)mousePos.y);
                    _specification._selectedEntity = data == -1 || data < 0 || data >= _specification._scene->_registry->size() 
                        ? Entity() 
                        : Entity{ (entt::entity)data, _specification._scene };
                }
            }
        }
        ImGui::End();
    }

    void GUI::showDirectoryPanel() {
        ImGui::Begin("Directory");
        ImGui::End();
    }

    void GUI::showFileDialog() {
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))  {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            }
            _specification.isFileDialogOpen = false;
            ImGuiFileDialog::Instance()->Close();
        }
    }

    bool GUI::showGizmo() {
        if(_specification._gizmoOperation == GizmoOperation::None || _specification._selectedEntity.getSource() == entt::null)
            return false;
        Camera* camera = _specification._scene->_sceneCameraSpecification.camera;
        if(!camera)
            return false;
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        auto& windowPos = ImGui::GetWindowPos();
        auto& windowSize = ImGui::GetWindowSize();
        ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

        const glm::mat4& cameraProjection = camera->getProjection();
        glm::mat4 cameraView = camera->getView(_specification._scene->_sceneCameraSpecification.position, 
            _specification._scene->_sceneCameraSpecification.forward, _specification._scene->_sceneCameraSpecification.up);
        auto& selectedEntityTransform = _specification._selectedEntity.getComponent<TransformComponent>();
        auto selectedEntityModel = selectedEntityTransform.model;

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), static_cast<ImGuizmo::OPERATION>(_specification._gizmoOperation),
                             ImGuizmo::LOCAL, glm::value_ptr(selectedEntityModel));

        bool isUsing = ImGuizmo::IsUsing();

        if(isUsing) {
            glm::vec3 position, rotation, size;
            Math::decomposeTransform(selectedEntityModel, position, rotation, size);
            bool hasPhysics = _specification._selectedEntity.hasComponent<PhysicsComponent>();
            selectedEntityTransform.setPosition(position);
            if(hasPhysics)
                _specification._selectedEntity.getComponent<PhysicsComponent>().setPosition(position);
            selectedEntityTransform.setRotation(rotation);
            if(hasPhysics)
                _specification._selectedEntity.getComponent<PhysicsComponent>().setRotation(glm::quat(rotation));
            selectedEntityTransform.setSize(size);
            if(hasPhysics)
                _specification._selectedEntity.getComponent<PhysicsComponent>().setSize(_specification._scene->getDynamicWorld(), size);
        }

        return isUsing;
    }

    void GUI::processInput() {
        if(_specification._scene->getIsFocusedOnViewport() && !Input::isMouseButtonPressed(Mouse::MOUSE_BUTTON_RIGHT) 
        && _specification._selectedEntity.getSource() != entt::null) {
            if(Input::isKeyPressed(Keyboard::KEY_Q))
                _specification._gizmoOperation = GizmoOperation::None;
            if(Input::isKeyPressed(Keyboard::KEY_W))
                _specification._gizmoOperation = GizmoOperation::Translate;
            if(Input::isKeyPressed(Keyboard::KEY_E))
                _specification._gizmoOperation = GizmoOperation::Rotate;
            if(Input::isKeyPressed(Keyboard::KEY_R))
                _specification._gizmoOperation = GizmoOperation::Scale;
        }
    }

    void GUI::showComponentPanel() {
        ImGui::Begin("Components");
        if(_specification._selectedEntity.getSource() != entt::null) {
            showNameComponent();
            showTransformComponent();
            showMeshComponent();
            showMeshRendererComponent();
            showCameraComponent();
            showLightComponent();
            showPhysicsComponent();
            showScriptComponent();
        }
        ImGui::End();
    }

    void GUI::showNameComponent() {
        if(_specification._selectedEntity.hasComponent<NameComponent>()) {
            auto& nameComponent = _specification._selectedEntity.getComponent<NameComponent>();
            auto& name = nameComponent.getName();
            ImGui::Text("Name");
            ImGui::SameLine();
            if(ImGui::InputText("##Name", &name))
                nameComponent.setName(name);
        }
    }

    void GUI::showTransformComponent() {
        if(_specification._selectedEntity.hasComponent<TransformComponent>()) {
            auto id = (void*)(typeid(TransformComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Transform")) {
                auto& transformComponent = _specification._selectedEntity.getComponent<TransformComponent>();
                bool hasPhysics = _specification._selectedEntity.hasComponent<PhysicsComponent>();
                if(dragFloat3("Position", transformComponent.position, 0.05f, 0.f)) {
                    transformComponent.setPosition(transformComponent.position);
                    if(hasPhysics)
                        _specification._selectedEntity.getComponent<PhysicsComponent>().setPosition(transformComponent.position);
                }
                auto rotation = transformComponent.rotation * 180.f / PI;
                if(dragFloat3("Rotation", rotation, hasPhysics ? 1.5f : 0.5f, 0.f)) {
                    rotation *= PI / 180.f;
                    transformComponent.setRotation(rotation);
                    if(hasPhysics)
                        _specification._selectedEntity.getComponent<PhysicsComponent>().setRotation(rotation);
                }
                if(dragFloat3("Scale", transformComponent.size, 0.01f, 1.f, 0.01f, 999999.f)) {
                    transformComponent.setSize(transformComponent.size);
                    if(hasPhysics)
                        _specification._selectedEntity.getComponent<PhysicsComponent>()
                            .setSize(_specification._scene->getDynamicWorld(), transformComponent.size);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUI::showMeshComponent() {
        if(_specification._selectedEntity.hasComponent<MeshComponent>()) {
            auto id = (void*)(typeid(MeshComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh")) {
                auto& meshComponent = _specification._selectedEntity.getComponent<MeshComponent>();
                if(!meshComponent.textures.empty())
                    ImGui::Image((void*)(uint64_t)meshComponent.textures[0]->getId(), {30.f, 30.f});
                // if (ImGui::Button("Open File Dialog")) {
                //     ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png, .jpeg, .jpg", ".");
                //     _specification.isFileDialogOpen = true;
                // }
                ImGui::TreePop();
            }
        }
    }

    void GUI::showMeshRendererComponent() {
        if(_specification._selectedEntity.hasComponent<MeshRendererComponent>()) {
            auto id = (void*)(typeid(MeshRendererComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh Renderer")) {
                auto& meshRendererComponent = _specification._selectedEntity.getComponent<MeshRendererComponent>();
                if(ImGui::TreeNodeEx(id, flags, "Material")) {
                    colorEdit3("Color", meshRendererComponent.material->objColor);
                    dragFloat("Ambient", meshRendererComponent.material->ambient, 0.01f);
                    dragFloat("Diffuse", meshRendererComponent.material->diffuse, 0.1f, 0.1f, 999999.f);
                    dragFloat("Specular", meshRendererComponent.material->specular, 0.1f, 0.f, 999999.f);
                    dragFloat("Shininess", meshRendererComponent.material->shininess, 1.f, 12.f, 999999.f);
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUI::showCameraComponent() {
        if(_specification._selectedEntity.hasComponent<CameraComponent>()) {
            auto id = (void*)(typeid(CameraComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Camera")) {
                auto& cameraComponent = _specification._selectedEntity.getComponent<CameraComponent>();
                auto* cameraSource = cameraComponent.getSource();
                int typeIndex = combo("Projection", cameraProjectionTypes[cameraSource->getType()], cameraProjectionTypes, 105.f);
                if(typeIndex == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    cameraSource->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                        perspectiveSpecification.wndHeight, perspectiveSpecification.near, perspectiveSpecification.far);
                } else if(typeIndex == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    cameraSource->setOrthographic(orthographicSpecification.left, orthographicSpecification.right, 
                        orthographicSpecification.bottom, orthographicSpecification.top, orthographicSpecification.near, orthographicSpecification.far);
                }
                auto isFocusedOn = cameraComponent.isFocusedOn();
                if(checkBox("Is focused on", isFocusedOn, 105.f))
                    cameraComponent.setFocuse(isFocusedOn);
                if(cameraSource->getType() == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    if(dragFloat("FOV", perspectiveSpecification.fov, 1.f, 1.f, 140.f, 105.f))
                        cameraSource->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                            perspectiveSpecification.wndHeight, perspectiveSpecification.near, perspectiveSpecification.far);
                } else if(cameraSource->getType() == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    auto aspect = orthographicSpecification.right;
                    if(dragFloat("Aspect", aspect, 0.2f, 0.2f, 100.f, 105.f))
                        cameraSource->setOrthographic(-aspect, aspect, -aspect, aspect, 
                            orthographicSpecification.near, orthographicSpecification.far);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUI::showLightComponent() {
        if(_specification._selectedEntity.hasComponent<LightComponent>()) {
            auto id = (void*)(typeid(LightComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Light")) {
                auto& lightComponent = _specification._selectedEntity.getComponent<LightComponent>();
                std::string selectedType = lightTypes[lightComponent.type];
                int typeIndex = combo("Type", lightTypes[lightComponent.type], lightTypes, 100.f);
                if(typeIndex > -1)
                    lightComponent.setType(static_cast<LightType>(typeIndex));
                colorEdit3("Color", lightComponent.color, 100.f);
                if(lightComponent.type == LightType::SPOT) {
                    dragFloat("Inner radius", lightComponent.innerRadius, 0.2f, 0.f, 999999.f, 100.f);
                    dragFloat("Outer radius", lightComponent.outerRadius, 0.2f, 0.f, 999999.f, 100.f);
                }
                if(lightComponent.type == LightType::SPOT || lightComponent.type == LightType::POINT) {
                    dragFloat("Constant", lightComponent.constant, 0.05f, 0.01f, 999999.f, 100.f);
                    dragFloat("Linear", lightComponent.linear, 0.01f, 0.01f, 999999.f, 100.f);
                    dragFloat("Quadratic", lightComponent.quadratic, 0.001f, 0.001f, 999999.f, 100.f);
                }
                if(lightComponent.type == LightType::DIR) {
                    if(checkBox("Cast shadows", lightComponent.castShadows, 100.f))
                        lightComponent.setCastShadows(lightComponent.castShadows);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUI::showPhysicsComponent() {
        if(_specification._selectedEntity.hasComponent<PhysicsComponent>()) {
            auto id = (void*)(typeid(PhysicsComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Physics")) {
                auto& physicsComponent = _specification._selectedEntity.getComponent<PhysicsComponent>();
                auto mass = physicsComponent.getMass();
                if(dragFloat("Mass", mass, 0.01f, 0.f, 999999.f))
                    physicsComponent.setMass(_specification._scene->getDynamicWorld(), mass);
                ImGui::TreePop();
            }
        }
    }

    void GUI::showScriptComponent() {
        if(_specification._selectedEntity.hasComponent<ScriptComponent>()) {
            auto id = (void*)(typeid(ScriptComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Script")) {
                auto& scriptComponent = _specification._selectedEntity.getComponent<ScriptComponent>();
                text("Name", scriptComponent.getBehaviorClassName());
                ImGui::TreePop();
            }
        }
    }

    void GUI::text(const std::string& label, const std::string& value, float labelColumnWidth) {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        ImGui::Text(value.c_str());
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool GUI::checkBox(const std::string& label, bool& value, float labelColumnWidth) {
        bool isInteracted = false;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());
        
        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        if(ImGui::Checkbox("##Checkbox", &value))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    int GUI::combo(const std::string& label, std::string& selected, std::vector<std::string>& options, float labelColumnWidth) {
        int res = -1;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        int size = options.size();
        if(ImGui::BeginCombo("##Combo", selected.c_str())) {
            for(int i = 0; i < size; ++i) {
                bool isSelected = options[i] == selected;
                if(ImGui::Selectable(options[i].c_str(), isSelected))
                    res = i;
                if(isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    bool GUI::colorEdit3(const std::string& label, glm::vec3& values, float labelColumnWidth) {
        bool isInteracted = false;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        if(ImGui::ColorEdit3("##ColorEdit3", glm::value_ptr(values)))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    bool GUI::dragFloat(const std::string& label, float& value, float step, float min, float max, float labelColumnWidth) {
        bool isInteracted = false;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        if(ImGui::DragFloat("##Value", &value, step, min, max, "%.3f"))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    bool GUI::dragFloat3(const std::string& label, glm::vec3& values, float step, float resetValue, float min, float max, float labelColumnWidth) {
        bool isInteracted = false;
        ImGuiButtonFlags btnFlags = ImGuiButtonFlags_PressedOnDoubleClick;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 3.f });
        float height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 size = { height + 3.f, height };

        if(ImGui::ButtonEx("X", size, btnFlags)) {
            isInteracted = true;
            values.x = resetValue;
        }
        ImGui::SameLine();
        if(ImGui::DragFloat("##X", &values.x, step, min, max, "%.2f"))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if(ImGui::ButtonEx("Y", size, btnFlags)) {
            isInteracted = true;
            values.y = resetValue;
        }
        ImGui::SameLine();
        if(ImGui::DragFloat("##Y", &values.y, step, min, max, "%.2f"))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if(ImGui::ButtonEx("Z", size, btnFlags)) {
            isInteracted = true;
            values.z = resetValue;
        }
        ImGui::SameLine();
        if(ImGui::DragFloat("##Z", &values.z, step, min, max, "%.2f"))
            isInteracted = true;
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    void GUI::addCheckbox(const char* name, bool& var) {
        _checkBoxes.push_back({name, var});
    }

    void GUI::addInputText(const char* name, std::string& var) {
        _inputTextes.push_back({name, var});
    }

    void GUI::addButton(const char* name, std::function<void()> func) {
        _buttons.push_back({name, func});
    }

    void GUI::setScene(Scene* scene) {
        _specification._scene = scene;
    }
}