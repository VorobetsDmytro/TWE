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
        _guiState.scene = nullptr;
        _guiState.isMouseOnViewport = false;

        initPanels();
        
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

    void GUI::initPanels() {
        _startPanel = std::make_unique<GUIStartPanel>();
        _panels.emplace_back(std::make_unique<GUIComponentsPanel>());
        _panels.emplace_back(std::make_unique<GUIConsolePanel>());
        _panels.emplace_back(std::make_unique<GUIDirectoryPanel>());
        _panels.emplace_back(std::make_unique<GUIScenePanel>());
        _panels.emplace_back(std::make_unique<GUIViewportPanel>());
        for(auto& panel : _panels)
            panel->setGUIState(&_guiState);
    }

    void GUI::showMenuBar() {
        if(ImGui::BeginMenuBar()) {
            if(ImGui::BeginMenu("File")) {
                bool hasBGFuncsInRun = !_guiState.bgFuncsInRun.empty();
                if(hasBGFuncsInRun) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if(ImGui::MenuItem("New project"))
                    ImGuiFileDialog::Instance()->OpenDialog("NewProject", "Choose File", ".project", 
                        (_guiState.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                if(ImGui::MenuItem("Open project"))
                    ImGuiFileDialog::Instance()->OpenDialog("OpenProject", "Choose File", ".project", 
                        (_guiState.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                ImGui::Separator();

                bool validateScriptsDisabledFlag = _guiState.scene->getSceneState() != SceneState::Edit;
                if(validateScriptsDisabledFlag) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if(ImGui::MenuItem("Save scene")) {
                    if(!_guiState.projectData->lastScenePath.empty())
                        if(SceneSerializer::serialize(_guiState.scene, _guiState.projectData->lastScenePath.string(), _guiState.projectData))
                            _guiState.preUndoCount = _guiState.scene->getSceneRegistry()->edit.urControl.getUndoStack().size();
                    else
                        ImGuiFileDialog::Instance()->OpenDialog("SaveSceneAs", "Choose File", ".scene", 
                            (_guiState.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                }
                if(ImGui::MenuItem("Save scene as"))
                    ImGuiFileDialog::Instance()->OpenDialog("SaveSceneAs", "Choose File", ".scene", 
                        (_guiState.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                ImGui::Separator();

                if(ImGui::MenuItem("Validate scripts")) {
                    std::shared_future<void> validScriptsFuture = std::async(std::launch::async, [&]() {
                        _guiState.scene->getSceneScripts()->validateScripts(_guiState.scene);
                    });
                    _guiState.bgFuncsInRun.push_back(validScriptsFuture);
                }

                ImGui::Separator();
                if(ImGui::MenuItem("Build"))
                    if(!_guiState.projectData->lastScenePath.empty()) {
                        std::shared_future<void> buildFuture = std::async(std::launch::async, [&]() {
                            std::cout << "Start building the project\n";
                            if(BuildCreator::create(_guiState.projectData, _guiState.projectData->lastScenePath))
                                std::cout << "The project has been built successfully!\n\n";
                            else
                                std::cout << "The project building has been failed!\n\n";
                        });
                        _guiState.bgFuncsInRun.push_back(buildFuture);
                    }

                if(validateScriptsDisabledFlag || hasBGFuncsInRun) {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Options")) {
                bool isVSyncOn = _guiState.window->getIsVSyncOn();
                if(ImGui::MenuItem("VSync", "", isVSyncOn))
                    _guiState.window->setVSync(!isVSyncOn);
                bool isFocusedOnDebugCamera = _guiState.scene->getIsFocusedOnDebugCamera();
                if(ImGui::MenuItem("Is focused on debug camera", "", isFocusedOnDebugCamera))
                    _guiState.scene->setIsFocusedOnDebugCamera(!isFocusedOnDebugCamera);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void GUI::showDockSpace() {
        static bool opt_dockspace = true;
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags windowFlags =  ImGuiWindowFlags_NoDocking;
        if(!_guiState.projectData->rootPath.empty())
            windowFlags |= ImGuiWindowFlags_MenuBar;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        if(dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &opt_dockspace, windowFlags);
        ImGui::PopStyleVar(3);
        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }
        if(_guiState.projectData->rootPath.empty()) {
            _startPanel->showPanel();
            ImGui::End();
            return;
        }
        if(!isSelectedEntityValid())
            _guiState.selectedEntity = {};
            
        showMenuBar();
        if((io.ConfigFlags & ImGuiConfigFlags_NoMouse) && io.MouseClicked[0])
            io.MouseClicked[0] = false;

        for(auto& panel : _panels)
            panel->showPanel();

        processInput();
        processBGFuncs();
        processUR();
        ImGui::End();
    }

    void GUI::selectEntity(Entity& entity) {
        unselectEntity();
        _guiState.selectedEntity = entity;
        if(_guiState.selectedEntity.hasComponent<PhysicsComponent>())
            _guiState.selectedEntity.getComponent<PhysicsComponent>().setShowCollider(true);
    }

    void GUI::unselectEntity() {
        if(_guiState.selectedEntity.getSource() == entt::null)
            return;
        if(_guiState.selectedEntity.hasComponent<PhysicsComponent>())
            _guiState.selectedEntity.getComponent<PhysicsComponent>().setShowCollider(false);
        _guiState.selectedEntity = {};
    }

    bool GUI::isSelectedEntityValid() {
        return _guiState.scene->getSceneStateSpecification()->entityRegistry.valid(_guiState.selectedEntity.getSource());
    }

    void GUI::showFileDialog() {
        if(ImGuiFileDialog::Instance()->Display("NewProject"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string projectName = std::filesystem::path(filePathName).stem().string();
                std::string projectDir = std::filesystem::path(filePathName).parent_path().string();
                if(ProjectCreator::create(projectName, projectDir)) {
                    unselectEntity();
                    _guiState.scene->getScriptDLLRegistry()->clean();
                    std::string projectFilePath = projectDir + '/' + projectName + '/' + projectName + ".project";
                    auto rootPath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    auto projectData = ProjectCreator::load(projectFilePath, _guiState.scene->getScriptDLLRegistry(), rootPath + "/" + projectName);
                    if(projectData) {
                        std::filesystem::path dllTempDir = projectData->rootPath.string() + '/' + projectData->dllTempDir.string();
                        DLLCreator::initPaths(dllTempDir.string());
                        *_guiState.projectData = *projectData;
                        if(!projectData->lastScenePath.empty())
                            SceneSerializer::deserialize(_guiState.scene, projectData->lastScenePath.string(), projectData);
                        _guiState.preUndoCount = 0;
                        _guiState.undoCount = 0;
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
                _guiState.scene->getScriptDLLRegistry()->clean();
                auto rootPath = ImGuiFileDialog::Instance()->GetCurrentPath();
                auto projectData = ProjectCreator::load(projectFilePath, _guiState.scene->getScriptDLLRegistry(), rootPath);
                if(projectData) {
                    projectData->rootPath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    std::filesystem::path dllTempDir = projectData->rootPath.string() + '/' + projectData->dllTempDir.string();
                    DLLCreator::initPaths(dllTempDir.string());
                    *_guiState.projectData = *projectData;
                    if(!projectData->lastScenePath.empty() && std::filesystem::exists(projectData->rootPath / projectData->lastScenePath)) {
                        std::filesystem::path lastScenePath = projectData->rootPath.string() + '/' + projectData->lastScenePath.string();
                        SceneSerializer::deserialize(_guiState.scene, lastScenePath.string(), projectData);
                        _guiState.preUndoCount = 0;
                        _guiState.undoCount = 0;
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
        if(ImGuiFileDialog::Instance()->Display("SaveSceneAs"))  {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                if(SceneSerializer::serialize(_guiState.scene, filePathName, _guiState.projectData)) {
                    _guiState.projectData->lastScenePath = std::filesystem::relative(filePathName, _guiState.projectData->rootPath);
                    ProjectCreator::save(_guiState.projectData, _guiState.scene->getScriptDLLRegistry());
                    _guiState.preUndoCount = _guiState.scene->getSceneRegistry()->edit.urControl.getUndoStack().size();
                }
            }
            ImGuiFileDialog::Instance()->Close();
            return;
        }
    }

    void GUI::processInput() {
        if(!_guiState.bgFuncsInRun.empty())
            return;
        static std::vector<int> keysDown;
        auto hasFunc = [&](int key) {
            return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
        };
        ImGuiIO& io = ImGui::GetIO();
        keysDown.erase(std::remove_if(keysDown.begin(), keysDown.end(), [&](int key) {
            return !io.KeysDown[key];
        }), keysDown.end());

        if(_guiState.isFocusedOnViewport && !Input::isMouseButtonPressed(Mouse::MOUSE_BUTTON_RIGHT) 
        && _guiState.selectedEntity.getSource() != entt::null) {
            if(io.KeysDown[Keyboard::KEY_Q] && !hasFunc(Keyboard::KEY_Q)) {
                _guiState.gizmoOperation = GizmoOperation::None;
                keysDown.push_back(Keyboard::KEY_Q);
            }
            if(io.KeysDown[Keyboard::KEY_W] && !hasFunc(Keyboard::KEY_W)) {
                _guiState.gizmoOperation = GizmoOperation::Translate;
                keysDown.push_back(Keyboard::KEY_W);
            }
            if(io.KeysDown[Keyboard::KEY_E] && !hasFunc(Keyboard::KEY_E)) {
                _guiState.gizmoOperation = GizmoOperation::Rotate;
                keysDown.push_back(Keyboard::KEY_E);
            }
            if(io.KeysDown[Keyboard::KEY_R] && !hasFunc(Keyboard::KEY_R)) {
                _guiState.gizmoOperation = GizmoOperation::Scale;
                keysDown.push_back(Keyboard::KEY_R);
            }
        }
        if(io.KeysDown[Keyboard::KEY_DELETE] && !hasFunc(Keyboard::KEY_DELETE) && _guiState.selectedEntity.getSource() != entt::null) {
            auto removeEntity = _guiState.selectedEntity;
            unselectEntity();
            _guiState.scene->getSceneRegistry()->current->urControl.execute(new RemoveEntityCommand(removeEntity, 
                [&](){
                    unselectEntity();
                }
            ));
            keysDown.push_back(Keyboard::KEY_DELETE);
        }
        if(io.KeysDown[Keyboard::KEY_LEFT_CONTROL]) {
            if(io.KeysDown[Keyboard::KEY_Z] && !hasFunc(Keyboard::KEY_Z)) {
                _guiState.scene->getSceneRegistry()->current->urControl.undo();
                keysDown.push_back(Keyboard::KEY_Z);
            }
            if(io.KeysDown[Keyboard::KEY_Y] && !hasFunc(Keyboard::KEY_Y)) {
                _guiState.scene->getSceneRegistry()->current->urControl.redo();
                keysDown.push_back(Keyboard::KEY_Y);
            }
            if(io.KeysDown[Keyboard::KEY_D] && !hasFunc(Keyboard::KEY_D) && _guiState.selectedEntity.getSource() != entt::null) {
                _guiState.scene->getSceneRegistry()->current->urControl.execute(new CopyEntityCommand(_guiState.selectedEntity, {}, [&](){ unselectEntity(); }));
                keysDown.push_back(Keyboard::KEY_D);
            }
            if(io.KeysDown[Keyboard::KEY_S] && !hasFunc(Keyboard::KEY_S)) {
                if(!_guiState.projectData->lastScenePath.empty()) {
                    std::filesystem::path scenePath = _guiState.projectData->rootPath.string() + '/' + _guiState.projectData->lastScenePath.string();
                    if(SceneSerializer::serialize(_guiState.scene, scenePath.string(), _guiState.projectData))
                        _guiState.preUndoCount = _guiState.scene->getSceneRegistry()->edit.urControl.getUndoStack().size();
                }
                else
                    ImGuiFileDialog::Instance()->OpenDialog("SaveSceneAs", "Choose File", ".scene", 
                        (_guiState.projectData->rootPath.string() + '/').c_str(), 1, nullptr);
                keysDown.push_back(Keyboard::KEY_S);
            }
        }
        if(Input::getShowCursor() && io.ConfigFlags & ImGuiConfigFlags_NoMouse)
            io.ConfigFlags ^= ImGuiConfigFlags_NoMouse;
        else if(!Input::getShowCursor() && !(io.ConfigFlags & ImGuiConfigFlags_NoMouse))
            io.ConfigFlags ^= ImGuiConfigFlags_NoMouse;
    }

    void GUI::processBGFuncs() {
        if(_guiState.bgFuncsInRun.empty())
            return;
        static std::vector<int> indexesForRemoving;
        int size = _guiState.bgFuncsInRun.size();
        for(int i = 0; i < size; ++i) {
            auto status = _guiState.bgFuncsInRun[i].wait_for(std::chrono::milliseconds{0});
            if(status == std::future_status::ready)
                indexesForRemoving.push_back(i);
        }
        if(indexesForRemoving.empty())
            return;
        int sizeForRemoving = indexesForRemoving.size();
        for(int i = sizeForRemoving - 1; i >= 0; --i)
            _guiState.bgFuncsInRun.erase(_guiState.bgFuncsInRun.begin() + i);
        indexesForRemoving.clear();
    }

    void GUI::processUR() {
        _guiState.undoCount = _guiState.scene->getSceneRegistry()->edit.urControl.getUndoStack().size();
    }

    void GUI::setScene(IScene* scene) {
        _guiState.scene = scene;
    }

    void GUI::setWindow(Window* window) {
        _guiState.window = window;
    }

    void GUI::setProjectData(ProjectData* projectData) {
        _guiState.projectData = projectData;
    }

    bool GUI::getHasBGFuncs() { return !_guiState.bgFuncsInRun.empty(); }
    bool GUI::getIsMouseOnViewport() { return _guiState.isMouseOnViewport; }
    bool GUI::getIsFocusedOnViewport() { return _guiState.isFocusedOnViewport; }
    bool GUI::getIsMouseDisabled() { return ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NoMouse; }
    bool GUI::getIsURExecuted() { return _guiState.undoCount != _guiState.preUndoCount; }
}