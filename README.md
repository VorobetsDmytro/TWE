# The Worst Engine

The game engine based on OpenGL

![TWE2022-12-30 190749](https://user-images.githubusercontent.com/82779713/210095876-72be4d73-1b03-4cac-9188-4d25c30cc4dd.png)

![123](https://user-images.githubusercontent.com/82779713/213765398-8fc738ae-bae7-452d-8442-308aa0eb5ece.png)

## Requirements
1. Visual Studio 16 2019

2. GNU Make: Recommended version - 3.81

3. CMake: Recommended version - 3.20

4. External libs `(link: https://github.com/Parasik72/TWE-external-libs)`

## Build
Firstly recommended to create a new directory`(example "mkdir build")` and move to`(example "cd build")`.

The next step is generating build files:
>`cmake .. -G "Visual Studio 16 2019"`

Afterwards you have to build the project:
>`cmake --build .`

## Settings
To working with scripts you have to set a CMake default generator as MSVC`(recommended Visual Studio 16 2019)`

## Author
[Dmytro Vorobets](https://github.com/VorobetsDmytro)
