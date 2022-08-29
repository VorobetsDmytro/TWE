#ifndef TWELOGO_HPP
#define TWELOGO_HPP

#include "object.hpp"

class TWELogo: public Object {
public:
    TWELogo();
    void draw() override;
private:
    static GLfloat vertArr[];
    static GLuint indices[];
};

#endif