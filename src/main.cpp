#include <iostream>
#include <memory>
#include "test.hpp"

int main(){
    std::unique_ptr<TWE::Engine> engine = std::make_unique<TWE::Test>(1024, 768, "The Worst Engine", nullptr, nullptr);
    engine->start();
    return 0;
}