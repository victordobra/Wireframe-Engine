#version 450

layout(location = 0) in vec3 worldNormal;

layout(location = 0) out vec4 outColor;

const vec4 AMBIENT_COLOR = vec4(1.0, 1.0, 1.0, 0.1);
const vec3 LIGHT_DIRECTION = normalize(vec3(1.0, 1.0, -1.0));

void main() {
    vec3 diffuseLight = AMBIENT_COLOR.xyz * AMBIENT_COLOR.w;
    
    float cosAngIncidence = max(-dot(worldNormal, LIGHT_DIRECTION), 0.0);
    diffuseLight += cosAngIncidence * vec3(1.0, 1.0, 1.0);

    outColor = vec4(1.0, 1.0, 1.0, 1.0) * vec4(diffuseLight, 1.0);
} 