#include "RenderMesh.hpp"
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
	RenderMesh::RenderMesh(VulkanRenderer* renderer, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) : renderer(renderer), vertexCount(vertices.size()), indexCount(indices.size()) {
		// Set the queue family indices
		VulkanDevice* device = renderer->GetDevice();

		uint32_t familyIndices[] = { device->GetDeviceQueues().graphicsIndex, device->GetDeviceQueues().transferIndex };
		uint32_t familyCount = 1 + (uint32_t)(familyIndices[0] != familyIndices[1]);
		VkSharingMode sharingMode = (familyCount == 1) ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
		
		// Set the vertex buffer create info
		VkBufferCreateInfo vertexBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)sizeof(Vertex) * vertices.size(),
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharingMode = sharingMode,
			.queueFamilyIndexCount = familyCount,
			.pQueueFamilyIndices = familyIndices
		};

		// Create the vertex buffer
		VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &vertexBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &vertexBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan render mesh vertex buffer! Error code: " + string_VkResult(result));
		
		// Allocate and bind the vertex buffer's memory
		try {
			vertexBufferMemory = device->GetAllocator()->AllocBufferMemory(vertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate Vulkan mesh vertex buffer memory!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), vertexBuffer, vertexBufferMemory.memory, vertexBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan mesh vertex buffer to its memory! Error code: " + string_VkResult(result));

		// Set the index buffer create info
		VkBufferCreateInfo indexBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)sizeof(uint32_t) * indices.size(),
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			.sharingMode = sharingMode,
			.queueFamilyIndexCount = familyCount,
			.pQueueFamilyIndices = familyIndices
		};

		// Create the index buffer
		result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &indexBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &indexBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan render mesh index buffer! Error code: " + string_VkResult(result));

		// Allocate and bind the index buffer's memory
		try {
			indexBufferMemory = device->GetAllocator()->AllocBufferMemory(indexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate Vulkan mesh index buffer memory!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), indexBuffer, indexBufferMemory.memory, indexBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan mesh index buffer to its memory! Error code: " + string_VkResult(result));

		// Set the vertex staging buffer create info
		uint32_t transferFamily = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo vertexStagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)sizeof(Vertex) * vertices.size(),
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferFamily
		};

		// Create the vertex staging buffer
		VkBuffer vertexStagingBuffer;
		result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &vertexStagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &vertexStagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan render mesh vertex staging buffer! Error code: " + string_VkResult(result));
		
		// Allocate and bind the vertex staging buffer's memory
		VulkanAllocator::Memory vertexStagingBufferMemory;
		try {
			vertexStagingBufferMemory = device->GetAllocator()->AllocBufferMemory(vertexStagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate Vulkan mesh vertex staging buffer memory!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), vertexStagingBuffer, vertexStagingBufferMemory.memory, vertexStagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan mesh vertex staging buffer to its memory! Error code: " + string_VkResult(result));

		// Set the vertex staging buffer's info
		memcpy(device->GetAllocator()->GetMappedMemory(vertexStagingBufferMemory), vertices.data(), sizeof(Vertex) * vertices.size());
		
		// Set the index staging buffer create info
		VkBufferCreateInfo indexStagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)sizeof(uint32_t) * indices.size(),
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferFamily
		};

		// Create the index staging buffer
		VkBuffer indexStagingBuffer;
		result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &indexStagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &indexStagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan render mesh index staging buffer! Error code: " + string_VkResult(result));

		// Allocate and bind the index staging buffer's memory
		VulkanAllocator::Memory indexStagingBufferMemory;
		try {
			indexStagingBufferMemory = device->GetAllocator()->AllocBufferMemory(indexStagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate Vulkan mesh index staging buffer memory!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), indexStagingBuffer, indexStagingBufferMemory.memory, indexStagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan mesh index staging buffer to its memory! Error code: " + string_VkResult(result));

		// Set the index staging buffer's info
		memcpy(device->GetAllocator()->GetMappedMemory(indexStagingBufferMemory), indices.data(), sizeof(uint32_t) * indices.size());

		// Set the fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the copy fence
		VkFence copyFence;
		result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = renderer->GetTransferCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan command buffer for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan command buffer for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Write the vertex and index data to the main buffers
		VkBufferCopy vertexBufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = (VkDeviceSize)sizeof(Vertex) * vertices.size()
		};
		device->GetLoader()->vkCmdCopyBuffer(commandBuffer, vertexStagingBuffer, vertexBuffer, 1, &vertexBufferCopy);

		VkBufferCopy indexBufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = (VkDeviceSize)sizeof(uint32_t) * indices.size()
		};
		device->GetLoader()->vkCmdCopyBuffer(commandBuffer, indexStagingBuffer, indexBuffer, 1, &indexBufferCopy);
		
		// End the command buffer recording
		device->GetLoader()->vkEndCommandBuffer(commandBuffer);

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &copyFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for mesh vertex and copy! Error code: " + string_VkResult(result));
		
		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), copyFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), renderer->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);

		// Destroy the staging buffers and free their memories
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), vertexStagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), indexStagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(vertexStagingBufferMemory);
		device->GetAllocator()->FreeMemory(indexStagingBufferMemory);
	}

	void RenderMesh::GetMeshData(Vertex* vertices, uint32_t* indices) const {
		// Set the vertex staging buffer create info
		VulkanDevice* device = renderer->GetDevice();
		uint32_t transferFamily = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo vertexStagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)sizeof(Vertex) * vertexCount,
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferFamily
		};

		// Create the vertex staging buffer
		VkBuffer vertexStagingBuffer;
		VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &vertexStagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &vertexStagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan render mesh vertex staging buffer! Error code: " + string_VkResult(result));
		
		// Allocate and bind the vertex staging buffer's memory
		VulkanAllocator::Memory vertexStagingBufferMemory;
		try {
			vertexStagingBufferMemory = device->GetAllocator()->AllocBufferMemory(vertexStagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate Vulkan mesh vertex staging buffer memory!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), vertexStagingBuffer, vertexStagingBufferMemory.memory, vertexStagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan mesh vertex staging buffer to its memory! Error code: " + string_VkResult(result));
		
		// Set the index staging buffer create info
		VkBufferCreateInfo indexStagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)sizeof(uint32_t) * indexCount,
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferFamily
		};

		// Create the index staging buffer
		VkBuffer indexStagingBuffer;
		result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &indexStagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &indexStagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan render mesh index staging buffer! Error code: " + string_VkResult(result));

		// Allocate and bind the index staging buffer's memory
		VulkanAllocator::Memory indexStagingBufferMemory;
		try {
			indexStagingBufferMemory = device->GetAllocator()->AllocBufferMemory(indexStagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate Vulkan mesh index staging buffer memory!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), indexStagingBuffer, indexStagingBufferMemory.memory, indexStagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan mesh index staging buffer to its memory! Error code: " + string_VkResult(result));

		// Set the fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the copy fence
		VkFence copyFence;
		result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = renderer->GetTransferCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan command buffer for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan command buffer for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Write the vertex and index data to the main buffers
		VkBufferCopy vertexBufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = (VkDeviceSize)sizeof(Vertex) * vertexCount
		};
		device->GetLoader()->vkCmdCopyBuffer(commandBuffer, vertexBuffer, vertexStagingBuffer, 1, &vertexBufferCopy);

		VkBufferCopy indexBufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = (VkDeviceSize)sizeof(uint32_t) * indexCount
		};
		device->GetLoader()->vkCmdCopyBuffer(commandBuffer, indexBuffer, indexStagingBuffer, 1, &indexBufferCopy);
		
		// End the command buffer recording
		device->GetLoader()->vkEndCommandBuffer(commandBuffer);

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for mesh vertex and index copy! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &copyFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for mesh vertex and copy! Error code: " + string_VkResult(result));

		// Get the vertices and indices from the staging buffers
		memcpy(vertices, device->GetAllocator()->GetMappedMemory(vertexStagingBufferMemory), sizeof(Vertex) * vertexCount);
		memcpy(indices, device->GetAllocator()->GetMappedMemory(indexStagingBufferMemory), sizeof(uint32_t) * indexCount);
		
		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), copyFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), renderer->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);

		// Destroy the staging buffers and free their memories
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), vertexStagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), indexStagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(vertexStagingBufferMemory);
		device->GetAllocator()->FreeMemory(indexStagingBufferMemory);

	}

	RenderMesh::~RenderMesh() {
		// Destroy the vertex and index buffers and free their memories
		renderer->GetLoader()->vkDestroyBuffer(renderer->GetDevice()->GetDevice(), vertexBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroyBuffer(renderer->GetDevice()->GetDevice(), indexBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetDevice()->GetAllocator()->FreeMemory(vertexBufferMemory);
		renderer->GetDevice()->GetAllocator()->FreeMemory(indexBufferMemory);
	}
}