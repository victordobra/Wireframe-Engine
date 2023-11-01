#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A struct comtaining the indices of the queue families used by the Vulkan device.
	struct VulkanQueueFamilyIndices {
		/// @brief The index of the graphics queue family.
		uint32_t graphicsQueueIndex = -1;
		/// @brief The index of the present queue family.
		uint32_t presentQueueIndex = -1;
		/// @brief The index of the transfer queue family.
		uint32_t transferQueueIndex = -1;
		/// @brief The index of the compute queue family.
		uint32_t computeQueueIndex = -1;
	};

	/// @brief Creates the Vulkan logical device.
	/// @return True if a device that supports Vulkan and meets all requirements was found, otherwise false.
	bool8_t CreateVulkanDevice();
	/// @brief Destroys the Vulkan logical device.
	void DestroyVulkanDevice();

	/// @brief Gets the current Vulkan physical device's properties.
	/// @return A const reference to the struct containing all physical device properties.
	const VkPhysicalDeviceProperties& GetVulkanPhysicalDeviceProperties();
	/// @brief Gets the current Vulkan physical device's features.
	/// @return A const reference to the struct containing all physical device features.
	const VkPhysicalDeviceFeatures& GetVulkanPhysicalDeviceFeatures();
	/// @brief Gets the current Vulkan physical device's memory properties.
	/// @return A const reference to the struct containing all physical device memory properties.
	const VkPhysicalDeviceMemoryProperties& GetVulkanPhysicalDeviceMemoryProperties();
	/// @brief Gets the current Vulkan device's used queue family indices.
	/// @return A const reference to the struct containing all used queue family indices.
	const VulkanQueueFamilyIndices& GetVulkanDeviceQueueFamilyIndices();
	/// @brief Gets the current Vulkan device's udex extensions.
	/// @return A const reference to a set containing all used extension names.
	const set<const char_t*>& GetVulkanDeviceExtensions();

	/// @brief Finds the first supported memory type with the given properties.
	/// @param startIndex The lowest possible index of the memory type.
	/// @param typeBitmask The bitmask of allowed types.
	/// @param properties The required properties of the memory type.
	/// @return The index of the first supported memory type, or -1 if none is supported.
	uint32_t FindVulkanMemoryType(uint32_t startIndex, uint32_t typeBitmask, VkMemoryPropertyFlags properties);
	/// @brief Gets the first supported format from the given candidates.
	/// @param candidates A vector containing all format candidates.
	/// @param tiling The required tiling for the format.
	/// @param features The required available formats for the given format.
	/// @return The first supported format from the given format, or VK_FORMAT_UNDEFINED if none is supported.
	VkFormat FindVulkanSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	/// @brief Gets the current Vulkan physical device.
	/// @return A handle to the current Vulkan physical device.
	VkPhysicalDevice GetVulkanPhysicalDevice();
	/// @brief Gets the current Vulkan logical device.
	/// @return A handle to the current Vulkan logical device.
	VkDevice GetVulkanDevice();
	/// @brief Gets the current Vulkan graphics queue.
	/// @return A handle to the current Vulkan graphics queue.
	VkQueue GetVulkanGraphicsQueue();
	/// @brief Gets the current Vulkan present queue.
	/// @return A handle to the current Vulkan present queue.
	VkQueue GetVulkanPresentQueue();
	/// @brief Gets the current Vulkan transfer queue.
	/// @return A handle to the current Vulkan transfer queue.
	VkQueue GetVulkanTransferQueue();
	/// @brief Gets the current Vulkan graphics queue.
	/// @return A handle to the current Vulkan compute queue.
	VkQueue GetVulkanComputeQueue();
}