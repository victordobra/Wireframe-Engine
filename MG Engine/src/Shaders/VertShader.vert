#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
	mat4 mesh;
	mat4 proj;
} push;

void main() {
	vec4 pos4 = vec4(position, 1.0);
	vec4 cameraPos = pos4 * push.mesh * push.proj;
	gl_Position = cameraPos;
	fragColor = color;
}