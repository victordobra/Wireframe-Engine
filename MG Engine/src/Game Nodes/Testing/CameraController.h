#pragma once
#include "Node.h"
#include "Camera.h"
#include "Input.h"
#include "EngineTime.h"

namespace mge {
	class CameraController : public Node {
	public:
		void Start() override {
			//Find the camera object
			std::vector<Node*> nodes = Node::scene->GetChildren();

			for (size_t i = 0; i < nodes.size(); i++) {
				Camera* cam = dynamic_cast<Camera*>(nodes[i]);

				if (cam != nullptr) {
					//The camera was found; set its value and exit the function
					camera = cam;
					return;
				}
			}

			//The camera was not found; throw an error
			throw std::runtime_error("Camera not found!");
		}
		void Frame() override {
			//Mode the camera
			if (IsKeyDown(Key::KEY_W) || IsKeyDown(Key::UP))
				camera->Translate({ 0.f, 0.f, -speed * GetDeltaTime() });
			if (IsKeyDown(Key::KEY_S) || IsKeyDown(Key::DOWN))
				camera->Translate({ 0.f, 0.f, speed * GetDeltaTime() });
			if (IsKeyDown(Key::KEY_A) || IsKeyDown(Key::LEFT))
				camera->Translate({ -speed * GetDeltaTime(), 0.f, 0.f });
			if (IsKeyDown(Key::KEY_D) || IsKeyDown(Key::RIGHT))
				camera->Translate({ speed * GetDeltaTime(), 0.f, 0.f });
			if (IsKeyDown(Key::KEY_E))
				camera->Translate({ 0.f, speed * GetDeltaTime(), 0.f });
			if (IsKeyDown(Key::KEY_Q))
				camera->Translate({ 0.f, -speed * GetDeltaTime(), 0.f });

			xRot += GetMouseMovement().y * sensitivity;
			xRot = (xRot > 90.f) ? 90.f : ((xRot < -90.f) ? -90.f : xRot);
			yRot += GetMouseMovement().x * sensitivity;

			camera->rotation = Quaternion::EulerAngles({ xRot, yRot, 0.f });
		}
	private:
		Camera* camera{nullptr};
		float32_t xRot{0.f};
		float32_t yRot{0.f};

		const float32_t speed = 5.f;
		const float32_t sensitivity = 0.001f;
	};
}