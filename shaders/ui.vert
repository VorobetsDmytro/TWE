#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 textureCoord;

out vec2 texCoord;

uniform mat4 model = mat4(1.0f);

void main() {
   gl_Position = model * vec4(pos, 1.0);
   texCoord = textureCoord;
}