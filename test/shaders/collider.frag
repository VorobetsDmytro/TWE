#version 330 core

in vec3 normal;

layout (location = 0) out vec4 outColor;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);
uniform float alpha = 1.f;

void main(){
    outColor = vec4(color, alpha);
}