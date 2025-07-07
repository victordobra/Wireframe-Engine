#version 440

// Vertex input
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec2 norm;

// Push constants
layout(push_constant) uniform PushConstants {
	mat4x4 objectTransform;
	mat4x4 cameraTransform;
};

// Shader output
layout(location = 0) out vec2 outUV;

void main() {
	// Calculate the vertex's camera position
	gl_Position = vec4(pos, 1.0) * objectTransform * cameraTransform;

	// Output the UV coordinate to the fragment shader
	outUV = uv;
}
