#ifndef DLL_CREATOR_HPP
#define DLL_CREATOR_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define TWE_PLATFORM_WINDOWS
#include "windows.h"
#else
#define TWE_DLL_CREATOR_ERROR
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace TWE {
    typedef void*(*PVoid)();

    struct DLLLoadData {
        DLLLoadData() = default;
        DLLLoadData(const std::string& dllPath, const std::string& factoryFuncName, const std::string& scriptName)
            : dllPath(dllPath), factoryFuncName(factoryFuncName), scriptName(scriptName) {}
        std::string dllPath;
        std::string factoryFuncName;
        std::string scriptName;
    };

    class DLLCreator {
    public:
        static DLLLoadData compileScript(const std::string& tempDir, const std::string& scriptName, const std::string& scriptDirectoryPath,
            const std::string& openCLSDKPath);
        [[nodiscard]] static PVoid loadDLLFunc(const std::string& dllPath, const std::string& factoryFuncName);
        [[nodiscard]] static PVoid loadDLLFunc(const DLLLoadData& loadData);
    private:
        static void createScriptDirectory(const std::string& tempDir, const std::string& scriptName);
        static void createCMakeFile(const std::string& tempDir, const std::string& scriptName, const std::string& openCLSDKPath);
        static void createCPPFile(const std::string& tempDir, const std::string& scriptName, const std::string& scriptDirectoryPath);
    };
}

#endif