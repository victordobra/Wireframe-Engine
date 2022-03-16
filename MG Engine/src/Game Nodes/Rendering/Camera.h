#pragma once
#include "Node.h"
#include "EngineMath.h"

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

		Matrix4x4 GetCameraMatrix() const;
		Matrix4x4 GetInvCameraMatrix() const;

		~Camera() = default;
	private:
		float32_t aspectRatio{ 1.0f };

		template<class T>
		void GetNodesOfType(std::vector<T*>& nodeVector, Node* parent = Node::scene) const;
	};
}