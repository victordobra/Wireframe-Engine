#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief Creates the program's Vulkan instance.
	/// @param enableDebugging Whether or not to enable debugging.
	/// @return True if Vulkan is supported, otherwise false.
	bool8_t CreateVulkanInstance(bool8_t enableDebugging);
	/// @brief Deletes the program's Vulkan instance.
	void DeleteVulkanInstance();

	/// @brief Gets the current Vulkan instance.
	/// @return The handle of the Vulkan instance.
	VkInstance GetVulkanInstance();
	/// @brief Gets the current Vulkan API version.
	/// @return An unsigned integer, representing the current Vulkan API version.
	uint32_t GetVulkanAPIVersion();
	/// @brief Gets the list of all enabled instance extensions.
	/// @return A reference to a vector containing all enabled instance extensions.
	const set<const char_t*>& GetVulkanInstanceExtensions();
	/// @brief Gets the list of all enabled instance validation layers.
	/// @return A reference to a vector containing all enabled instance validation layers.
	const set<const char_t*>& GetVulkanInstanceLayers();
}