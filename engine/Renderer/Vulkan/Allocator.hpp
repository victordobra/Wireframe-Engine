#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief Gets the Vulkan allocation callbacks.
	/// @return A pointer to the Vulkan allocation callbacks struct.
	const VkAllocationCallbacks* GetVulkanAllocCallbacks();
}