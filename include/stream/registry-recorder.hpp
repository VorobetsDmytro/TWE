#ifndef REGISTRY_RECORDER_HPP
#define REGISTRY_RECORDER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace TWE {
    class RegistryRecorder {
    public:
        static void recordScript(const std::string& registryLoaderPath, const std::string& className, const std::string& scriptDirectoryPath);
    private:
        [[nodiscard]] static std::string trim(const std::string& str);
    };
}

#endif