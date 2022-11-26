#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace TWE {
    class File {
    public:
        static std::string getBody(const char* filePath);
        static void save(const char* filePath, const std::string& data);
    };
}

#endif