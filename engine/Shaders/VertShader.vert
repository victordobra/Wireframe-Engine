#version 450

// Vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

// Output to fragment shader
layout(location = 0) out ShaderOut {
    vec3 worldPosition;
    vec3 worldNormal;
    vec3 worldTangent;
    vec3 worldBitangent;
    vec2 uvCoord;
} shaderOut;

// Lighting buffer (for camera matrix)
layout(set = 0, binding = 0) uniform LightingUbo {
    mat4 cameraMatrix;
    mat4 inverseView;
} lightingUbo;

// Push constant
layout(push_constant) uniform Push {
	mat4 modelTransform;
    mat4 modelRotation;
} push;

void main() {
    // Set output variables to the fragment shader
    shaderOut.worldPosition = vec3(push.modelTransform * vec4(position, 1.0));
    shaderOut.worldNormal = normalize(vec3(push.modelRotation * vec4(normal, 0.0)));
    shaderOut.worldTangent = normalize(vec3(push.modelRotation * vec4(tangent, 0.0)));
    shaderOut.worldBitangent = normalize(vec3(push.modelRotation * vec4(bitangent, 0.0)));

    shaderOut.uvCoord = uv;

    // Calculate the screen position
    vec4 screenPosition = vec4(position, 1.0) * push.modelTransform * lightingUbo.cameraMatrix;
    screenPosition.y = -screenPosition.y;
    gl_Position = screenPosition;
} 