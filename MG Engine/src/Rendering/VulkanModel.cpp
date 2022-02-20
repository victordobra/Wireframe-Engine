#include "VulkanModel.h"
#include "Model.h"

#include <unordered_map>

namespace mge {
	VulkanModel::VulkanModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
		CreateVertexBuffer(vertices);
		CreateIndexBuffer(indices);
	}

	VulkanModel::VulkanModel(const Model* model) {
		std::vector<VulkanModel::Vertex> vertices(model->vertexC);
		std::vector<uint32_t> indices(model->indexC);

		for (size_t i = 0; i < model->vertexC; i++)
			vertices[i] = model->vertices[i];

		for (size_t i = 0; i < model->indexC; i++)
			indices[i] = (uint32_t)model->indices[i];

		CreateVertexBuffer(vertices);
		CreateIndexBuffer(indices);
	}

	void VulkanModel::CreateVertexBuffer(const std::vector<Vertex>& vertices) {
		vertexCount = (uint32_t)vertices.size();
		VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;
		uint32_t vertexSize = sizeof(Vertex);

		Buffer stagingBuffer{ vertexSize, vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());

		vertexBuffer = new Buffer(vertexSize, vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		CopyBuffer(stagingBuffer.GetBuffer(), vertexBuffer->GetBuffer(), bufferSize);
	}
	void VulkanModel::CreateIndexBuffer(const std::vector<uint32_t>& indices) {
		indexCount = (uint32_t)indices.size();
		VkDeviceSize bufferSize = sizeof(uint32_t) * indexCount;
		uint32_t indexSize = sizeof(uint32_t);

		Buffer stagingBuffer{ indexSize, indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices.data());

		indexBuffer = new Buffer(indexSize, indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		CopyBuffer(stagingBuffer.GetBuffer(), indexBuffer->GetBuffer(), bufferSize);
	}

	void VulkanModel::Draw(VkCommandBuffer commandBuffer) {
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}
	void VulkanModel::Bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	VulkanModel::~VulkanModel() {
		delete vertexBuffer;
		delete indexBuffer;
	}

	std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::GetBindingDescriptions(){
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions; 
	}
	std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::GetAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
		attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
		attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
		attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

		return attributeDescriptions;
	}
}