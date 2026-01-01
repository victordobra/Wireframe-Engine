#include "VulkanCommandPool.hpp"
#include "VulkanDevice.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
	VulkanCommandPool::VulkanCommandPool(VulkanDevice* device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags) : device(device), queueFamilyIndex(queueFamilyIndex), flags(flags) { }
	
	VkCommandPool VulkanCommandPool::GetCommandPool() {
		// Get the current thread's command pool	
		std::thread::id threadId = std::this_thread::get_id();

		uint32_t locked = 0;
		while(!mapMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		auto commandPoolIter = commandPools.find(threadId);

		// Create a new command pool if it doesn't exist
		VkCommandPool commandPool;
		if(commandPoolIter == commandPools.end()) {
			// Unlock the mutex
			mapMutex = 0;

			// Set the command pool create info
			VkCommandPoolCreateInfo commandPoolInfo {
				.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = flags,
				.queueFamilyIndex = queueFamilyIndex
			};

			// Create the command pool
			VkResult result = device->GetLoader()->vkCreateCommandPool(device->GetDevice(), &commandPoolInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &commandPool);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan command pool! Error code: " + string_VkResult(result));
			
			// Add the command pool to the map
			locked = 0;
			while(!mapMutex.compare_exchange_strong(locked, 1))
				locked = 0;

			commandPools.insert({ threadId, commandPool });

			mapMutex = 0;
		} else {
			// Get the command pool
			commandPool = commandPoolIter->second;

			// Unlock the mutex
			mapMutex = 0;
		}

		return commandPool;
	}

	VulkanCommandPool::~VulkanCommandPool() {
		// Destroy all command pools
		for(std::pair<std::thread::id, VkCommandPool> pair : commandPools)
			device->GetLoader()->vkDestroyCommandPool(device->GetDevice(), pair.second, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}