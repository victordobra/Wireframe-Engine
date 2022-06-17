#version 450

// Inputs from the vertex shader
layout(location = 0) in vec3 worldPosition;
layout(location = 1) in vec3 worldNormal;
layout(location = 2) in vec2 uvCoord;

// Output
layout(location = 0) out vec4 outColor;

// Lighting structs
struct DirectionalLight {
    vec4 direction;
    vec4 color;
};
struct PointLight {
    vec4 position;
    vec4 color;
};
const uint MAX_LIGHT_COUNT = 100;

// Lighting buffer
layout(set = 0, binding = 0) uniform LightingUbo {
    mat4 cameraMatrix;
    vec4 ambientLightColor;

    DirectionalLight directionalLights[MAX_LIGHT_COUNT];
    PointLight       pointLights      [MAX_LIGHT_COUNT];

    uint directionalLightCount;
    uint pointLightCount;
} lightingUbo;

// Material buffer
layout(set = 1, binding = 0) uniform MaterialUbo {
    vec4 color;
} material;

// Material images
layout(set = 1, binding = 1) uniform sampler2D diffuse;

void main() {
    // Set the initial diffuse color to the ambient lighting
    vec3 diffuseColor = lightingUbo.ambientLightColor.xyz * lightingUbo.ambientLightColor.w;

    // Add to the diffuse color for every directional light
    for(uint i = 0; i < lightingUbo.directionalLightCount; ++i) {
        DirectionalLight directionalLight = lightingUbo.directionalLights[i];
        
        vec3 lightColor = directionalLight.color.xyz * directionalLight.color.w;
        float cosAngIncidence = max(-dot(worldNormal, vec3(directionalLight.direction)), 0.0);

        diffuseColor += lightColor * cosAngIncidence;
    }

    // Add to the diffuse color for every point light
    for(uint i = 0; i < lightingUbo.pointLightCount; ++i) {
        PointLight pointLight = lightingUbo.pointLights[i];

        vec3 directionToLight = normalize(vec3(pointLight.position) - worldPosition);
        vec3 lightColor = pointLight.color.xyz * pointLight.color.w;
        float cosAndIncidence = max(-dot(worldNormal, directionToLight), 0.0);
        float attenuation = 1.0 / dot(directionToLight, directionToLight);

        diffuseColor += lightColor * cosAndIncidence * attenuation;
    }

    // Set the output color
    outColor = material.color * vec4(diffuseColor, 1.0) * texture(diffuse, uvCoord);
} 