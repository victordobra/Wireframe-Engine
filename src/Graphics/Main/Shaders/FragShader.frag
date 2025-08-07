#version 440

// Constants
const uint MAX_LIGHT_COUNT = 64;

// Vertex shader input
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

// Material info
layout(set = 1, binding = 0) uniform MaterialInfo {
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float specularExp;
};
layout(set = 1, binding = 1) uniform sampler2D ambientTex;
layout(set = 1, binding = 2) uniform sampler2D diffuseTex;
layout(set = 1, binding = 3) uniform sampler2D specularTex;
layout(set = 1, binding = 4) uniform sampler2D specularExpMap;

// Shader output
layout(location = 0) out vec4 outColor;

void main() {
	// Store the diffuse and specular light values
	vec3 diffuseLightValue = vec3(0.0);
	vec3 specularLightValue = vec3(0.0);

	// Get the specular exponent and view vector for the current fragment
	float localSpecExp = specularExp * texture(specularExpMap, uv).r;
	vec3 viewVec = normalize(pos - cameraPos);

	// Calculate the sun light contribution
	for(uint i = 0; i != sunLightCount; ++i) {
		// Calculate the diffuse light value
		float normalDot = max(-dot(norm, sunLights[i].direction.xyz), 0.0);

		diffuseLightValue += sunLights[i].color * sunLights[i].intensity * normalDot;

		// Get the reflection vector and calculate the specular light value
		vec3 reflVec = reflect(sunLights[i].direction, norm);
		float reflDot = max(-dot(viewVec, reflVec), 0.0);

		specularLightValue += sunLights[i].color * sunLights[i].intensity * pow(reflDot, localSpecExp);
	}

	// Calculate the point light contribution
	for(uint i = 0; i != pointLightCount; ++i) {
		// Calculate the distance from the point light to the fragment
		vec3 distVec = pointLights[i].position - pos;
		float distSqr = dot(distVec, distVec);
		float dist = sqrt(distSqr);
		distVec /= dist;

		// Calculate the light's intensity
		float lightIntensity = pointLights[i].intensity / (pointLights[i].constantScaling + pointLights[i].linearScaling * dist + pointLights[i].quadraticScaling * distSqr);

		// Calculate the diffuse light value
		float normalDot = max(dot(norm, distVec), 0.0);

		diffuseLightValue += pointLights[i].color * lightIntensity * normalDot;

		// Get the reflection vector and calculate the specular light value
		vec3 reflVec = reflect(-distVec, norm);
		float reflDot = max(-dot(viewVec, reflVec), 0.0);

		specularLightValue += pointLights[i].color * lightIntensity * pow(reflDot, localSpecExp);
	}

	// Calculate the spot light contribution
	for(uint i = 0; i != spotLightCount; ++i) {
		// Calculate the direction vector from the spot light to the current point
		vec3 pointDir = normalize(pos - spotLights[i].position);
		float pointDot = dot(spotLights[i].direction, pointDir);

		// Calculate the light's intensity
		float lightIntensity = spotLights[i].intensity * clamp((pointDot - spotLights[i].outerCutoff) / (spotLights[i].innerCutoff - spotLights[i].outerCutoff), 0.0, 1.0);

		// Calculate the diffuse light value
		float normalDot = max(-dot(norm, pointDir), 0.0);

		diffuseLightValue += spotLights[i].color * lightIntensity * normalDot;

		// Get the reflection vector and calculate the specular light value
		vec3 reflVec = reflect(pointDir, norm);
		float reflDot = max(-dot(viewVec, reflVec), 0.0);

		specularLightValue += spotLights[i].color * lightIntensity * pow(reflDot, localSpecExp);
	}

	// Calculate the color values
	vec3 ambientValue = texture(ambientTex, uv).rgb * ambientColor * ambientLightColor;
	vec3 diffuseValue = texture(diffuseTex, uv).rgb * diffuseColor * diffuseLightValue;
	vec3 specularValue = texture(specularTex, uv).rgb * specularColor * specularLightValue;

	// Set the output color
	outColor = vec4(ambientValue + diffuseValue + specularValue, 1.0);
}