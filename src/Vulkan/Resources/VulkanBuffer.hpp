#pragma once

#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanDevice.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class implementing a Vulkan buffer.
	class VulkanBuffer {
	public:
		/// @brief Creates a new Vulkan buffer.
		/// @param device The Vulkan device to own the buffer.
		/// @param size The size of the buffer, in bytes.
		/// @param usage The usage flags of the buffer.
		/// @param sharingMode The sharing mode of the buffer resource.
		/// @param queueTypes A mask containing all queue types that can access the buffer. Ignored if sharingMode is VK_SHARING_MODE_EXCLUSIVE.
		/// @param memoryProperties The memory property flags for the buffer's memory.
		VulkanBuffer(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VulkanDevice::QueueTypeMask queueTypes, VkMemoryPropertyFlags memoryProperties);
		VulkanBuffer(const VulkanBuffer&) = delete;
		VulkanBuffer(VulkanBuffer&&) noexcept = delete;

		VulkanBuffer& operator=(const VulkanBuffer&) = delete;
		VulkanBuffer& operator=(VulkanBuffer&&) = delete;

		/// @brief Gets the Vulkan device that owns this buffer.
		/// @return The Vulkan device that owns this buffer.
		VulkanDevice* GetDevice() const {
			return device;
		}

		/// @brief Gets the Vulkan buffer's handle.
		/// @return The Vulkan buffer's handle.
		VkBuffer GetBuffer() const {
			return buffer;
		}
		/// @brief Gets the Vulkan buffer's corresponding memory region.
		/// @return A struct describing the Vulkan buffer's corresponding memory region.
		const VulkanAllocator::Memory& GetBufferMemory() const {
			return bufferMemory;
		}
		/// @brief Gets the Vulkan buffer's size.
		/// @return The Vulkan buffer's size.
		VkDeviceSize GetBufferSize() const {
			return size;
		}

		/// @brief Destroys the Vulkan buffer.
		~VulkanBuffer();
	private:
		VulkanDevice* device;

		VkBuffer buffer;
		VulkanAllocator::Memory bufferMemory;
		VkDeviceSize size;
	};
}