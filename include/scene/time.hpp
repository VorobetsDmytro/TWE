#ifndef TIME_HPP
#define TIME_HPP

#include <glad.h>
#include <glfw3.h>

namespace TWE {
    class Time{
    public:
        Time();
        static void calculate();
        static GLfloat fps;
        static GLfloat deltaTime;
        static GLfloat curTime;
        static GLfloat preTime;
    };
}

#endif