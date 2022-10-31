#include "stream/file.hpp"

namespace TWE {
    std::string File::getBody(const char* filePath){
        std::ifstream is;
        is.open(filePath);
        if(!is.is_open()){
            std::cout << "A file by < " << filePath << " > path was not found." << std::endl;
            return {};
        }
        std::stringstream ss;
        ss << is.rdbuf();
        is.close();
        return ss.str();
    }
}