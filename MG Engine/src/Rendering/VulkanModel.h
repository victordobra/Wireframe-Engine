#pragma once
#include "VulkanDevice.h"
#include "Vector2.h"
#include "Vector3.h"
#include <vector>

namespace mge {
	class VulkanModel {
	public:
		struct Vertex {
			Vector3 position;
			Vector3 color;

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		VulkanModel(const std::vector<Vertex>& vertices);
		VulkanModel(const VulkanModel&) = delete;
		VulkanModel(VulkanModel&&) noexcept = delete;

		VulkanModel& operator=(const VulkanModel&) = delete;
		VulkanModel& operator=(VulkanModel&&) noexcept = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		~VulkanModel();
	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}