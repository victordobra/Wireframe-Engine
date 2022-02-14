#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 worldNormal;
layout(location = 2) out vec3 worldPosition;
layout(location = 3) out vec4 ambientLightColor;
layout(location = 4) out vec3 lightPosition;
layout(location = 5) out vec4 lightColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 view;
	mat4 projection;

	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
} ubo;

layout(push_constant) uniform Push {
	mat4 meshRot;
	mat4 mesh;
} push;

void main() {
	vec4 cameraPos = vec4(position, 1.0) * push.mesh * ubo.view * ubo.projection;
	cameraPos[1] = -cameraPos[1];
	gl_Position = cameraPos;

	fragColor = color;
	worldNormal = normalize(vec3(vec4(normal, 0.0) * push.meshRot));
	worldPosition = vec3(vec4(position, 0.0) * push.mesh);
	ambientLightColor = ubo.ambientLightColor;
	lightPosition = ubo.lightPosition;
	lightColor = ubo.lightColor;
}