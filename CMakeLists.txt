cmake_minimum_required(VERSION 3.20)
project(TWE)

file(GLOB_RECURSE CPPFILES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE LIBFILES "lib/*.lib")

add_executable(TWE ${CPPFILES})
target_compile_features(TWE PRIVATE cxx_std_17)
target_include_directories(TWE PRIVATE
    "include"
    "include/twe"
    "include/GLFW"
    "include/glad"
    "include/stb"
    "include/glm"
    "include/imgui"
)
target_link_libraries(TWE PRIVATE ${LIBFILES})