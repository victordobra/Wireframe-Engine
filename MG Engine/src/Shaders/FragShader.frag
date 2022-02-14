#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 worldNormal;
layout(location = 2) in vec3 worldPosition;
layout(location = 3) in vec4 ambientLightColor;
layout(location = 4) in vec3 lightPosition;
layout(location = 5) in vec4 lightColor;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 directionToLight = lightPosition - worldPosition;
	float attenuation = 1.0 / dot(directionToLight, directionToLight);

	vec3 lightColor3 = lightColor.xyz * lightColor.w * attenuation;
	vec3 ambientLight3 = ambientLightColor.xyz * ambientLightColor.w;
	vec3 diffuseLight = lightColor3 * max(dot(worldNormal, directionToLight), 0);

	outColor = vec4((diffuseLight + ambientLight3) * fragColor, 0.0);
}