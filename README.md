# The Worst Engine

The game engine based on OpenGL

![1245](https://user-images.githubusercontent.com/82779713/187254652-d5eaea06-1f0e-4ded-a996-389a3a6132ec.png)

## Requirements
1. VS 2019 amd64

2. GNU Make: Recommended version - 3.81

3. CMake: Recommended version - 3.20

4. C++17

## Build
Firstly recommended to create a new directory`(example "mkdir build")` and move to`(example "cd build")`.

The next step is generating build files:
>`cmake .. -G "Visual Studio 16 2019"`

Afterwards you have to build the project:
>`cmake --build .`

## Settings
You can change the game environment in `"test.cpp"` file.

## Start
Execute `"*.exe"` file in the build directory.

## Control
To move the camera you have to hold the right mouse button and push `W,A,S,D` buttons.

## Author
[Dmytro Vorobets](https://github.com/Parasik72)
