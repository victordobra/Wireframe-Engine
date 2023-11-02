#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief Creates all required Vulkan command pools.
	void CreateVulkanCommandPools();
	/// @brief Destroys all Vulkan command pools.
	void DestroyVulkanCommandPools();

	/// @brief Gets the Vulkan command pool used for graphics commands.
	/// @return A handle to the graphics command pool. May be identical to other command pools.
	VkCommandPool GetVulkanGraphicsCommandPool();
	/// @brief Gets the Vulkan command pool used for transfer commands.
	/// @return A handle to the transfer command pool. May be identical to other command pools.
	VkCommandPool GetVulkanTransferCommandPool();
	/// @brief Gets the Vulkan command pool used for compute commands.
	/// @return A handle to the compute command pool. May be identical to other command pools.
	VkCommandPool GetVulkanComputeCommandPool();
}