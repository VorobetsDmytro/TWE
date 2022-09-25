#version 330 core

in vec3 texCoord;

out vec4 color;

uniform samplerCube textureImg;

void main(){
    color = texture(textureImg, texCoord);
}