#pragma once

#include "Core/Types/Defines.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <set>
#include <unordered_map>
#include <vector>

namespace wfe {
	class VulkanDevice;

	/// @brief A class that manages Vulkan memory allocation for buffers and images.
	class VulkanAllocator {
	public:
		/// @brief A struct representing a block of allocated memory.
		struct Memory {
			/// @brief The Vulkan device memory handle associated with the allocated memory.
			VkDeviceMemory memory = VK_NULL_HANDLE;
			/// @brief The offset of the allocated memory block in the biffer Vulkan device memory.
			VkDeviceSize offset = 0;
			/// @brief The size of the allocated memory block in bytes.
			VkDeviceSize size = 0;
		};

		/// @brief Created a Vulkan device memory allocator.
		/// @param device The Vulkan device whose memory to manage.
		VulkanAllocator(VulkanDevice* device);
		VulkanAllocator(const VulkanAllocator&) = delete;
		VulkanAllocator(VulkanAllocator&&) = delete;

		VulkanAllocator& operator=(const VulkanAllocator&) = delete;
		VulkanAllocator& operator=(VulkanAllocator&&) = delete;

		/// @brief Gets the Vulkan device that this allocator manages memory for.
		/// @return The Vulkan device that this allocator manages memory for.
		VulkanDevice* GetDevice() const {
			return device;
		}

		/// @brief Allocates memory for a Vulkan buffer.
		/// @param buffer The Vulkan buffer to allocate memory for.
		/// @param memoryProperties The required memory properties for the buffer's memory.
		/// @return A struct containing the allocated memory handle, offset and size.
		Memory AllocBufferMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties);
		/// @brief Allocates memory for a Vulkan image.
		/// @param buffer The Vulkan image to allocate memory for.
		/// @param tiling The tiling mode of the image.
		/// @param memoryProperties The required memory properties for the image's memory.
		/// @return A struct containing the allocated memory handle, offset and size.
		Memory AllocImageMemory(VkImage image, VkImageTiling tiling, VkMemoryPropertyFlags memoryProperties);
		/// @brief Frees the memory allocated for a Vulkan buffer or image.
		/// @param memory The memory to free.
		void FreeMemory(Memory memory);

		/// @brief Gets the mapped data region of the given memory block.
		/// @param memory The memory block whosed mapped data to get.
		/// @return A pointer to the memory's mapped data, or nullptr if the given memory is not host visible.
		void* GetMappedMemory(Memory memory) const;

		/// @brief Destroys the Vulkan device memory allocator and frees all allocated memory.
		~VulkanAllocator();
	private:
		struct Allocation {
			VkDeviceSize offset;
			VkDeviceSize size;

			bool operator<(const Allocation& other) const {
				return offset < other.offset;
			}
		};
		struct MemoryType {
			VkMemoryPropertyFlags properties;
			std::unordered_map<VkDeviceMemory, std::set<Allocation>> allocations;
		};

		bool AllocateDeviceMemory(uint32_t memoryTypeIndex, VkDeviceMemory& memory);
		bool AllocateDedicatedMemory(VkDeviceSize size, uint32_t memoryTypeIndex, VkBuffer buffer, VkImage image, Memory& memory);
		bool AllocateFreeBlock(VkDeviceSize size, VkDeviceSize alignment, VkDeviceSize freeStart, VkDeviceSize freeEnd, VkDeviceSize& offset);
		bool AllocateMemory(VkDeviceSize size, VkDeviceSize alignment, uint32_t memoryTypeIndex, bool requireDedicated, bool recommendDedicated, VkBuffer buffer, VkImage image, Memory& memory);

		VulkanDevice* device;

		std::unordered_map<VkDeviceMemory, uint32_t> memoryTypeIndices;
		std::unordered_map<VkDeviceMemory, void*> memoryDatas;
		std::vector<MemoryType> memoryTypes;

		mutable atomic_uint32_t allocMutex = 0;
	};
}