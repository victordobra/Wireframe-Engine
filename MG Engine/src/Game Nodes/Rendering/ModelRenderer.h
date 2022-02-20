#pragma once
#include "Node.h"
#include "Model.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "VulkanModel.h"

namespace mge {
	class ModelRenderer : public Node {
	public:
		Vector3 position{};
		Quaternion rotation{};
		Vector3 scale{1.f, 1.f, 1.f};

		Vector3 color{};

		Matrix4x4 GetTransformationMatrix();

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		Model* GetModel() { return model; }
		void SetModel(Model* newModel);
	private:
		VulkanModel* vkModel{nullptr};

		Model* model{nullptr};
	};
}
