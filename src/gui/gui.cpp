#include "gui/gui.hpp"

namespace TWE {
    GUI::GUI(GLFWwindow *window) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF("../../fonts/Roboto-Bold.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        _specification._scene = nullptr;
        _specification.isMouseOnViewport = false;

        _start.initialize();
        
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".scene", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
        ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".project", ImVec4(1.0f, 1.0f, 0.0f, 1.f));
    }

    GUI::~GUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUI::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void GUI::end() {
        showDockSpace();
        showFileDialog();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUI::showDockSpace() {
        static bool opt_dockspace = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoDocking;
        if(!_specification.projectData->rootPath.empty())
            window_flags |= ImGuiWindowFlags_MenuBar;
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
        if(_specification.projectData->rootPath.empty()) {
            _start.showPanel();
            ImGui::End();
            return;
        }
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("New project"))
                    ImGuiFileDialog::Instance()->OpenDialog("NewProject", "Choose File", ".project", 
                        (_specification.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                if(ImGui::MenuItem("Open project"))
                    ImGuiFileDialog::Instance()->OpenDialog("OpenProject", "Choose File", ".project", 
                        (_specification.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                ImGui::Separator();
                if(ImGui::MenuItem("Save scene")) {
                    if(!_specification.projectData->lastScenePath.empty())
                        SceneSerializer::serialize(_specification._scene, _specification.projectData->lastScenePath.string(), _specification.projectData);
                    else
                        ImGuiFileDialog::Instance()->OpenDialog("SaveSceneAs", "Choose File", ".scene", 
                            (_specification.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                }
                if(ImGui::MenuItem("Save scene as"))
                    ImGuiFileDialog::Instance()->OpenDialog("SaveSceneAs", "Choose File", ".scene", 
                        (_specification.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                ImGui::Separator();
                bool validateScriptsFlag = _specification._scene->_sceneState != SceneState::Edit;
                if(validateScriptsFlag) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if(ImGui::MenuItem("Validate scripts"))
                    _specification._scene->getSceneScripts()->validateScripts(_specification._scene);
                if(validateScriptsFlag) {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Build"))
                    if(!_specification.projectData->lastScenePath.empty())
                        BuildCreator::create(_specification.projectData, _specification.projectData->lastScenePath);
                ImGui::EndMenu();
            }
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
        if((io.ConfigFlags & ImGuiConfigFlags_NoMouse) && io.MouseClicked[0])
            io.MouseClicked[0] = false;
        showTestPanel();
        _viewport.showPanel(_specification._selectedEntity, _specification.isFocusedOnViewport, _specification.isMouseOnViewport);
        _scene.showPanel(_specification._selectedEntity);
        _directory.showPanel(_specification._selectedEntity);
        _components.showPanel(_specification._selectedEntity);
        processInput();
        ImGui::End();
    }

    void GUI::selectEntity(Entity& entity) {
        unselectEntity();
        _specification._selectedEntity = entity;
        if(_specification._selectedEntity.hasComponent<PhysicsComponent>())
            _specification._selectedEntity.getComponent<PhysicsComponent>().setShowCollider(true);
    }

    void GUI::unselectEntity() {
        if(_specification._selectedEntity.getSource() == entt::null)
            return;
        if(_specification._selectedEntity.hasComponent<PhysicsComponent>())
            _specification._selectedEntity.getComponent<PhysicsComponent>().setShowCollider(false);
        _specification._selectedEntity = {};
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

    void GUI::showFileDialog() {
        if(ImGuiFileDialog::Instance()->Display("NewProject"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string projectName = std::filesystem::path(filePathName).stem().string();
                std::string projectDir = std::filesystem::path(filePathName).parent_path().string();
                if(ProjectCreator::create(projectName, projectDir)) {
                    unselectEntity();
                    _specification._scene->_scriptDLLRegistry->clean();
                    std::string projectFilePath = projectDir + '/' + projectName + '/' + projectName + ".project";
                    auto projectData = ProjectCreator::load(projectFilePath, _specification._scene->_scriptDLLRegistry);
                    if(projectData) {
                        std::filesystem::path dllTempDir = projectData->rootPath.string() + '/' + projectData->dllTempDir.string();
                        DLLCreator::initPaths(dllTempDir.string());
                        *_specification.projectData = *projectData;
                        _directory.setCurrentPath(projectData->rootPath);
                        if(!projectData->lastScenePath.empty())
                            SceneSerializer::deserialize(_specification._scene, projectData->lastScenePath.string());
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("OpenProject"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string projectFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
                unselectEntity();
                _specification._scene->_scriptDLLRegistry->clean();
                auto projectData = ProjectCreator::load(projectFilePath, _specification._scene->_scriptDLLRegistry);
                if(projectData) {
                    std::filesystem::path dllTempDir = projectData->rootPath.string() + '/' + projectData->dllTempDir.string();
                    DLLCreator::initPaths(dllTempDir.string());
                    *_specification.projectData = *projectData;
                    _directory.setCurrentPath(projectData->rootPath);
                    if(!projectData->lastScenePath.empty() && std::filesystem::exists(projectData->rootPath / projectData->lastScenePath)) {
                        std::filesystem::path lastScenePath = projectData->rootPath.string() + '/' + projectData->lastScenePath.string();
                        SceneSerializer::deserialize(_specification._scene, lastScenePath.string());
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("SaveSceneAs"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                SceneSerializer::serialize(_specification._scene, filePathName, _specification.projectData);
                _specification.projectData->lastScenePath = filePathName;
                ProjectCreator::save(_specification.projectData, _specification._scene->_scriptDLLRegistry);
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("LoadModel"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                auto& modelEntity = Shape::createModelEntity(_specification._scene, filePathName);
                if(modelEntity.getSource() != entt::null) {
                    selectEntity(modelEntity);
                    _scene.addEntityToSelected(_specification._selectedEntity);
                    _specification._scene->_sceneRegistry.current->urControl.execute(new CreateEntityCommand(_specification._selectedEntity, 
                        [&](){ unselectEntity(); }));
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
                        selectEntity(Shape::createCubemapEntity(_specification._scene, attachemnts));
                        _scene.addEntityToSelected(_specification._selectedEntity);
                        _specification._scene->_sceneRegistry.current->urControl.execute(new CreateEntityCommand(_specification._selectedEntity, 
                            [&](){ unselectEntity(); }));
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
    }

    void GUI::processInput() {
        static std::vector<int> keysDown;
        auto hasFunc = [&](int key) {
            return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
        };
        ImGuiIO& io = ImGui::GetIO();
        keysDown.erase(std::remove_if(keysDown.begin(), keysDown.end(), [&](int key) {
            return !io.KeysDown[key];
        }), keysDown.end());

        if(_specification.isFocusedOnViewport && !Input::isMouseButtonPressed(Mouse::MOUSE_BUTTON_RIGHT) 
        && _specification._selectedEntity.getSource() != entt::null) {
            if(io.KeysDown[Keyboard::KEY_Q] && !hasFunc(Keyboard::KEY_Q)) {
                _viewport.setGizmoOperation(GizmoOperation::None);
                keysDown.push_back(Keyboard::KEY_Q);
            }
            if(io.KeysDown[Keyboard::KEY_W] && !hasFunc(Keyboard::KEY_W)) {
                _viewport.setGizmoOperation(GizmoOperation::Translate);
                keysDown.push_back(Keyboard::KEY_W);
            }
            if(io.KeysDown[Keyboard::KEY_E] && !hasFunc(Keyboard::KEY_E)) {
                _viewport.setGizmoOperation(GizmoOperation::Rotate);
                keysDown.push_back(Keyboard::KEY_E);
            }
            if(io.KeysDown[Keyboard::KEY_R] && !hasFunc(Keyboard::KEY_R)) {
                _viewport.setGizmoOperation(GizmoOperation::Scale);
                keysDown.push_back(Keyboard::KEY_R);
            }
        }
        if(io.KeysDown[Keyboard::KEY_DELETE] && !hasFunc(Keyboard::KEY_DELETE) && _specification._selectedEntity.getSource() != entt::null) {
            auto removeEntity = _specification._selectedEntity;
            unselectEntity();
            _specification._scene->_sceneRegistry.current->urControl.execute(new RemoveEntityCommand(removeEntity, 
                [&](){
                    unselectEntity();
                }
            ));
            keysDown.push_back(Keyboard::KEY_DELETE);
        }
        if(io.KeysDown[Keyboard::KEY_LEFT_CONTROL]) {
            if(io.KeysDown[Keyboard::KEY_Z] && !hasFunc(Keyboard::KEY_Z)) {
                _specification._scene->_sceneRegistry.current->urControl.undo();
                keysDown.push_back(Keyboard::KEY_Z);
            }
            if(io.KeysDown[Keyboard::KEY_Y] && !hasFunc(Keyboard::KEY_Y)) {
                _specification._scene->_sceneRegistry.current->urControl.redo();
                keysDown.push_back(Keyboard::KEY_Y);
            }
            if(io.KeysDown[Keyboard::KEY_D] && !hasFunc(Keyboard::KEY_D) && _specification._selectedEntity.getSource() != entt::null) {
                _specification._scene->_sceneRegistry.current->urControl.execute(new CopyEntityCommand(_specification._selectedEntity, {}, [&](){ unselectEntity(); }));
                keysDown.push_back(Keyboard::KEY_D);
            }
            if(io.KeysDown[Keyboard::KEY_S] && !hasFunc(Keyboard::KEY_S)) {
                if(!_specification.projectData->lastScenePath.empty()) {
                    std::filesystem::path scenePath = _specification.projectData->rootPath.string() + '/' + _specification.projectData->lastScenePath.string();
                    SceneSerializer::serialize(_specification._scene, scenePath.string(), _specification.projectData);
                }
                else
                    ImGuiFileDialog::Instance()->OpenDialog("SaveSceneAs", "Choose File", ".scene", 
                        (_specification.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                keysDown.push_back(Keyboard::KEY_S);
            }
        }
        if(Input::getShowCursor() && io.ConfigFlags & ImGuiConfigFlags_NoMouse)
            io.ConfigFlags ^= ImGuiConfigFlags_NoMouse;
        else if(!Input::getShowCursor() && !(io.ConfigFlags & ImGuiConfigFlags_NoMouse))
            io.ConfigFlags ^= ImGuiConfigFlags_NoMouse;
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
        _components.setScene(scene);
        _directory.setScene(scene);
        _start.setScene(scene);
        _scene.setScene(scene);
        _viewport.setScene(scene);
    }

    void GUI::setProjectData(ProjectData* projectData) {
        _specification.projectData = projectData;
        _directory.setProjectData(projectData);
    }

    bool GUI::getIsMouseOnViewport() { return _specification.isMouseOnViewport; }
    bool GUI::getIsFocusedOnViewport() { return _specification.isFocusedOnViewport; }
    bool GUI::getIsMouseDisabled() { return ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouse; }
}