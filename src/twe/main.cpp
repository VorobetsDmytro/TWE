#include <iostream>
#include "test.hpp"
#include <memory>

int main(){
    std::unique_ptr<Engine> engine = std::make_unique<Test>(1024, 768, "The Worst Engine", nullptr, nullptr);
    engine->start();
    return 0;
}