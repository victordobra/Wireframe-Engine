#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec3 worldNormal;

layout(push_constant) uniform Push {
	mat4 transformMatrix;
    mat4 modelRotation;
} push;

void main() {
    worldNormal = normalize(vec3(vec4(normal, 1.0) * push.modelRotation));
    gl_Position = vec4(position, 1.0) * push.transformMatrix;
} 