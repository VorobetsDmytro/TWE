#include "gui/gui-directory-panel.hpp"

namespace TWE {
    GUIDirectoryPanel::GUIDirectoryPanel() {
        _curPath = "";
        _guiState = nullptr;
        loadTextures();
    }

    void GUIDirectoryPanel::loadTextures() {
        _dirImgPath = "../../images/folder.png";
        _fileImgPath = "../../images/file.png";
        _dirTexture = new Texture();
        _dirTexture->setTexture(TextureSpecification{_dirImgPath, 0, TextureType::Texture2D, TextureInOutFormat::RGBA});
        _fileTexture = new Texture();
        _fileTexture->setTexture(TextureSpecification{_fileImgPath, 0, TextureType::Texture2D, TextureInOutFormat::RGBA});
    }

    void GUIDirectoryPanel::setGUIState(GUIStateSpecification* guiState) {
        _guiState = guiState;
    }

    void GUIDirectoryPanel::showPanel() {
        ImGui::Begin("Directory");
        if(!_guiState) {
            ImGui::End();
            return;
        }

        bool isInteracted = renderContent();

        std::string directoryMenuPopup = guiPopups[GUIPopupIds::DirectoryMenuPopup];
        if(!isInteracted && ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
            ImGui::SetWindowFocus();
            ImGui::OpenPopup(directoryMenuPopup.c_str());
        }
        showDirectoryMenuPopup(directoryMenuPopup);
        ImGui::End();
    }

    void GUIDirectoryPanel::setRootPath(std::filesystem::path& rootPath) {
        _rootPath = _guiState->projectData->rootPath;
        _curPath = _rootPath;
    }

