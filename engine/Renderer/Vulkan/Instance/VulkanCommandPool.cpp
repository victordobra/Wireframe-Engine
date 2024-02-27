#include "VulkanCommandPool.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"

#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
	VulkanCommandPool::VulkanCommandPool(VulkanDevice* device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags commandPoolFlags) : device(device) {
		// Set the command pool create info
		VkCommandPoolCreateInfo createInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = commandPoolFlags,
			.queueFamilyIndex = queueFamilyIndex
		};

		// Create the command pools
		for(size_t i = 0; i != Renderer::MAX_FRAMES_IN_FLIGHT; ++i) {
			VkResult result = device->GetLoader()->vkCreateCommandPool(device->GetDevice(), &createInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, commandPools + 1);
			if(result != VK_SUCCESS)
				throw Exception("Failed to create Vulkan command pool! Error code: %s", string_VkResult(result));
		}
	}

	VulkanCommandPool::~VulkanCommandPool() {
		// Destroy the command pools
		for(size_t i = 0; i != Renderer::MAX_FRAMES_IN_FLIGHT; ++i)
			device->GetLoader()->vkDestroyCommandPool(device->GetDevice(), commandPools[i], &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);
	}
}