#include "VulkanModel.h"

namespace mge {
	VulkanModel::VulkanModel(const std::vector<Vertex>& vertices) {
		CreateVertexBuffers(vertices);
	}

	void VulkanModel::CreateVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = (uint32_t)vertices.size();
		VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

		void* data;
		vkMapMemory(device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(device(), vertexBufferMemory);
	}
	void VulkanModel::Draw(VkCommandBuffer commandBuffer) {
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
	void VulkanModel::Bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	VulkanModel::~VulkanModel() {
		vkDestroyBuffer(device(), vertexBuffer, nullptr);
		vkFreeMemory(device(), vertexBufferMemory, nullptr);
	}

	std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::GetBindingDescriptions(){
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions; 
	}
	std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::GetAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
}