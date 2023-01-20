#include <iostream>
#include <memory>

#include "engine.hpp"

int main(){
    std::unique_ptr<TWE::Engine> engine = std::make_unique<TWE::Engine>(1366, 768, "The Worst Engine", nullptr, nullptr);
    #ifdef TWE_BUILD_FILE_PATH
    engine->start(TWE_BUILD_FILE_PATH);
    #else
    engine->start();
    #endif
    return 0;
}