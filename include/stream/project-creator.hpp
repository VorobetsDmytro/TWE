#ifndef PROJECT_CREATOR_HPP
#define PROJECT_CREATOR_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include <single_include/nlohmann/json.hpp>

#include "stream/file.hpp"

namespace TWE {
    struct ProjectData {
        ProjectData() = default;
        ProjectData(const std::string& projectName, const std::filesystem::path& rootPath, const std::filesystem::path& dllTempDir, const std::filesystem::path& lastScenePath)
            : projectName(projectName), rootPath(rootPath), dllTempDir(dllTempDir), lastScenePath(lastScenePath) {}
        std::string projectName;
        std::filesystem::path rootPath;
        std::filesystem::path dllTempDir;
        std::filesystem::path lastScenePath;
    };

    class ProjectCreator {
    public:
        static bool create(const std::string& projectName, const std::string& projectPath);
        static bool save(ProjectData* projectData);
        static ProjectData* load(const std::string& projectFilePath);
    private:
        static void createProjectFile(const std::string& projectName, const std::string& projectDirectoryPath);
        static bool validateProjectName(const std::string& projectName);
    };
}

#endif