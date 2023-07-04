#ifndef SCRIPT_CREATOR_HPP
#define SCRIPT_CREATOR_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

namespace TWE {
    class ScriptCreator {
    public:
        static bool create(std::string scriptName, std::string scriptDirectoryPath);
    private:
        static bool validateScriptName(std::string scriptName);
    };
}

#endif