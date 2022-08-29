#ifndef TIME_HPP
#define TIME_HPP

#include <glad.h>
#include <glfw3.h>

class Time{
public:
    Time();
    static void calculateFPS();
    static GLfloat fps;
    static GLfloat deltaTime;
    static GLfloat curTime;
    static GLfloat preTime;
};

#endif