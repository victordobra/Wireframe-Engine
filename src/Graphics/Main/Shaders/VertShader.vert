#version 440

// Constants
const uint MAX_LIGHT_COUNT = 64;

// Vertex input
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

// Light structures
struct AmbientLightInfo {
	vec4 color;
};
struct SunLightInfo {
	vec4 color;
	vec4 direction;
};
struct PointLightInfo {
	vec4 color;
	vec4 position;
};

// Scene info
layout(set = 0, binding = 0) uniform SceneInfo {
	mat4x4 cameraTransform;
	vec4 cameraPos;
	vec4 cameraFwd;

	uint ambientLightCount;
	uint sunLightCount;
	uint pointLightCount;

	AmbientLightInfo ambientLights[MAX_LIGHT_COUNT];
	SunLightInfo sunLights[MAX_LIGHT_COUNT];
	PointLightInfo pointLights[MAX_LIGHT_COUNT];
};

// Push constants
layout(push_constant) uniform PushConstants {
	mat4x4 objectTransform;
	mat4x4 objectRotTransform;
};

// Shader output
layout(location = 0) out vec3 globalPos;
layout(location = 1) out vec2 globalUV;
layout(location = 2) out vec3 globalNorm;

void main() {
	// Pass on the global position, normal and UV coordinates to the fragment shader
	globalPos = (vec4(pos, 1.0) * objectTransform).xyz;
	globalUV = uv;
	globalNorm = (vec4(norm, 1.0) * objectRotTransform).xyz;

	// Calculate the vertex's camera position
	gl_Position = vec4(globalPos, 1.0) * cameraTransform;
}
