#pragma once
#include "GameNode.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "VulkanModel.h"

namespace mge {
	class MeshRenderer : public GameNode {
	public:
		Vector3 position{};
		Quaternion rotation{};
		Vector3 scale{1.0f, 1.0f, 1.0f};

		Vector3 color{};

		Matrix4x4 GetTransformationMatrix();

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		Mesh* GetMesh() { return mesh; }
		void SetMesh(Mesh* newMesh);
	private:
		VulkanModel* vkModel{nullptr};

		Mesh* mesh{nullptr};
	};
}
