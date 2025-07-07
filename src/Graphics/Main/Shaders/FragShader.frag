#version 440

// Vertex shader input
layout(location = 0) in vec2 uv;

// Material info
layout(set = 0, binding = 0) uniform MaterialInfo {
	vec4 surfaceColor;
};
layout(set = 0, binding = 1) uniform sampler2D surfaceTex;

// Shader output
layout(location = 0) out vec4 outColor;

void main() {
	// Set the output color
	outColor = texture(surfaceTex, uv) * surfaceColor;
}