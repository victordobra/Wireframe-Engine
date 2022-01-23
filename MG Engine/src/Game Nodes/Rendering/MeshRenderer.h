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
		Mesh* mesh;
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		Vector3 color;

		void GameFrame() override;

		Matrix4x4 GetTransformationMatrix();

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);
	private:
		VulkanModel* vkModel{nullptr};
	};
}
