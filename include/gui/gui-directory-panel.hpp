#ifndef GUI_DIRECTORY_PANEL_HPP
#define GUI_DIRECTORY_PANEL_HPP

#include "scene/iscene.hpp"
#include "scene/scene-serializer.hpp"

#include "registry/registry.hpp"

#include "stream/script-creator.hpp"
#include "stream/project-creator.hpp"

#include "gui/gui-components.hpp"
#include "gui/gui-types.hpp"
#include "gui/igui-panel.hpp"

#include "renderer/texture.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <algorithm>

namespace TWE {
    class GUIDirectoryPanel: public IGUIPanel {
    public:
        GUIDirectoryPanel();
        void showPanel() override;
        void setGUIState(GUIStateSpecification* guiState) override;
    private:
        void setRootPath(std::filesystem::path& rootPath);
        void loadScene(std::filesystem::path& scenePath);
        void loadTextures();
        void showDirectoryMenuPopup(const std::string& popupId);
        void showDirectoryFileMenuPopup(const std::string& popupId, std::filesystem::path& filePath);
        bool renderContent();
        std::filesystem::path _curPath;
        std::filesystem::path _rootPath;
        Texture* _dirTexture;
        Texture* _fileTexture;
        std::string _dirImgPath;
        std::string _fileImgPath;
        GUIStateSpecification* _guiState;
    };
}

#endif