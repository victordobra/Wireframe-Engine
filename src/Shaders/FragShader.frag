#version 450

// Inputs from the vertex shader
layout(location = 0) in vec3 worldNormal;

// Output
layout(location = 0) out vec4 outColor;

// Material buffer
layout(set = 1, binding = 0) uniform MaterialUbo {
    vec4 color;
} material;

// Lighting constants - PLACEHOLDERS
const vec4 AMBIENT_COLOR = vec4(1.0, 1.0, 1.0, 0.1);
const vec3 LIGHT_DIRECTION = normalize(vec3(1.0, 1.0, -1.0));

void main() {
    vec3 diffuseLight = AMBIENT_COLOR.xyz * AMBIENT_COLOR.w;
    
    float cosAngIncidence = max(-dot(worldNormal, LIGHT_DIRECTION), 0.0);
    diffuseLight += cosAngIncidence * vec3(1.0, 1.0, 1.0);

    outColor = material.color * vec4(diffuseLight, 1.0);
} 