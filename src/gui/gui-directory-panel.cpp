#include "gui/gui-directory-panel.hpp"

namespace TWE {
    GUIDirectoryPanel::GUIDirectoryPanel() {
        _rootPath = "";
        _curPath = _rootPath;
        loadTextures();
    }

    GUIDirectoryPanel::GUIDirectoryPanel(const std::string& rootPath): _rootPath(rootPath) {
        _curPath = _rootPath;
        loadTextures();
    }

    void GUIDirectoryPanel::loadTextures() {
        _dirImgPath = "../../include/gui/images/folder.png";
        _fileImgPath = "../../include/gui/images/file.png";
        TextureSpecification dirSpec(_dirImgPath, 0, TextureType::Texture2D, TextureInOutFormat::RGBA);
        TextureSpecification fileSpec(_fileImgPath, 0, TextureType::Texture2D, TextureInOutFormat::RGBA);
        _dirTexture = new Texture();
        _dirTexture->setTexture(dirSpec);
        _fileTexture = new Texture();
        _fileTexture->setTexture(fileSpec);
    }

    void GUIDirectoryPanel::setRootPath(const std::string& rootPath) {
        _rootPath = rootPath;
        _curPath = _rootPath;
    }

    void GUIDirectoryPanel::showPanel() {
        ImGui::Begin("Directory");

        bool isInteracted = renderContent();

        std::string directoryMenuPopup = "DirectoryMenuPopup";
        if(!isInteracted && ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
            ImGui::SetWindowFocus();
            ImGui::OpenPopup(directoryMenuPopup.c_str());
        }
        showDirectoryMenuPopup(directoryMenuPopup);
        ImGui::End();
    }

    void GUIDirectoryPanel::setScene(Scene* scene) {
        _scene = scene;
    }

    bool GUIDirectoryPanel::renderContent() {
        bool isInteracted = false;
        int cleanPos = _rootPath.string().find(_rootPath.filename().string());
        std::string curPath = _curPath.string().substr(cleanPos, _curPath.string().length());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 10.f });
        std::string curPathTitle = "Current path: " + curPath;
        ImGui::Text(curPathTitle.c_str());
        ImGui::PopStyleVar();
        if(_curPath != _rootPath) {
            ImGui::SameLine();
            float backBtnHeight = 15.f;
            if(ImGui::Button("Back"))
                _curPath = _curPath.parent_path();
        }
        ImGui::Separator();

        float contentCellSize = 75.f;
        float padding = 16.f;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = max(static_cast<int>(panelWidth / contentCellSize), 1);

        ImGui::Columns(columns, 0, false);
        auto dirIt = std::filesystem::directory_iterator(_curPath);
        ImTextureID dirTextureId = (void*)(uint64_t)_dirTexture->getId(0);
        ImTextureID fileTextureId = (void*)(uint64_t)_fileTexture->getId(0);
        static std::filesystem::path fileMenuPath;
        std::string directoryFileMenuPopup = "DirectoryFileMenuPopup";
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
            if(ImGui::BeginDragDropSource()) {
                const wchar_t* item = path.c_str();
                ImGui::SetDragDropPayload(guiDragAndDropTypes[GUIDragAndDropType::DirectoryItem].c_str(), item, (wcslen(item) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }
            ImGui::PopID();
            if(ImGui::IsItemHovered()) {
                if(ImGui::IsMouseDoubleClicked(0)) {
                    ImGui::SetWindowFocus();
                    isInteracted = true;
                    if(entry.is_directory())
                        _curPath /= path.filename();
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
            if(fileExtension == ".hpp") {
                bool validateScriptFlag = _scene->_sceneState != SceneState::Edit;
                if(validateScriptFlag) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                if(ImGui::Button("Validate", {availSize.x, 0.f})) {
                    std::string fileName = filePath.stem().string();
                    _scene->validateScript(fileName);
                    ImGui::CloseCurrentPopup();
                }
                if(validateScriptFlag) {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
            }
            if(ImGui::Button("Remove", {availSize.x, 0.f})) {
                if(fileExtension == ".hpp") {
                    std::string fileName = filePath.stem().string();
                    if(_scene->_scriptDLLRegistry->has(fileName)) {
                        auto scriptDLLData = _scene->_scriptDLLRegistry->get(fileName);
                        if(scriptDLLData) {
                            _scene->unbindScript(&_scene->_entityRegistry.editEntityRegistry, scriptDLLData);
                            _scene->unbindScript(&_scene->_entityRegistry.runEntityRegistry, scriptDLLData);
                            DLLCreator::freeDLLFunc(*scriptDLLData);
                            _scene->_scriptDLLRegistry->erase(fileName);
                            _scene->_scriptRegistry->erase(fileName);
                            DLLCreator::removeScript(fileName);
                            RegistryRecorder::removeScript(fileName);
                        }
                    }
                }
                std::filesystem::remove_all(filePath);
                ImGui::CloseCurrentPopup();
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
            if(ImGui::BeginMenu("Create script")) {
                static std::string scriptName;
                if(GUIComponents::inputAndButton("Script name", scriptName, "Create")) {
                    auto checkScriptUniqName = _scene->_scriptRegistry->get(scriptName);
                    if(!checkScriptUniqName) {
                        std::string scriptDirectoryPath = _curPath.string();
                        if(ScriptCreator::create(scriptName, scriptDirectoryPath)) {
                            auto dllData = new DLLLoadData(DLLCreator::compileScript(scriptName, scriptDirectoryPath));
                            if(dllData->isValid) {
                                _scene->_scriptDLLRegistry->add(scriptName, dllData);
                                RegistryRecorder::recordScript(scriptName, scriptDirectoryPath);
                            }
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