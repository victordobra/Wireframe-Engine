#version 450

// Vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

// Output to fragment shader
layout(location = 0) out vec3 worldPosition;
layout(location = 1) out vec3 worldNormal;

// Lighting buffer (for camera matrix)
layout(set = 0, binding = 0) uniform LightingUbo {
    mat4 cameraMatrix;
} lightingUbo;

// Push constant
layout(push_constant) uniform Push {
	mat4 modelTransform;
    mat4 modelRotation;
} push;

void main() {
    // Set output variables to the fragment shader
    worldPosition = vec3(vec4(position, 1.0) * push.modelTransform);
    worldNormal = normalize(vec3(vec4(normal, 1.0) * push.modelRotation));

    // Calculate the screen position
    gl_Position = vec4(position, 1.0) * push.modelTransform * lightingUbo.cameraMatrix;
} 