#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class File {
public:
    static std::string getBody(const char* filePath);
};

#endif