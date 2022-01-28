#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 worldNormal;

layout(location = 0) out vec4 outColor;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(-1.0, -3.0, 1.0));
const float AMBIENT = 0.02;

void main() {
	float lightIntensity = AMBIENT + max(-dot(worldNormal, DIRECTION_TO_LIGHT), 0.0);

	outColor = vec4(lightIntensity * fragColor, 0.0);
}