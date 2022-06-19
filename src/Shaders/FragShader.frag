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
    vec4 cameraWorldPos;

    vec4 ambientLightColor;

    DirectionalLight directionalLights[MAX_LIGHT_COUNT];
    PointLight       pointLights      [MAX_LIGHT_COUNT];

    uint directionalLightCount;
    uint pointLightCount;
} lightingUbo;

// Material buffer
layout(set = 1, binding = 0) uniform MaterialUbo {
    vec4 diffuseColor;
    float specularExponent;
    vec4 specularColor;
} material;

// Material images
layout(set = 1, binding = 1) uniform sampler2D diffuseTexture;
layout(set = 1, binding = 2) uniform sampler2D specularTexture;
layout(set = 1, binding = 3) uniform sampler2D normalMap;

void main() {
    // Calculate the world normal based on the normal map
    vec3 zAxis = worldNormal;
    vec3 xAxis = normalize(cross(zAxis, vec3(0.0, 1.0, 0.0)));
    vec3 yAxis = cross(xAxis, zAxis);

    mat3 tbnMatrix;

    tbnMatrix[0] = xAxis;
	tbnMatrix[1] = yAxis;
	tbnMatrix[2] = zAxis;

    tbnMatrix = transpose(tbnMatrix);

    vec3 surfaceNormal = 2.0 * texture(normalMap, uvCoord).rgb - 1.0;
    surfaceNormal = normalize(surfaceNormal * tbnMatrix);

    // Set the initial diffuse light to the ambient lighting
    vec3 diffuseLight = lightingUbo.ambientLightColor.xyz * lightingUbo.ambientLightColor.w;

    // Set a initial specular light and other important variables for specular lighting
    vec3 specularLight = vec3(0.0);

    vec3 viewDirection = normalize(vec3(lightingUbo.cameraWorldPos) - worldPosition);

    // Add to the diffuse color for every directional light
    for(uint i = 0; i < lightingUbo.directionalLightCount; ++i) {
        // Diffuse lighting
        DirectionalLight directionalLight = lightingUbo.directionalLights[i];
        
        vec3 lightColor = directionalLight.color.xyz * directionalLight.color.w;
        float cosAngIncidence = max(dot(surfaceNormal, -vec3(directionalLight.direction)), 0.0);

        diffuseLight += lightColor * cosAngIncidence;

        // Specular lighting
        vec3 halfAngle = normalize(-vec3(directionalLight.direction) + viewDirection);
        float blinnTerm = dot(surfaceNormal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0, 1);
        blinnTerm = pow(blinnTerm, material.specularExponent);

        specularLight += lightColor * blinnTerm;
    }

    // Add to the diffuse color for every point light
    for(uint i = 0; i < lightingUbo.pointLightCount; ++i) {
        // Diffuse lighting
        PointLight pointLight = lightingUbo.pointLights[i];

        vec3 directionToLight = pointLight.position.xyz - worldPosition;
        vec3 lightColor = pointLight.color.xyz * pointLight.color.w;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);

        directionToLight = normalize(directionToLight);

        float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0.0);
        vec3 intensity = lightColor * attenuation;

        diffuseLight += intensity * cosAngIncidence;

        // Specular lighting
        vec3 halfAngle = normalize(directionToLight + viewDirection);
        float blinnTerm = dot(surfaceNormal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0, 1);
        blinnTerm = pow(blinnTerm, material.specularExponent);

        specularLight += intensity * blinnTerm;
    }

    // Set the output color
    outColor = material.diffuseColor * vec4(diffuseLight, 1.0) * texture(diffuseTexture, uvCoord) + material.specularColor * vec4(specularLight, 1.0) * texture(specularTexture, uvCoord);
} 