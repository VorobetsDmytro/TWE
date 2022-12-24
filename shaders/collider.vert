#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalVecs;

out vec3 normal;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

void main() {
   gl_Position = projection * view * model * vec4(pos, 1.0f);
   normal = normalVecs;
}