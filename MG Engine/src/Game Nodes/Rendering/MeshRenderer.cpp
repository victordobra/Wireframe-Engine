#include "MeshRenderer.h"
#include "Matrix4x4.h"

namespace mge {
	Matrix4x4 MeshRenderer::GetTransformationMatrix() {
		return Matrix4x4::Scaling(scale) * Matrix4x4::Rotation(rotation) * Matrix4x4::Translation(position);
	}

	void MeshRenderer::Bind(VkCommandBuffer commandBuffer) {
		vkModel->Bind(commandBuffer);
	}
	void MeshRenderer::Draw(VkCommandBuffer commandBuffer) {
		vkModel->Draw(commandBuffer);
	}

	void MeshRenderer::SetMesh(Mesh* newMesh) {
		mesh = newMesh;

		if (vkModel != nullptr)
			delete vkModel;

		vkModel = new VulkanModel(mesh);
	}
}