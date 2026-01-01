#include "VulkanBuffer.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
	VulkanBuffer::VulkanBuffer(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VulkanDevice::QueueTypeMask queueTypes, VkMemoryPropertyFlags memoryProperties) : device(device), size(size) {
		// Get the queue family indices
		uint32_t indices[4], indexCount = 0;
		if(sharingMode == VK_SHARING_MODE_CONCURRENT)
			device->GetQueueFamilyIndices(queueTypes, indices, indexCount);

		// Set the buffer create info
		VkBufferCreateInfo bufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = size,
			.usage = usage,
			.sharingMode = sharingMode,
			.queueFamilyIndexCount = indexCount,
			.pQueueFamilyIndices = indices
		};

		// Create the buffer
		VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &bufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &buffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan buffer! Error code: " + string_VkResult(result));
		
		// Allocate and bind the buffer's memory
		try {
			bufferMemory = device->GetAllocator()->AllocBufferMemory(buffer, memoryProperties);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan buffer!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), buffer, bufferMemory.memory, bufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan buffer to its memory! Error code: " + string_VkResult(result));
	}

	VulkanBuffer::~VulkanBuffer() {
		// Destroy the buffer and free its memory
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), buffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(bufferMemory);
	}
}
