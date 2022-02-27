#pragma once
#include "Node.h"
#include "Model.h"
#include "VulkanModel.h"
#include "EngineMath.h"
#include "Material.h"

namespace mge {
	class ModelRenderer : public Node {
	public:
		Vector3 position{};
		Quaternion rotation{};
		Vector3 scale{1.f, 1.f, 1.f};

		Material* material;

		Matrix4x4 GetTransformationMatrix();

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		Model* GetModel() { return model; }
		void SetModel(Model* newModel);

		~ModelRenderer() {
			if (vkModel != nullptr)
				delete vkModel;
		}
	private:
		VulkanModel* vkModel{nullptr};

		Model* model{nullptr};
	};
}
