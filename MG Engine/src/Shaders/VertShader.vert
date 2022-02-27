#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 worldPosition;
layout(location = 1) out vec3 worldNormal;
layout(location = 2) out vec4 cameraPosition;
layout(location = 3) out vec2 uvCoordinates;

layout(set = 0, binding = 0) uniform CameraUbo {
	mat4 view;
	mat4 projection;
} camera;

layout(push_constant) uniform Push {
	mat4 meshRot;
	mat4 mesh;
} push;

void main() {
	//Calculate gl_Position
	cameraPosition = vec4(position, 1.0) * push.mesh * camera.view * camera.projection;
	cameraPosition[1] = -cameraPosition[1];
	gl_Position = cameraPosition;

	//Calculate all other outputs
	worldPosition = vec3(vec4(position, 1.0) * push.mesh);
	worldNormal = normalize(vec3(vec4(normal, 1.0) * push.meshRot));
	cameraPosition = vec4(cameraPosition.xyz / cameraPosition.w, cameraPosition.w);
	uvCoordinates = uv;
}