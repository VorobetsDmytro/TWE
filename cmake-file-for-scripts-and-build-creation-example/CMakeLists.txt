cmake_minimum_required(VERSION 3.20)
project(TWE_BUILD_LIB)

file(GLOB_RECURSE CPPFILES
    "../src/main.cpp"
    "../src/engine.cpp"
    "../src/entity/*.cpp"
    "../src/input/*.cpp"
    "../src/model-loader/*.cpp"
    "../src/renderer/*.cpp"
    "../src/scene/*.cpp"
    "../src/stream/*.cpp"
    "../src/undo-redo/ur-control.cpp"
    "../src/ui-build/*.cpp"
)

file(GLOB_RECURSE LIBFILES 
    "../external/*.lib" 
)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Debug" "${CMAKE_BINARY_DIR}/Release")

add_library(TWE_BUILD_LIB STATIC ${CPPFILES})
target_compile_features(TWE_BUILD_LIB PRIVATE cxx_std_17)
target_include_directories(TWE_BUILD_LIB PRIVATE
    "../include"
    "../external"
    "../external/GLFW"
    "../external/glad"
    "../external/stb"
    "../external/glm"
    "../external/assimp"
    "../external/imgui"
    "../external/bullet3"
    "../external/entt"
    "../external/json"
    "../external/irrKlang"
)
target_link_libraries(TWE_BUILD_LIB PRIVATE ${LIBFILES})
target_compile_definitions(TWE_BUILD_LIB PRIVATE TWE_BUILD)