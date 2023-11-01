#include "Allocator.hpp"
#include "Device.hpp"

namespace wfe {
	// Alloc functions
	static void* VKAPI_CALL AllocCallback(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocScope) {
		// Allocate the wanted memory using malloc
		return malloc(size, MEMORY_USAGE_RENDERER_BACKEND);
	}
	static void* VKAPI_CALL ReallocCallback(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocScope) {
		// Reallocate the given memory using realloc
		return realloc(pOriginal, size, MEMORY_USAGE_RENDERER_BACKEND);
	}
	static void VKAPI_CALL FreeCallback(void* pUserData, void* pMemory) {
		// Free the given memoy using free
		return free(pMemory, MEMORY_USAGE_RENDERER_BACKEND);
	}

	// Alloc callbacks
	static const VkAllocationCallbacks ALLOC_CALLBACKS {
		nullptr,
		AllocCallback,
		ReallocCallback,
		FreeCallback,
		nullptr,
		nullptr
	};

	// Public functions
	const VkAllocationCallbacks* GetVulkanAllocCallbacks() {
		return &ALLOC_CALLBACKS;
	}

	VkResult AllocVulkanDeviceMemory(VkMemoryAllocateInfo& allocInfo, uint32_t memoryTypeBits, VkMemoryPropertyFlags memoryProperties, VkDeviceMemory& memory) {
		// Try to allocate the given memory with every supported memory type until an allocation succeeds
		allocInfo.memoryTypeIndex = FindVulkanMemoryType(0, memoryTypeBits, memoryProperties);
		while(allocInfo.memoryTypeIndex != -1) {
			// Try to allocate the current memory
			VkResult result = vkAllocateMemory(GetVulkanDevice(), &allocInfo, &ALLOC_CALLBACKS, &memory);

			// Exit the function if the function succeeded or failed with any error except VK_ERROR_OUT_OF_DEVICE_MEMORY
			if(result != VK_ERROR_OUT_OF_DEVICE_MEMORY)
				return result;
			
			// Find the next memory type index
			allocInfo.memoryTypeIndex = FindVulkanMemoryType(allocInfo.memoryTypeIndex + 1, memoryTypeBits, memoryProperties);
		}

		// The device is out of any supported memory; return VK_ERROR_OUT_OF_DEVICE_MEMORY
		return VK_ERROR_OUT_OF_DEVICE_MEMORY;
	}
}