#include "VulkanAllocator.hpp"
#include "VulkanDevice.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	static const VkDeviceSize DEVICE_MEMORY_SIZE = 0x4000000; // 64 MiB
	static const VkDeviceSize DEVICE_MAX_ALLOC_SIZE = 0x1000000; // 16 MiB

	// Internal helper functions
	bool VulkanAllocator::AllocateDeviceMemory(uint32_t memoryTypeIndex, VkDeviceMemory& memory) {
		// Set the alloc info
		VkMemoryAllocateInfo allocInfo {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = DEVICE_MEMORY_SIZE,
			.memoryTypeIndex = memoryTypeIndex
		};

		// Allocate the device memory
		VkResult result = device->GetLoader()->vkAllocateMemory(device->GetDevice(), &allocInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &memory);
		if(result == VK_SUCCESS) {
			// Lock the alloc mutex
			uint32_t locked = 0;
			while(!allocMutex.compare_exchange_strong(locked, 1))
				locked = 0;

			// Check if the current memory is host visible
			if(device->GetDeviceMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
				// Map the current memory
				void* data;
				result = device->GetLoader()->vkMapMemory(device->GetDevice(), memory, 0, VK_WHOLE_SIZE, 0, &data);
				if(result != VK_SUCCESS)
					throw std::runtime_error((std::string)"Failed to map Vulkan device memory! Error code: " + string_VkResult(result));
				
				// Add the mapped memory to the map
				memoryDatas.insert({ memory, data });
			}

			// Add the new memory to the memory types
			memoryTypeIndices.insert({ memory, memoryTypeIndex });
			memoryTypes[memoryTypeIndex].allocations.insert({ memory, {} });

			// Unlock the alloc mutex
			allocMutex = 0;

			return true;
		}
		if(result != VK_ERROR_OUT_OF_DEVICE_MEMORY)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan device memory! Error code: " + string_VkResult(result));
		
		return false;
	}
	bool VulkanAllocator::AllocateDedicatedMemory(VkDeviceSize size, uint32_t memoryTypeIndex, VkBuffer buffer, VkImage image, Memory& memory) {
		// Set the alloc info
		VkMemoryDedicatedAllocateInfo dedicatedAllocInfo {
			.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO,
			.pNext = nullptr,
			.image = image,
			.buffer = buffer
		};
		VkMemoryAllocateInfo allocInfo {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = &dedicatedAllocInfo,
			.allocationSize = size & ~1,
			.memoryTypeIndex = memoryTypeIndex
		};

		// Allocate the device memory
		VkResult result = device->GetLoader()->vkAllocateMemory(device->GetDevice(), &allocInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &memory.memory);
		if(result != VK_SUCCESS && result != VK_ERROR_OUT_OF_DEVICE_MEMORY)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan dedicated device memory! Error code: " + string_VkResult(result));
		
		memory.offset = 0;
		memory.size = size & ~1;

		// Exit if the allocation was unsuccessful
		if(result != VK_SUCCESS)
			return false;

		// Check if the current memory is host visible
		if(device->GetDeviceMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
			// Map the current memory
			void* data;
			result = device->GetLoader()->vkMapMemory(device->GetDevice(), memory.memory, 0, VK_WHOLE_SIZE, 0, &data);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to map Vulkan device memory! Error code: " + string_VkResult(result));
			
			// Add the mapped memory to the map
			uint32_t locked = 0;
			while(!allocMutex.compare_exchange_strong(locked, 1))
				locked = 0;
			memoryDatas.insert({ memory.memory, data });
			allocMutex = 0;
		}
		
		return true;
	}
	bool VulkanAllocator::AllocateFreeBlock(VkDeviceSize size, VkDeviceSize alignment, VkDeviceSize freeStart, VkDeviceSize freeEnd, VkDeviceSize& offset) {
		// Align the starts and ends of the free block accordingly
		VkDeviceSize granularity = device->GetDeviceProperties().limits.bufferImageGranularity;

		if((size & 1) != (freeStart & 1) && alignment < granularity)
			alignment = granularity;
		
		freeStart &= ~1;
		freeStart = (freeStart + alignment - 1) & ~(alignment - 1);
		
		if((size & 1) != (freeEnd & 1)) {
			freeEnd = freeEnd & ~(granularity - 1);
		} else {
			freeEnd &= ~1;
		}
		
		// Remove the image flag, if it exists
		size &= ~1;

		// Check if the free block is large enough
		if(freeStart + size <= freeEnd) {
			offset = freeStart;
			return true;
		}
		return false;
	}
	bool VulkanAllocator::AllocateMemory(VkDeviceSize size, VkDeviceSize alignment, uint32_t memoryTypeIndex, bool requireDedicated, bool recommendDedicated, VkBuffer buffer, VkImage image, Memory& memory) {
		// Check if a dedicated allocation is recommended
		if(requireDedicated || recommendDedicated) {
			// Allocate the dedicated memory
			if(AllocateDedicatedMemory(size, memoryTypeIndex, buffer, image, memory)) {
				return true;
			} else if(requireDedicated) {
				return false;
			}
		}

		// Lock the alloc mutex
		uint32_t locked = 0;
		while(!allocMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		// Loop through all available device memories
		for(std::pair<const VkDeviceMemory, std::set<Allocation>>& memoryAllocations : memoryTypes[memoryTypeIndex].allocations) {
			// Loop though all allication pairs to find all free blocks
			VkDeviceSize prevOffset = size & 1;

			for(const Allocation& allocation : memoryAllocations.second) {
				// Check if the current free block is large enough
				if(AllocateFreeBlock(size, alignment, prevOffset, allocation.offset + (allocation.size & 1), memory.offset)) {
					// Add the allocation to the set
					memoryAllocations.second.insert({ memory.offset, size });

					// Set the memory's info and exit the function
					memory.memory = memoryAllocations.first;
					memory.size = size & ~1;

					allocMutex = 0;
					return true;
				}

				// Set the new previous offset
				prevOffset = allocation.offset + allocation.size;
			}

			// Check if the last free block is large enough
			if(AllocateFreeBlock(size, alignment, prevOffset, DEVICE_MEMORY_SIZE + (size & 1), memory.offset)) {
				// Add the allocation to the set
				memoryAllocations.second.insert({ memory.offset, size });

				// Set the memory's info and exit the function
				memory.memory = memoryAllocations.first;
				memory.size = size & ~1;

				allocMutex = 0;
				return true;
			}
		};

		// Unlock the alloc mutex
		allocMutex = 0;

		// Allocate a new device memory
		VkDeviceMemory newMemory;
		if(!AllocateDeviceMemory(memoryTypeIndex, newMemory))
			return false;
		
		// Add the memory block to the new device memory
		while(!allocMutex.compare_exchange_strong(locked, 1))
			locked = 0;
		memoryTypes[memoryTypeIndex].allocations[newMemory].insert({ 0, size });
		allocMutex = 0;

		memory.memory = newMemory;
		memory.offset = 0;
		memory.size = size & ~1;
		return true;
	}

	// Public functions
	VulkanAllocator::VulkanAllocator(VulkanDevice* device) : device(device) {
		// Initialize the memory types
		memoryTypes.resize(device->GetDeviceMemoryProperties().memoryTypeCount);
		for(uint32_t i = 0; i != device->GetDeviceMemoryProperties().memoryTypeCount; ++i)
			memoryTypes[i].properties = device->GetDeviceMemoryProperties().memoryTypes[i].propertyFlags;
	}

	VulkanAllocator::Memory VulkanAllocator::AllocBufferMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties) {
		// Get the memory requirements for the buffer
		VkMemoryDedicatedRequirements dedicatedRequirements {
			.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS,
			.pNext = nullptr
		};
		VkMemoryRequirements2 memoryRequirements {
			.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2,
			.pNext = &dedicatedRequirements
		};
		VkBufferMemoryRequirementsInfo2 bufferMemoryRequirementsInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2,
			.pNext = nullptr,
			.buffer = buffer
		};

		device->GetLoader()->vkGetBufferMemoryRequirements2(device->GetDevice(), &bufferMemoryRequirementsInfo, &memoryRequirements);
		
		// Set the memory's alignment to at least 2, for buffer/image flags
		if(memoryRequirements.memoryRequirements.alignment < 2)
			memoryRequirements.memoryRequirements.alignment = 2;
		memoryRequirements.memoryRequirements.size = (memoryRequirements.memoryRequirements.size + memoryRequirements.memoryRequirements.alignment - 1) & ~(memoryRequirements.memoryRequirements.alignment - 1);

		// Require a dedicated allocation if the requested memory is too large
		dedicatedRequirements.requiresDedicatedAllocation |= (memoryRequirements.memoryRequirements.size > DEVICE_MAX_ALLOC_SIZE);

		// Loop through all memory types to find a suitable one
		Memory memory;

		for(uint32_t i = 0; i != device->GetDeviceMemoryProperties().memoryTypeCount; ++i) {
			// Check if the memory type is suitable
			if((memoryRequirements.memoryRequirements.memoryTypeBits & (1 << i)) && (device->GetDeviceMemoryProperties().memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties) {
				// Try to allocate the memory
				if(AllocateMemory(memoryRequirements.memoryRequirements.size, memoryRequirements.memoryRequirements.alignment, i, dedicatedRequirements.requiresDedicatedAllocation, dedicatedRequirements.prefersDedicatedAllocation, buffer, VK_NULL_HANDLE, memory))
					return memory;
			}
		}

		throw std::bad_alloc();
	}
	VulkanAllocator::Memory VulkanAllocator::AllocImageMemory(VkImage image, VkImageTiling tiling, VkMemoryPropertyFlags memoryProperties) {
		// Get the memory requirements for the image
		VkMemoryDedicatedRequirements dedicatedRequirements {
			.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS,
			.pNext = nullptr
		};
		VkMemoryRequirements2 memoryRequirements {
			.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2,
			.pNext = &dedicatedRequirements
		};
		VkImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2,
			.pNext = nullptr,
			.image = image
		};

		device->GetLoader()->vkGetImageMemoryRequirements2(device->GetDevice(), &imageMemoryRequirementsInfo, &memoryRequirements);
		
		// Set the memory's alignment to at least 2, for buffer/image flags
		if(memoryRequirements.memoryRequirements.alignment < 2)
			memoryRequirements.memoryRequirements.alignment = 2;
		memoryRequirements.memoryRequirements.size = (memoryRequirements.memoryRequirements.size + memoryRequirements.memoryRequirements.alignment - 1) & ~(memoryRequirements.memoryRequirements.alignment - 1);
		
		// Add the image flag to the size
		memoryRequirements.memoryRequirements.size |= 1;

		// Require a dedicated allocation if the requested memory is too large
		dedicatedRequirements.requiresDedicatedAllocation |= (memoryRequirements.memoryRequirements.size > DEVICE_MAX_ALLOC_SIZE);

		// Loop through all memory types to find a suitable one
		Memory memory;
		VkDeviceSize flaggedSize = memoryRequirements.memoryRequirements.size | (tiling == VK_IMAGE_TILING_OPTIMAL ? 1 : 0);

		for(uint32_t i = 0; i != device->GetDeviceMemoryProperties().memoryTypeCount; ++i) {
			// Check if the memory type is suitable
			if((memoryRequirements.memoryRequirements.memoryTypeBits & (1 << i)) && (device->GetDeviceMemoryProperties().memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties) {
				// Try to allocate the memory
				if(AllocateMemory(flaggedSize, memoryRequirements.memoryRequirements.alignment, i, dedicatedRequirements.requiresDedicatedAllocation, dedicatedRequirements.prefersDedicatedAllocation, VK_NULL_HANDLE, image, memory))
					return memory;
			}
		}

		throw std::bad_alloc();
	}
	void VulkanAllocator::FreeMemory(Memory memory) {
		// Lock the alloc mutex
		uint32_t locked = 0;
		while(!allocMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		// Get the memory type index
		auto memoryTypeIndexIter = memoryTypeIndices.find(memory.memory);

		// If the memory isn't in the type index map, it must be dedicated
		if(memoryTypeIndexIter == memoryTypeIndices.end()) {
			// Check if the memory is host visible
			auto dataIter = memoryDatas.find(memory.memory);
			if(dataIter != memoryDatas.end()) {
				// Unmap the memory and remove the memory data from the map
				device->GetLoader()->vkUnmapMemory(device->GetDevice(), memory.memory);
				memoryDatas.erase(dataIter);
			}

			// Unlock the alloc mutex and free the memory
			allocMutex = 0;
			device->GetLoader()->vkFreeMemory(device->GetDevice(), memory.memory, &VulkanRenderer::ALLOCATION_CALLBACKS);

			return;
		}

		// Get the memory type index
		uint32_t memoryTypeIndex = memoryTypeIndexIter->second;

		// Free the memory from the memory type
		memoryTypes[memoryTypeIndex].allocations[memory.memory].erase({ memory.offset, memory.size });
		allocMutex = 0;
	}

	void* VulkanAllocator::GetMappedMemory(Memory memory) const {
		// Check if the given memory is mapped
		uint32_t locked = 0;
		while(!allocMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		auto dataIter = memoryDatas.find(memory.memory);
		if(dataIter == memoryDatas.end()) {
			allocMutex = 0;
			return nullptr;
		}
		
		// Get the mapped memory and offset it accordingly
		uint8_t* dataStart = (uint8_t*)(dataIter->second);
		allocMutex = 0;
		return dataStart + memory.offset;
	}

	VulkanAllocator::~VulkanAllocator() {
		// Free all allocated device memory
		for(std::pair<const VkDeviceMemory, uint32_t>& deviceMemoryPair : memoryTypeIndices) {
			// Free the device memory
			device->GetLoader()->vkFreeMemory(device->GetDevice(), deviceMemoryPair.first, &VulkanRenderer::ALLOCATION_CALLBACKS);
		}
	}
}