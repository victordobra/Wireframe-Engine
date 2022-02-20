#include "ModelRenderer.h"
#include "Matrix4x4.h"

namespace mge {
	Matrix4x4 ModelRenderer::GetTransformationMatrix() {
		return Matrix4x4::Scaling(scale) * Matrix4x4::Rotation(rotation) * Matrix4x4::Translation(position);
	}

	void ModelRenderer::Bind(VkCommandBuffer commandBuffer) {
		vkModel->Bind(commandBuffer);
	}
	void ModelRenderer::Draw(VkCommandBuffer commandBuffer) {
		vkModel->Draw(commandBuffer);
	}

	void ModelRenderer::SetModel(Model* newModel) {
		model = newModel;

		if (vkModel != nullptr)
			delete vkModel;

		vkModel = new VulkanModel(model);
	}
}