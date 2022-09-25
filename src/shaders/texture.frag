#version 330 core

#define MAX_LIGHTS 60

struct Material {
    vec3 objColor;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

struct Fading {
    float constant;
    float linear;
    float quadratic;
};

struct LightType {
    bool spot;
    bool point;
    bool dir;
};

struct Light {
    vec3 direction;
    vec3 pos;
    vec3 color;
    float cutOff;
    float outerCutOff;
    Fading fading;
    LightType type;
    sampler2D shadowMap;
    mat4 lightSpaceMat;
};

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform sampler2D textureImg;
uniform float alpha = 1.0f;
uniform vec3 viewPos = vec3(0.0f, 0.0f, 0.0f);
uniform Material material;

vec3 calculateDiffuse(Light light, vec3 lightDir, vec3 normalNormlz) {
    float dotProduct = dot(normalNormlz, lightDir);
    float diffuse = max(dotProduct, 0.0);
    return (diffuse * material.diffuse) * light.color;
}

vec3 calculateSpecular(Light light, vec3 lightDir, vec3 normalNormlz, vec3 viewDir) {
    vec3 reflectDir = reflect(-lightDir, normalNormlz);
    float dotProduct = dot(viewDir, reflectDir);
    float spec = pow(max(dotProduct, 0.0), material.shininess);
    return material.specular * spec * light.color;
}

vec3 calculateAmbient() {
    return vec3(material.diffuse, material.diffuse, material.diffuse) * material.ambient;
}

float calculateFading(Light light) {
    float distan = length(light.pos - fragPos);
    return 1.0f / (light.fading.constant + light.fading.linear * distan + light.fading.quadratic * (distan * distan));
}

float calculateShadow(Light light, vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(light.shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(light.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth  ? 0.8 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

vec3 calculateDirLight(Light light, vec3 viewDir, vec3 normalNormlz) {
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = calculateDiffuse(light, lightDir, normalNormlz);
    vec3 specular = calculateSpecular(light, lightDir, normalNormlz, viewDir);
    vec3 ambient = calculateAmbient();
    float shadow = calculateShadow(light, light.lightSpaceMat * vec4(fragPos, 1.0));
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 calculatePointLight(Light light, vec3 viewDir, vec3 normalNormlz) {
    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 diffuse = calculateDiffuse(light, lightDir, normalNormlz);
    vec3 specular = calculateSpecular(light, lightDir, normalNormlz, viewDir);
    vec3 ambient = calculateAmbient();
    float fading = calculateFading(light);
    ambient *= fading;
    specular *= fading;
    diffuse *= fading;
    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(Light light, vec3 viewDir, vec3 normalNormlz) {
    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 diffuse = calculateDiffuse(light, lightDir, normalNormlz);
    vec3 specular = calculateSpecular(light, lightDir, normalNormlz, viewDir);
    vec3 ambient = calculateAmbient();
    float fading = calculateFading(light);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    ambient *= fading;
    specular *= fading * intensity;
    diffuse *= fading * intensity;
    return ambient + diffuse + specular;
}

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 normalNormlz = normalize(normal);
    vec3 lightColor = vec3(0.f, 0.f, 0.f);
    for(int i = 0; i < lightCount; ++i) {
        if(lights[i].type.spot)
            lightColor += calculateSpotLight(lights[i], viewDir, normalNormlz);
        else if(lights[i].type.point)
            lightColor += calculatePointLight(lights[i], viewDir, normalNormlz);
        else
            lightColor += calculateDirLight(lights[i], viewDir, normalNormlz);
    }
    color = texture(textureImg, texCoord) * vec4(lightColor * material.objColor, alpha);
}