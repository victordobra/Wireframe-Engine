#pragma once
#include "Node.h"
#include "EngineMath.h"
#include "Buffer.h"
#include "Descriptors.h"
#include "SwapChain.h"

namespace mge {
	enum class ClearMode {
		NONE,
		COLOR
	};

	class Camera : public Node {
	public:
		Vector3 position{};
		Quaternion rotation{};

		Vector3 clearColor{};
		ClearMode clearMode{ ClearMode::COLOR };
		float32_t fov{ 60.0f };
		float32_t zNear{ 0.01f };
		float32_t zFar{ 1000.0f };

		void Start() override;
		void Render() override;

		void Translate(Vector3 movement) {
			position += Matrix4x4::Rotation(rotation.Inverted()) * movement;
		}
		void Rotate(Vector3 eulerRotation) {
			rotation *= Quaternion::EulerAngles(eulerRotation * DEG_TO_RAD_MULTIPLIER);
		}

		Matrix4x4 GetCameraMatrix();
		Matrix4x4 GetInvCameraMatrix();
	private:
		float32_t aspectRatio{ 1.0f };
	};
}