#version 440

// Constants
const uint MAX_LIGHT_COUNT = 64;

// Vertex input
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

// Light structures
struct SunLightInfo {
	vec3 color;
	float intensity;
	vec3 direction;
};
struct PointLightInfo {
	vec3 color;
	float intensity;
	vec3 position;
	float constantScaling;
	float linearScaling;
	float quadraticScaling;
};
struct SpotLightInfo {
	vec3 color;
	float intensity;
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;
};

// Scene info
layout(set = 0, binding = 0) uniform SceneInfo {
	mat4x4 cameraTransform;
	vec3 cameraPos;
	vec3 cameraFwd;

	vec3 ambientLightColor;

	uint sunLightCount;
	uint pointLightCount;
	uint spotLightCount;

	SunLightInfo sunLights[MAX_LIGHT_COUNT];
	PointLightInfo pointLights[MAX_LIGHT_COUNT];
	SpotLightInfo spotLights[MAX_LIGHT_COUNT];
};

// Push constants
layout(push_constant) uniform PushConstants {
	mat4x4 objectTransform;
	mat4x4 normalTransform;
};

// Shader output
layout(location = 0) out vec3 globalPos;
layout(location = 1) out vec2 globalUV;
layout(location = 2) out vec3 globalNorm;

void main() {
	// Pass on the global position, normal and UV coordinates to the fragment shader
	globalPos = (objectTransform * vec4(pos, 1.0)).xyz;
	globalUV = uv;
	globalNorm = (normalTransform * vec4(norm, 1.0)).xyz;

	// Calculate the vertex's camera position
	gl_Position = cameraTransform * vec4(globalPos, 1.0);
}