    bool GUIDirectoryPanel::renderContent() {
        bool isInteracted = false;
        int cleanPos = _guiState->projectData->rootPath.string().find(_guiState->projectData->rootPath.filename().string());
        if(!_guiState->projectData)
            return false;
        if(_curPath.empty() && _rootPath.empty())
            setRootPath(_guiState->projectData->rootPath);
        if(_rootPath != _guiState->projectData->rootPath)
            setRootPath(_guiState->projectData->rootPath);
        if(!std::filesystem::exists(_curPath))
            if(std::filesystem::exists(_guiState->projectData->rootPath))
                setRootPath(_guiState->projectData->rootPath);
            else
                _curPath = "./";
        std::string curPath = _curPath.string().substr(cleanPos, _curPath.string().length());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 10.f });
        std::string curPathTitle = "Current path: " + curPath;
        ImGui::Text(curPathTitle.c_str());
        ImGui::PopStyleVar();
        if(_curPath != _guiState->projectData->rootPath) {
            ImGui::SameLine();
            float backBtnHeight = 15.f;
            if(ImGui::Button("Back"))
                _curPath = _curPath.parent_path();
        }
        ImGui::Separator();

        static const float contentCellSize = 75.f;
        static const float padding = 16.f;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = std::max(static_cast<int>(panelWidth / contentCellSize), 1);

        ImGui::Columns(columns, 0, false);
        auto dirIt = std::filesystem::directory_iterator(_curPath);
        ImTextureID dirTextureId = (void*)(uint64_t)_dirTexture->getId(0);
        ImTextureID fileTextureId = (void*)(uint64_t)_fileTexture->getId(0);
        static std::filesystem::path fileMenuPath;
        std::string directoryFileMenuPopup = guiPopups[GUIPopupIds::DirectoryFileMenuPopup];
        int i = 0;
        std::vector<std::filesystem::directory_entry> entries;
        for(auto& entry : dirIt)
            entries.push_back(entry);  
        std::sort(entries.begin(), entries.end(), [](const std::filesystem::directory_entry& entry1, const std::filesystem::directory_entry& entry2) {
            return entry1.is_directory() && !entry2.is_directory();
        });
        for(auto& entry : entries) {
            auto& path = entry.path();
            ImGui::PushID(i++);

            std::string fileName = path.filename().string();
            ImGui::ImageButton(entry.is_directory() ? dirTextureId : fileTextureId, { contentCellSize - padding, contentCellSize - padding }, {0, 1}, {1, 0});
            bool canDragAndDrop = _guiState->bgFuncsInRun.empty();
            if(canDragAndDrop && ImGui::BeginDragDropSource()) {
                const wchar_t* item = path.c_str();
                ImGui::SetDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::DirectoryItem].c_str(), item, (wcslen(item) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }
            ImGui::PopID();
            if(ImGui::IsItemHovered()) {
                if(ImGui::IsMouseDoubleClicked(0)) {
                    ImGui::SetWindowFocus();
                    isInteracted = true;
                    std::string extension = path.extension().string();
                    if(entry.is_directory())
                        _curPath /= path.filename();
                    else if(extension == ".scene") {
                        if(!_guiState->bgFuncsInRun.empty() || _guiState->undoCount != _guiState->preUndoCount)
                            return isInteracted;
                        _guiState->selectedEntity = {};
                        SceneSerializer::deserialize(_guiState->scene, path.string(), _guiState->projectData);
                        _guiState->projectData->lastScenePath = std::filesystem::relative(path, _guiState->projectData->rootPath);
                        ProjectCreator::save(_guiState->projectData, _guiState->scene->getScriptDLLRegistry());
                        _guiState->undoCount = 0;
                        _guiState->preUndoCount = 0;
                    }
                } else if(ImGui::IsMouseClicked(1)) {
                    ImGui::SetWindowFocus();
                    isInteracted = true;
                    fileMenuPath = path;
                    ImGui::OpenPopup(directoryFileMenuPopup.c_str());
                }
            }
            ImGui::TextWrapped(fileName.c_str());

            ImGui::NextColumn();
        }
        showDirectoryFileMenuPopup(directoryFileMenuPopup, fileMenuPath);
        return isInteracted;
    }

    void GUIDirectoryPanel::showDirectoryFileMenuPopup(const std::string& popupId, std::filesystem::path& filePath) {
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popupId.c_str())) {
            auto& availSize = ImGui::GetContentRegionAvail();
            std::string fileExtension = filePath.extension().string();
            if(fileExtension == ".scene") {
                bool cantLoadScene = !_guiState->bgFuncsInRun.empty() || _guiState->undoCount != _guiState->preUndoCount;
                if(cantLoadScene) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if(ImGui::Button("Load scene", {availSize.x, 0.f})) {
                    _guiState->selectedEntity = {};
                    SceneSerializer::deserialize(_guiState->scene, filePath.string(), _guiState->projectData);
                    _guiState->projectData->lastScenePath = std::filesystem::relative(filePath, _guiState->projectData->rootPath);
                    ProjectCreator::save(_guiState->projectData, _guiState->scene->getScriptDLLRegistry());
                    ImGui::CloseCurrentPopup();
                    _guiState->undoCount = 0;
                    _guiState->preUndoCount = 0;
                }
                if(cantLoadScene) {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
            }
            bool validateScriptFlag = _guiState->scene->getSceneState() != SceneState::Edit || !_guiState->bgFuncsInRun.empty();
            if(fileExtension == ".hpp") {
                if(validateScriptFlag) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if(ImGui::Button("Validate", {availSize.x, 0.f})) {
                    static std::string scriptName;
                    scriptName = filePath.stem().string();
                    std::shared_future<void> validScriptsFuture = std::async(std::launch::async, [&]() {
                        _guiState->scene->getSceneScripts()->validateScript(scriptName, _guiState->scene);
                    });
                    _guiState->bgFuncsInRun.push_back(validScriptsFuture);
                    ImGui::CloseCurrentPopup();
                }
                if(validateScriptFlag) {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
            }
            if(validateScriptFlag) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            if(ImGui::Button("Show in explorer", {availSize.x, 0.f})) {
                std::string cmdCommand = "explorer " + std::filesystem::absolute(filePath).parent_path().string();
                system(cmdCommand.c_str());
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::Button("Remove", {availSize.x, 0.f})) {
                if(fileExtension == ".hpp") {
                    std::string fileName = filePath.stem().string();
                    if(_guiState->scene->getScriptDLLRegistry()->has(fileName)) {
                        auto scriptDLLData = _guiState->scene->getScriptDLLRegistry()->get(fileName);
                        if(scriptDLLData) {
                            auto sceneScripts = _guiState->scene->getSceneScripts();
                            sceneScripts->unbindScript(&_guiState->scene->getSceneRegistry()->edit.entityRegistry, scriptDLLData, _guiState->scene);
                            sceneScripts->unbindScript(&_guiState->scene->getSceneRegistry()->run.entityRegistry, scriptDLLData, _guiState->scene);
                            DLLCreator::freeDLLFunc(*scriptDLLData);
                            _guiState->scene->getScriptDLLRegistry()->erase(fileName);
                            DLLCreator::removeScript(fileName);
                            ProjectCreator::save(_guiState->projectData, _guiState->scene->getScriptDLLRegistry());
                        }
                    }
                }
                if(std::filesystem::is_directory(filePath) && !std::filesystem::is_empty(filePath))
                    std::cout << "Not empty folder can't be deleted.\n";
                else
                    std::filesystem::remove(filePath);
                ImGui::CloseCurrentPopup();
            }
            if(validateScriptFlag) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
            ImGui::EndPopup();
        }
    }

    void GUIDirectoryPanel::showDirectoryMenuPopup(const std::string& popupId) {
        float popUpWidth = 150.f;
        ImGui::SetNextWindowSize({popUpWidth, 0.f});
        if(ImGui::BeginPopup(popupId.c_str())) {
            auto& availSize = ImGui::GetContentRegionAvail();
            if(ImGui::BeginMenu("Create folder")) {
                static std::string createFolderName;
                if(GUIComponents::inputAndButton("Folder name", createFolderName, "Create")) {
                    std::filesystem::create_directory(_curPath / createFolderName);
                    createFolderName.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Create scene")) {
                static std::string sceneName;
                if(GUIComponents::inputAndButton("Scene name", sceneName, "Create")) {
                    IScene* newScene = new IScene();
                    SceneSerializer::serialize(newScene, _curPath.string() + '/' + sceneName + ".scene", _guiState->projectData);
                    delete newScene;
                    sceneName.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Create script")) {
                static std::string scriptName;
                if(GUIComponents::inputAndButton("Script name", scriptName, "Create")) {
                    auto checkScriptUniqName = _guiState->scene->getScriptDLLRegistry()->get(scriptName);
                    if(!checkScriptUniqName && !scriptName.empty()) {
                        std::string scriptDirectoryPath = _curPath.string();
                        if(ScriptCreator::create(scriptName, scriptDirectoryPath)) {
                            auto dllData = new DLLLoadData(DLLCreator::compileScript(scriptName, scriptDirectoryPath));
                            if(dllData->isValid)
                                _guiState->scene->getScriptDLLRegistry()->add(scriptName, dllData);
                            ProjectCreator::save(_guiState->projectData, _guiState->scene->getScriptDLLRegistry());
                            scriptName.clear();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }
}