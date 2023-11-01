#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A struct representing a range from a Vulkan device memory block.
	struct VulkanMemoryRange {
		/// @brief The handle of the memory block the current range is in.
		VkDeviceMemory memory;
		/// @brief The offset of the current range from the start of the memory block.
		VkDeviceSize offset;
		/// @brief The size of the memory range.
		VkDeviceSize size;
	};

	/// @brief Gets the Vulkan allocation callbacks.
	/// @return A pointer to the Vulkan allocation callbacks struct.
	const VkAllocationCallbacks* GetVulkanAllocCallbacks();

	/// @brief Allocates Vulkan device memory with the given info.
	/// @param allocInfo The struct containing the Vulkan allocate info. The memory type index will be set by the function.
	/// @param memoryTypeBits The bitmask of avaliable memroy types.
	/// @param memoryProperties The required properties for the requested memory.
	/// @param memory A reference to the variable in which the handle of the allocated memory will be written.
	/// @return VK_SUCCESS if the memory was allocated successfully, otherwise a corresponding error code.
	VkResult AllocVulkanDeviceMemory(VkMemoryAllocateInfo& allocInfo, uint32_t memoryTypeBits, VkMemoryPropertyFlags memoryProperties, VkDeviceMemory& memory);
}