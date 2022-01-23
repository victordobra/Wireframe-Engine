#include "MeshRenderer.h"
#include "Matrix4x4.h"

namespace mge {
	void MeshRenderer::GameFrame() {
		if (vkModel != nullptr)
			delete vkModel;

		std::vector<VulkanModel::Vertex> vertices;
		for(size_t i = 0; i < mesh->faceC; i++)
			for (size_t j = 0; j < mesh->faces[i].vertexCount; j++) {
				VulkanModel::Vertex vertex;
				vertex.position = mesh->positions[mesh->faces[i].positionIndices[j]];
				vertex.color = { color.x, color.y, color.z };
				vertices.push_back(vertex);
			}

		vkModel = new VulkanModel(vertices);
	}

	Matrix4x4 MeshRenderer::GetTransformationMatrix() {
		return Matrix4x4::Scaling(scale) * Matrix4x4::Rotation(rotation) * Matrix4x4::Translation(position);
	}

	void MeshRenderer::Bind(VkCommandBuffer commandBuffer) {
		vkModel->Bind(commandBuffer);
	}
	void MeshRenderer::Draw(VkCommandBuffer commandBuffer) {
		vkModel->Draw(commandBuffer);
	}
}