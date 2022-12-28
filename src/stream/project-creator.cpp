#include "stream/project-creator.hpp"

namespace TWE {
    bool ProjectCreator::create(const std::string& projectName, const std::string& projectPath) {
        if(!validateProjectName(projectName))
            return false;
        if(!std::filesystem::exists(projectPath))
            return false;
        std::string projectDirectoryPath = projectPath + '/' + projectName;
        std::filesystem::create_directory(projectDirectoryPath);
        std::filesystem::create_directory(projectDirectoryPath + "/models");
        std::filesystem::create_directory(projectDirectoryPath + "/scenes");
        std::filesystem::create_directory(projectDirectoryPath + "/textures");
        std::filesystem::create_directory(projectDirectoryPath + "/scripts");
        std::filesystem::create_directory(projectDirectoryPath + "/scripts/temp");
        createProjectFile(projectName, projectDirectoryPath);
        return true;
    }

    ProjectData* ProjectCreator::load(const std::string& projectFilePath) {
        if(!std::filesystem::exists(projectFilePath))
            return nullptr;
        std::string jsonBodyStr = File::getBody(projectFilePath.c_str());
        nlohmann::json jsonMain = nlohmann::json::parse(jsonBodyStr);
        return new ProjectData(jsonMain["Project"], jsonMain["ProjectRootPath"], jsonMain["DLLTempDir"], jsonMain["LastScenePath"]);
    }

    bool ProjectCreator::save(ProjectData* projectData) {
        std::filesystem::path projectFilePath = projectData->rootPath.string() + '/' + projectData->projectName + ".project";
        if(!std::filesystem::exists(projectFilePath))
            return false;
        nlohmann::json jsonMain;
        jsonMain["Project"] = projectData->projectName;
        jsonMain["ProjectRootPath"] = projectData->rootPath;
        jsonMain["DLLTempDir"] = projectData->dllTempDir;
        jsonMain["LastScenePath"] = projectData->lastScenePath;
        File::save(projectFilePath.string().c_str(), jsonMain.dump());
        return true;
    }

    void ProjectCreator::createProjectFile(const std::string& projectName, const std::string& projectDirectoryPath) {
        nlohmann::json jsonMain;
        std::filesystem::path projectFilePath = projectDirectoryPath + '/' + projectName + ".project";

        jsonMain["Project"] = projectName;
        jsonMain["ProjectRootPath"] = std::filesystem::absolute(projectDirectoryPath).string();
        jsonMain["DLLTempDir"] = std::filesystem::absolute(projectDirectoryPath + "/scripts/temp").string();
        jsonMain["LastScenePath"] = "";

        File::save(projectFilePath.string().c_str(), jsonMain.dump());
    }

    bool ProjectCreator::validateProjectName(const std::string& projectName) {
        return std::regex_match(projectName, std::regex("^[a-zA-Z][a-zA-Z0-9]+"));
    }
}