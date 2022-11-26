cmake_minimum_required(VERSION 3.20)
project(TWE)

file(GLOB_RECURSE CPPFILES 
    "src/*.cpp" 
    "external/*.cpp" 
    "external/*.c"
)

file(GLOB_RECURSE LIBFILES 
    "external/*.lib" 
    "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.7/lib/*.lib" #Your OpenCL SDK lib path
)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Debug" "${CMAKE_BINARY_DIR}/Release")

add_executable(TWE ${CPPFILES})
target_compile_features(TWE PRIVATE cxx_std_17)
target_include_directories(TWE PRIVATE
    "include"
    "scripts" # Custom scripts
    "external"
    "external/GLFW"
    "external/glad"
    "external/stb"
    "external/glm"
    "external/imgui"
    "external/imgui-filedialog"
    "external/ImGuizmo"
    "external/assimp"
    "external/bullet3"
    "external/entt"
    "external/json"
    "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.7/include" #Your OpenCL SDK include path
)
target_link_libraries(TWE PRIVATE ${LIBFILES})

add_custom_target(ASSIMPCopy
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/bin/assimp-vc142-mtd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/bin/assimp-vc142-mtd.dll ${CMAKE_BINARY_DIR}/Release
)
add_dependencies(TWE ASSIMPCopy)