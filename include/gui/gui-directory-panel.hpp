#ifndef GUI_DIRECTORY_PANEL_HPP
#define GUI_DIRECTORY_PANEL_HPP

#include "scene/scene.hpp"
#include "registry/registry.hpp"
#include "stream/script-creator.hpp"
#include "gui/gui-components.hpp"
#include "gui/gui-drag-and-drop-type.hpp"
#include "stream/registry-recorder.hpp"
#include "renderer/texture.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace TWE {
    class GUIDirectoryPanel {
    public:
        GUIDirectoryPanel();
        GUIDirectoryPanel(const std::string& rootPath);
        void showPanel();
        void setScene(Scene* scene);
        void setRootPath(const std::string& rootPath);
    private:
        void loadTextures();
        void showDirectoryMenuPopup(const std::string& popupId);
        void showDirectoryFileMenuPopup(const std::string& popupId, std::filesystem::path& filePath);
        bool renderContent();
        std::filesystem::path _rootPath;
        std::filesystem::path _curPath;
        Scene* _scene;
        Texture* _dirTexture;
        Texture* _fileTexture;
        std::string _dirImgPath;
        std::string _fileImgPath;
    };
}

#endif