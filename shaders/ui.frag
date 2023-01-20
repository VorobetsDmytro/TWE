#version 330 core

struct Material {
    vec3 objColor;
};

layout (location = 0) out vec4 outColor;
layout (location = 1) out int outId;

uniform bool hasTexture = false;
uniform sampler2D textureImg;
uniform float alpha = 1.f;
uniform Material material;
uniform int id = -1;

in vec2 texCoord;

void main(){
    if(hasTexture)
        outColor = texture(textureImg, texCoord) * vec4(material.objColor, alpha);
    else
        outColor = vec4(material.objColor, alpha);
    outId = id;
}