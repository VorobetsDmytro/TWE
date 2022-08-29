#version 330 core

in vec3 normal;

out vec4 color;

struct LightType {
    bool spot;
    bool point;
    bool dir;
};

uniform LightType type;
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main(){
    if(type.spot) {
        if(normal.z < 0.9f)
            color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        else
            color = vec4(lightColor, 1.0f);
    } else if(type.point) 
        color = vec4(lightColor, 1.0f);
    else
        color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}