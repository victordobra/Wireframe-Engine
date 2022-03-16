#version 450

layout(location = 0) in vec3 worldPosition;
layout(location = 1) in vec3 worldNormal;
layout(location = 2) in vec4 cameraPosition;
layout(location = 3) in vec2 uvCoordinate;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform LightingUbo {
	vec4 ambientLightColor;

	vec4 directionalLightDirections[20];
	vec4 directionalLightColors[20];

	vec4 pointLightPositions[20];
	vec4 pointLightColors[20];

	uint directionalLightCount, pointLightCount;
} lighting;

layout(set = 1, binding = 0) uniform MaterialUbo {
	vec4 color;

	uint materialIndex;
} material;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

void main() {
	//Get the ambient light
	vec3 diffuseLight = lighting.ambientLightColor.xyz * lighting.ambientLightColor.w;

	//Loop through every directional light
	for(uint i = 0; i < lighting.directionalLightCount; i++) {
		float cosAngIncidence = max(-dot(worldNormal, vec3(lighting.directionalLightDirections[i])), 0.0);
		vec3 intensity = lighting.directionalLightColors[i].xyz * lighting.directionalLightColors[i].w;

		diffuseLight += intensity * cosAngIncidence;
	}

	//Loop through every point light
	for(uint i = 0; i < lighting.pointLightCount; i++) {
		vec3 directionToLight = vec3(lighting.pointLightPositions[i]) - worldPosition;
		float attenuation = 1.0 / dot(directionToLight, directionToLight);
		float cosAngIncidence = max(-dot(worldNormal, directionToLight), 0.0);
		vec3 intensity = lighting.pointLightColors[i].xyz * lighting.pointLightColors[i].w * attenuation;

		diffuseLight += intensity * cosAngIncidence;
	}

	vec2 targetUV = uvCoordinate;
	targetUV[0] *= 0.1;
	targetUV[0] += material.materialIndex / 10.0;

	outColor = vec4(diffuseLight * vec3(material.color * texture(texSampler, targetUV)), 0.0);
}