#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 worldNormal;

layout(push_constant) uniform Push {
	mat4 mesh;
	mat4 camera;
} push;

void main() {
	vec4 cameraPos = vec4(position, 1.0) * push.mesh * push.camera;
	cameraPos[1] = -cameraPos[1];
	gl_Position = cameraPos;
	
	worldNormal = normalize(normal * mat3(push.mesh));
	fragColor = color;
}