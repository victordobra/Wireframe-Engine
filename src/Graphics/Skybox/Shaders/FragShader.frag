#version 440

// Vertex shader input
layout(location = 0) in vec3 forward;

// Skybox cubemap
layout(set = 0, binding = 0) uniform samplerCube skybox;

// Shader output
layout(location = 0) out vec4 outColor;

void main() {
	// Get the cubemap sample using the forward vector
	outColor = texture(skybox, forward);
}