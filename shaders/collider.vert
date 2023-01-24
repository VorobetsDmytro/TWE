#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 vp = mat4(1.0f);

out vec3 inColor;

void main() {
   gl_Position = vp * vec4(pos, 1.0f);
   inColor = color;
}