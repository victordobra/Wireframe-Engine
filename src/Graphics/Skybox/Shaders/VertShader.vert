#version 440

// Constants
const vec2 SKYBOX_VERTICES[] = {
	vec2(-1.0,  1.0),
	vec2( 1.0,  1.0),
	vec2(-1.0, -1.0),
	vec2( 1.0, -1.0)
};
const uint SKYBOX_INDICES[] = {
	0, 1, 3,
	0, 3, 2
};

// Push constants
layout(push_constant) uniform PushConstants {
	vec4 skyboxPoints[4];
};

// Shader output
layout(location = 0) out vec3 forward;

void main() {
	// Set the forward vector for the current vertex
	uint index = SKYBOX_INDICES[gl_VertexIndex];
	forward = skyboxPoints[index].xyz;

	// Set the vertex position on the screen, set to the far plane's distance
	gl_Position = vec4(SKYBOX_VERTICES[index], 1.0, 1.0);
}