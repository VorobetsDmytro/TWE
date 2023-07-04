cmake_minimum_required(VERSION 3.20)
project(TWE)

# BUILD TYPE (Debug/Release)
set(CMAKE_BUILD_TYPE Release)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

file(GLOB_RECURSE CPPFILES 
    "src/*.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB_RECURSE LIBFILES 
        "external/*/lib/Debug/*.lib" 
    )
else()
    file(GLOB_RECURSE LIBFILES 
        "external/*/lib/Release/*.lib" 
    )
endif()
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Debug" "${CMAKE_BINARY_DIR}/Release")

add_executable(TWE ${CPPFILES})
target_compile_features(TWE PRIVATE cxx_std_17)
target_include_directories(TWE PRIVATE
    "include"
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
    "external/irrKlang"
)
target_link_libraries(TWE PRIVATE ${LIBFILES})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(TWE PRIVATE TWE_DEBUG)
else()
    target_compile_definitions(TWE PRIVATE TWE_RELEASE)
endif()

add_custom_target(ASSIMPCopy
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/bin/assimp-vc142-mtd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/bin/assimp-vc142-mtd.dll ${CMAKE_BINARY_DIR}/Release
)
add_custom_target(IRRKLANGCopy
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/irrKlang/bin/winx64-visualStudio/ikpFlac.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/irrKlang/bin/winx64-visualStudio/ikpMP3.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/irrKlang/bin/winx64-visualStudio/irrKlang.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/irrKlang/bin/winx64-visualStudio/ikpFlac.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/irrKlang/bin/winx64-visualStudio/ikpMP3.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/irrKlang/bin/winx64-visualStudio/irrKlang.dll ${CMAKE_BINARY_DIR}/Release
)
add_dependencies(TWE ASSIMPCopy)
add_dependencies(TWE IRRKLANGCopy)