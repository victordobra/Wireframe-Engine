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
		/// @brief The index of the transfer queue family. Set to -1 if one wasn't found.
		uint32_t transferQueueIndex = -1;
		/// @brief The index of the compute queue family. Set to -1 if one wasn't found.
		uint32_t computeQueueIndex = -1;
	};

	/// @brief Creates the Vulkan logical device.
	/// @return True if a device that supports Vulkan and meets all requirements was found, otherwise false.
	bool8_t CreateVulkanDevice();
	/// @brief Deletes the Vulkan logical device.
	void DeleteVulkanDevice();

	/// @brief Gets the current Vulkan physical device's properties.
	/// @return A const reference to the struct containing all physical device properties.
	const VkPhysicalDeviceProperties& GetVulkanPhysicalDeviceProperties();
	/// @brief Gets the current Vulkan physical device's features.
	/// @return A const reference to the struct containing all physical device features.
	const VkPhysicalDeviceFeatures& GetVulkanPhysicalDeviceFeatures();
	/// @brief Gets the current Vulkan device's used queue family indices.
	/// @return A const reference to the struct containing all used queue family indices.
	const VulkanQueueFamilyIndices& GetVulkanDeviceQueueFamilyIndices();
	/// @brief Gets the current Vulkan device's udex extensions.
	/// @return A const reference to a set containing all used extension names.
	const set<const char_t*>& GetVulkanDeviceExtensions();

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
	/// @return A handle to the current Vulkan transfer queue if it is supported, otherwise a null handle.
	VkQueue GetVulkanTransferQueue();
	/// @brief Gets the current Vulkan graphics queue.
	/// @return A handle to the current Vulkan compute queue if it is supported, otherwise a null handle.
	VkQueue GetVulkanComputeQueue();
}