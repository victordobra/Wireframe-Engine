#pragma once

#include "Core/Types/Defines.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <thread>
#include <unordered_map>

namespace wfe {
	class VulkanDevice;

	/// @brief A class implementing a Vulkan command pool with automatic synchronization.
	class VulkanCommandPool {
	public:
		/// @brief Creates a new Vulkan command pool for the given device and queue family index.
		/// @param device The Vulkan device to own the command pool.
		/// @param queueFamilyIndex The queue family index to create the command pool for.
		/// @param flags The flags to create the command pool with.
		VulkanCommandPool(VulkanDevice* device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
		VulkanCommandPool(const VulkanCommandPool&) = delete;
		VulkanCommandPool(VulkanCommandPool&&) = delete;

		VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
		VulkanCommandPool& operator=(VulkanCommandPool&&) = delete;

		/// @brief Gets the Vulkan device that owns the command pool.
		/// @return The Vulkan device that owns the command pool.
		VulkanDevice* GetDevice() const {
			return device;
		}
		/// @brief Gets the queue family index of the command pool.
		/// @return The queue family index of the command pool.
		uint32_t GetQueueFamilyIndex() const {
			return queueFamilyIndex;
		}
		/// @brief Gets the current thread's Vulkan command pool handle.
		/// @return The current thread's Vulkan command pool handle.
		VkCommandPool GetCommandPool();

		/// @brief Destroys the Vulkan command pool.
		~VulkanCommandPool();
	private:
		VulkanDevice* device;
		uint32_t queueFamilyIndex;
		VkCommandPoolCreateFlags flags;

		atomic_uint32_t mapMutex = 0;
		std::unordered_map<std::thread::id, VkCommandPool> commandPools;
	};
}