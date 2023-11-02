#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <Vulkan/vulkan_core.h>

namespace wfe {
	/// @brief Creates the Vulkan surface.
	void CreateVulkanSurface();
	/// @brief Destroys the Vulkan surface.
	void DestroyVulkanSurface();
	/// @brief Generates the Vulkan surface's support details.
	void GenerateVulkanSurfaceSupportDetails();

	/// @brief Gets the window's Vulkan surface.
	/// @return The handle of the Vulkan surface.
	VkSurfaceKHR GetVulkanSurface();
	/// @brief Gets the Vulkan surface's supported formats.
	/// @return A const reference to a vector containing all supported formats.
	const vector<VkSurfaceFormatKHR>& GetVulkanSurfaceSupportedFormats();
	/// @brief Gets the Vulkan surface's supported present modes.
	/// @return A const reference to a vector containing all supported present modes.
	const vector<VkPresentModeKHR>& GetVulkanSurfaceSupportedPresentModes();
}