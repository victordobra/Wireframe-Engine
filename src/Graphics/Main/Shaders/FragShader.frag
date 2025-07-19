#version 440

// Constants
const uint MAX_LIGHT_COUNT = 64;

// Vertex shader input
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

// Material info
layout(set = 1, binding = 0) uniform MaterialInfo {
	vec4 surfaceColor;
};
layout(set = 1, binding = 1) uniform sampler2D surfaceTex;

// Shader output
layout(location = 0) out vec4 outColor;

void main() {
	// Store the light value
	vec3 lightValue = vec3(0.0);

	// Calculate the ambient light contribution
	for(uint i = 0; i != ambientLightCount; ++i) {
		// Simply add the ambient light color
		lightValue += ambientLights[i].color.rgb * ambientLights[i].color.a;
	}

	// Calculate the sun light contribution
	for(uint i = 0; i != sunLightCount; ++i) {
		// Calculate the dot product between the light direction and the normal
		float normalDot = max(dot(norm, -sunLights[i].direction.xyz), 0.0);

		// Add the sun light contribution
		lightValue += sunLights[i].color.rgb * sunLights[i].color.a * normalDot;
	}

	// Calculate the point light contribution
	for(uint i = 0; i != pointLightCount; ++i) {
		// Calculate the distance from the point light to the fragment
		vec3 distanceVec = pointLights[i].position.xyz - pos;
		float distanceSqr = dot(distanceVec, distanceVec);
		distanceVec *= inversesqrt(distanceSqr);

		// Calculate the dot product between the light direction and the normal
		float normalDot = max(dot(norm, distanceVec), 0.0);

		// Add the sun light contribution
		lightValue += pointLights[i].color.rgb * pointLights[i].color.a * normalDot / distanceSqr;
	}

	// Set the output color
	outColor = texture(surfaceTex, uv) * surfaceColor * vec4(lightValue, 1.0);
}