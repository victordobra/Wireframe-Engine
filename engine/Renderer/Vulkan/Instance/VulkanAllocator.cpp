#include "VulkanAllocator.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"

namespace wfe {
	// Constants
	static const VkDeviceSize ALLOC_BLOCK_SIZE = 0x4000000;
	static const size_t FREE_BLOCKS_START_SIZE = 16;

	// Internal helper functions
	VkMemoryPropertyFlags VulkanAllocator::GetMemoryTypeFlags(MemoryType memoryType) const {
		switch(memoryType) {
		case MEMORY_TYPE_GPU:
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		case MEMORY_TYPE_GPU_CPU_VISIBLE:
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		case MEMORY_TYPE_CPU_GPU_VISIBLE:
			return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		case MEMORY_TYPE_GPU_LAZY:
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		default:
			return VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
		}
	}
	VkResult VulkanAllocator::InternalAllocMemory(const VkMemoryRequirements& memRequirements, FreeList& freeList, MemoryTypeIndicesMap& memoryTypeIndices, uint32_t memoryTypeIndex, MemoryBlock& memoryBlock) {
		// Check if the requested memory is too large for the allocated blocks
		if(memRequirements.size >= ALLOC_BLOCK_SIZE) {
			// Set the memory alloc info
			VkMemoryAllocateInfo allocInfo {
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = nullptr,
				.allocationSize = memRequirements.size,
				.memoryTypeIndex = memoryTypeIndex
			};

			// Allocate the memory
			VkDeviceMemory memory;

			VkResult result = device->GetLoader()->vkAllocateMemory(device->GetDevice(), &allocInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &memory);
			if(result != VK_SUCCESS)
				return result;
			
			// Set the memory block's info and exit the function
			memoryBlock.offset = 0;
			memoryBlock.size = memRequirements.size;
			memoryBlock.memory = memory;

			return VK_SUCCESS;
		}

		// Loop through every free block in the current free list
		for(size_t freeBlockIndex = freeList.first; freeBlockIndex != SIZE_T_MAX; freeBlockIndex = freeBlocks[freeBlockIndex].next) {
			// Load the current block's info
			FreeBlock freeBlock = freeBlocks[freeBlockIndex];

			// Get the padding required for the resource's alignment
			VkDeviceSize alignmentPadding = (memRequirements.alignment - (freeBlock.block.offset & (memRequirements.alignment - 1))) & (memRequirements.alignment - 1);

			// Move on to the next free block if the current free block is not large enough
			if(alignmentPadding + memRequirements.size > freeBlock.block.size)
				continue;
			
			// Calculate the ammount of leftover padding
			VkDeviceSize leftoverPadding = freeBlock.block.size - alignmentPadding - memRequirements.size;

			if(alignmentPadding && leftoverPadding) {
				// Get a free block to write the new data in
				size_t newFreeBlockIndex;
				if(freeBlockList.first == SIZE_T_MAX) {
					// Save the free blocks list's old size
					size_t oldSize = freeBlocks.size();

					// Double the free block vector's size
					freeBlocks.resize(oldSize << 1);

					// Set the new free blocks's values
					freeBlocks[oldSize + 1].prev = SIZE_T_MAX;
					for(size_t i = oldSize + 1; i != freeBlocks.size() - 1; ++i) {
						freeBlocks[i].next = i + 1;
						freeBlocks[i + 1].prev = i;
					}
					freeBlocks.back().next = SIZE_T_MAX;

					// Modify the first new free block
					newFreeBlockIndex = oldSize;
				} else {
					// Get the first first free block in the list
					newFreeBlockIndex = freeBlockList.first;

					// Remove the block from the free list
					freeBlockList.first = freeBlocks[newFreeBlockIndex].next;
					if(freeBlocks[newFreeBlockIndex].next != SIZE_T_MAX) {
						freeBlocks[freeBlocks[newFreeBlockIndex].next].prev = SIZE_T_MAX;
					} else {
						freeBlockList.last = SIZE_T_MAX;
					}
				}

				// Set the current and new block's data
				freeBlocks[freeBlockIndex].block.size = alignmentPadding;
				freeBlocks[newFreeBlockIndex].block.offset = freeBlock.block.offset + alignmentPadding + memRequirements.size;
				freeBlocks[newFreeBlockIndex].block.size = leftoverPadding;
				freeBlocks[newFreeBlockIndex].block.memory = freeBlock.block.memory;

				// Add the new free block to the list
				freeBlocks[freeBlockIndex].next = newFreeBlockIndex;
				freeBlocks[newFreeBlockIndex].prev = freeBlockIndex;
				freeBlocks[newFreeBlockIndex].next = freeBlock.next;

				if(freeBlocks[newFreeBlockIndex].next != SIZE_T_MAX) {
					freeBlocks[freeBlocks[newFreeBlockIndex].next].prev = newFreeBlockIndex;
				} else {
					freeList.last = newFreeBlockIndex;
				}
			} else if(alignmentPadding) {
				// Change the free block's size
				freeBlocks[freeBlockIndex].block.size = alignmentPadding;
			} else if(leftoverPadding) {
				// Change the free block's offset and size
				freeBlocks[freeBlockIndex].block.offset += memRequirements.size;
				freeBlocks[freeBlockIndex].block.size = leftoverPadding;
			} else {
				// Remove the block from its list
				if(freeBlock.prev != SIZE_T_MAX) {
					freeBlocks[freeBlock.prev].next = freeBlock.next;
				} else {
					freeList.first = freeBlock.next;
				}
				if(freeBlock.next != SIZE_T_MAX) {
					freeBlocks[freeBlock.next].prev = freeBlock.prev;
				} else {
					freeList.last = freeBlock.prev;
				}

				// Add the block to the free block list
				freeBlocks[freeBlockIndex].prev = SIZE_T_MAX;
				freeBlocks[freeBlockIndex].next = freeBlockList.first;
				freeBlockList.first = freeBlockIndex;

				if(freeBlocks[freeBlockIndex].next != SIZE_T_MAX) {
					freeBlocks[freeBlocks[freeBlockIndex].next].prev = freeBlockIndex;
				} else {
					freeBlockList.last = freeBlockIndex;
				}
			}

			// Set the memory block's data and exit the function
			memoryBlock.offset = freeBlock.block.offset + alignmentPadding;
			memoryBlock.size = memRequirements.size;
			memoryBlock.memory = freeBlock.block.memory;

			return VK_SUCCESS;
		}

		// No free block is large enough for the requested allocation; allocate a new block
		VkMemoryAllocateInfo allocInfo {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = ALLOC_BLOCK_SIZE,
			.memoryTypeIndex = memoryTypeIndex
		};

		// Allocate the memory
		VkDeviceMemory memory;

		VkResult result = device->GetLoader()->vkAllocateMemory(device->GetDevice(), &allocInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &memory);
		if(result != VK_SUCCESS)
			return result;

		// Get a free block to write the new data in
		size_t newFreeBlockIndex;
		if(freeBlockList.first == SIZE_T_MAX) {
			// Save the free blocks list's old size
			size_t oldSize = freeBlocks.size();

			// Double the free block vector's size
			freeBlocks.resize(oldSize << 1);

			// Set the new free blocks's values
			freeBlocks[oldSize + 1].prev = SIZE_T_MAX;
			for(size_t i = oldSize + 1; i != freeBlocks.size() - 1; ++i) {
				freeBlocks[i].next = i + 1;
				freeBlocks[i + 1].prev = i;
			}
			freeBlocks.back().next = SIZE_T_MAX;

			// Modify the first new free block
			newFreeBlockIndex = oldSize;
		} else {
			// Get the first first free block in the list
			newFreeBlockIndex = freeBlockList.first;

			// Remove the block from the free list
			freeBlockList.first = freeBlocks[newFreeBlockIndex].next;
			if(freeBlocks[newFreeBlockIndex].next != SIZE_T_MAX) {
				freeBlocks[freeBlocks[newFreeBlockIndex].next].prev = SIZE_T_MAX;
			} else {
				freeBlockList.last = SIZE_T_MAX;
			}
		}

		// Set the new free block's info
		freeBlocks[newFreeBlockIndex].block.offset = memRequirements.size;
		freeBlocks[newFreeBlockIndex].block.size = ALLOC_BLOCK_SIZE - memRequirements.size;
		freeBlocks[newFreeBlockIndex].block.memory = memory;

		// Add the new free block to the list
		freeBlocks[newFreeBlockIndex].prev = SIZE_T_MAX;
		freeBlocks[newFreeBlockIndex].next = freeList.first;
		freeList.first = newFreeBlockIndex;

		if(freeBlocks[newFreeBlockIndex].next != SIZE_T_MAX) {
			freeBlocks[freeBlocks[newFreeBlockIndex].next].prev = newFreeBlockIndex;
		} else {
			freeList.last = newFreeBlockIndex;
		}

		// Add the memory to the memory type indices map
		memoryTypeIndices.insert({ memory, memoryTypeIndex });

		// Set the memory block's info and exit the function
		memoryBlock.offset = 0;
		memoryBlock.size = memRequirements.size;
		memoryBlock.memory = memory;

		return VK_SUCCESS;
	}
	void VulkanAllocator::InternalTrimFreeList(FreeList& freeList, MemoryTypeIndicesMap& memoryTypeIndices) {
		// Loop through the free list
		for(size_t freeBlockIndex = freeList.first; freeBlockIndex != SIZE_T_MAX;) {
			// Move on to the next block if the current memory block doesn't cover its entire corresponding area
			if(freeBlocks[freeBlockIndex].block.size != ALLOC_BLOCK_SIZE) {
				freeBlockIndex = freeBlocks[freeBlockIndex].next;
				continue;
			}
			
			// Remove the current memory block from the memory type unordered map
			memoryTypeIndices.erase(freeBlocks[freeBlockIndex].block.memory);

			// Free the current block's memory
			device->GetLoader()->vkFreeMemory(device->GetDevice(), freeBlocks[freeBlockIndex].block.memory, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);

			// Remove the block from its list
			if(freeBlocks[freeBlockIndex].prev != SIZE_T_MAX) {
				freeBlocks[freeBlocks[freeBlockIndex].prev].next = freeBlocks[freeBlockIndex].next;
			} else {
				freeList.first = freeBlocks[freeBlockIndex].next;
			}
			if(freeBlocks[freeBlockIndex].next != SIZE_T_MAX) {
				freeBlocks[freeBlocks[freeBlockIndex].next].prev = freeBlocks[freeBlockIndex].prev;
			} else {
				freeList.last = freeBlocks[freeBlockIndex].prev;
			}

			// Save the next free block's index
			size_t nextIndex = freeBlocks[freeBlockIndex].next;

			// Add the block to the free block list
			freeBlocks[freeBlockIndex].prev = SIZE_T_MAX;
			freeBlocks[freeBlockIndex].next = freeBlockList.first;
			freeBlockList.first = freeBlockIndex;

			if(freeBlocks[freeBlockIndex].next != SIZE_T_MAX) {
				freeBlocks[freeBlocks[freeBlockIndex].next].prev = freeBlockIndex;
			} else {
				freeBlockList.last = freeBlockIndex;
			}

			// Move on to the next free block in the list
			freeBlockIndex = nextIndex;
		}
	}

	// Public functions
	VulkanAllocator::VulkanAllocator(VulkanDevice* device) : device(device), freeBlocks(FREE_BLOCKS_START_SIZE) {
		// Get the physical device's memory properties
		device->GetLoader()->vkGetPhysicalDeviceMemoryProperties(device->GetPhysicalDevice(), &memoryProperties);

		// Check if the device supports the wanted extensions
		dedicatedAllocSupported = device->GetDeviceProperties().apiVersion >= VK_API_VERSION_1_1 || (device->GetEnabledExtensions().count(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME) && device->GetEnabledExtensions().count(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME));
		bind2Supported = device->GetDeviceProperties().apiVersion >= VK_API_VERSION_1_1 || device->GetEnabledExtensions().count(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);

		// Resize the free list vectors
		bufferFreeLists.resize(memoryProperties.memoryTypeCount);
		bufferFreeLists.shrink_to_fit();
		imageFreeLists.resize(memoryProperties.memoryTypeCount);
		imageFreeLists.shrink_to_fit();

		// Set the free blocks list's initial values
		freeBlockList = { 0, freeBlocks.size() - 1 };

		freeBlocks.front().prev = SIZE_T_MAX;
		for(size_t i = 0; i != freeBlocks.size() - 1; ++i) {
			freeBlocks[i].next = i + 1;
			freeBlocks[i + 1].prev = i;
		}
		freeBlocks.back().next = SIZE_T_MAX;
	}

	uint32_t VulkanAllocator::GetMemoryTypeIndex(MemoryType memoryType, uint32_t memoryTypeBits) const {
		// Get the required memory flags based on the given type
		VkMemoryPropertyFlags memoryTypeFlags = GetMemoryTypeFlags(memoryType);

		// Check if any valid mmeory type has the exact required flags
		for(uint32_t i = 0; i != memoryProperties.memoryTypeCount; ++i) {
			// Skip the current index if it is not valid
			if(!((1 << i) & memoryTypeBits))
				continue;

			// Check if the current memory type has the exact required flags
			if(memoryProperties.memoryTypes[i].propertyFlags == memoryTypeFlags)
				return i;
		}

		// Check if any valid memory type includes the required flags
		for(uint32_t i = 0; i != memoryProperties.memoryTypeCount; ++i) {
			// Skip the current index if it is not valid
			if(!((1 << i) & memoryTypeBits))
				continue;

			// Check if the current memory type includes the required flags
			if((memoryProperties.memoryTypes[i].propertyFlags & memoryTypeFlags) == memoryTypeFlags)
				return i;
		}

		// Use MEMORY_TYPE_CPU_GPU_VISIBLE if MEMORY_TYPE_GPU_CPU_VISIBLE was requested but not supported
		if(memoryType == MEMORY_TYPE_GPU_CPU_VISIBLE)
			return GetMemoryTypeIndex(MEMORY_TYPE_CPU_GPU_VISIBLE, memoryTypeBits);
		
		return UINT32_T_MAX;
	}

	VkResult VulkanAllocator::AllocBufferMemory(VkBuffer buffer, MemoryType memoryType, MemoryBlock& memoryBlock) {
		// Check if dedicated allocations are supported
		VkMemoryRequirements memRequirements;

		if(dedicatedAllocSupported) {
			// Set the memory requirements struct with the dedicated alloc requirements in the pNext chain
			VkMemoryDedicatedRequirementsKHR dedicatedMemoryRequirements {
				.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR,
				.pNext = nullptr
			};

			VkMemoryRequirements2KHR memoryRequirements {
				.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR,
				.pNext = &dedicatedMemoryRequirements
			};

			// Get the buffer's memory requirements
			VkBufferMemoryRequirementsInfo2KHR memoryRequirementsInfo {
				.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2_KHR,
				.pNext = nullptr,
				.buffer = buffer
			};

			device->GetLoader()->vkGetBufferMemoryRequirements2KHR(device->GetDevice(), &memoryRequirementsInfo, &memoryRequirements);

			// Proceed if the buffer requires or prefers a dedicated allocation
			if(dedicatedMemoryRequirements.requiresDedicatedAllocation || dedicatedMemoryRequirements.prefersDedicatedAllocation) {
				// Get the buffer's memory type index
				uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryType, memoryRequirements.memoryRequirements.memoryTypeBits);
				if(memoryTypeIndex == UINT32_T_MAX)
					return VK_ERROR_FEATURE_NOT_PRESENT;

				// Set the dedicated alloc info
				VkMemoryDedicatedAllocateInfoKHR dedicatedAllocInfo {
					.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR,
					.pNext = nullptr,
					.image = VK_NULL_HANDLE,
					.buffer = buffer
				};

				// Set the memory alloc info
				VkMemoryAllocateInfo allocInfo {
					.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					.pNext = &dedicatedAllocInfo,
					.allocationSize = memoryRequirements.memoryRequirements.size,
					.memoryTypeIndex = memoryTypeIndex
				};

				// Allocate the buffer's memory
				VkDeviceMemory memory;

				VkResult result = device->GetLoader()->vkAllocateMemory(device->GetDevice(), &allocInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &memory);
				if(result != VK_SUCCESS)
					return result;
				
				// Set the memory block's info and exit the function
				memoryBlock.offset = 0;
				memoryBlock.size = memoryRequirements.memoryRequirements.size;
				memoryBlock.memory = memory;

				return VK_SUCCESS;
			} else {
				// Copy the buffer's memory requirements to the higher scope variable
				memRequirements = memoryRequirements.memoryRequirements;
			}
		} else {
			// Get the buffer's memory requirements
			device->GetLoader()->vkGetBufferMemoryRequirements(device->GetDevice(), buffer, &memRequirements);
		}

		// Get the buffer's memory type index
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryType, memRequirements.memoryTypeBits);
		if(memoryTypeIndex == UINT32_T_MAX)
			return VK_ERROR_FEATURE_NOT_PRESENT;

		// Allocate the memory using the allocator's internal function
		return InternalAllocMemory(memRequirements, bufferFreeLists[memoryTypeIndex], bufferMemoryTypeIndices, memoryTypeIndex, memoryBlock);
	}
	VkResult VulkanAllocator::AllocImageMemory(VkImage image, MemoryType memoryType, MemoryBlock& memoryBlock) {
		// Check if dedicated allocations are supported
		VkMemoryRequirements memRequirements;

		if(dedicatedAllocSupported) {
			// Set the memory requirements struct with the dedicated alloc requirements in the pNext chain
			VkMemoryDedicatedRequirementsKHR dedicatedMemoryRequirements {
				.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR,
				.pNext = nullptr
			};

			VkMemoryRequirements2KHR memoryRequirements {
				.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR,
				.pNext = &dedicatedMemoryRequirements
			};

			// Get the image's memory requirements
			VkImageMemoryRequirementsInfo2KHR memoryRequirementsInfo {
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2_KHR,
				.pNext = nullptr,
				.image = image
			};

			device->GetLoader()->vkGetImageMemoryRequirements2KHR(device->GetDevice(), &memoryRequirementsInfo, &memoryRequirements);

			// Proceed if the image requires or prefers a dedicated allocation
			if(dedicatedMemoryRequirements.requiresDedicatedAllocation || dedicatedMemoryRequirements.prefersDedicatedAllocation) {
				// Get the image's memory type index
				uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryType, memoryRequirements.memoryRequirements.memoryTypeBits);
				if(memoryTypeIndex == UINT32_T_MAX)
					return VK_ERROR_FEATURE_NOT_PRESENT;

				// Set the dedicated alloc info
				VkMemoryDedicatedAllocateInfoKHR dedicatedAllocInfo {
					.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR,
					.pNext = nullptr,
					.image = image,
					.buffer = VK_NULL_HANDLE
				};

				// Set the memory alloc info
				VkMemoryAllocateInfo allocInfo {
					.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					.pNext = &dedicatedAllocInfo,
					.allocationSize = memoryRequirements.memoryRequirements.size,
					.memoryTypeIndex = memoryTypeIndex
				};

				// Allocate the image's memory
				VkDeviceMemory memory;

				VkResult result = device->GetLoader()->vkAllocateMemory(device->GetDevice(), &allocInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &memory);
				if(result != VK_SUCCESS)
					return result;
				
				// Set the memory block's info and exit the function
				memoryBlock.offset = 0;
				memoryBlock.size = memoryRequirements.memoryRequirements.size;
				memoryBlock.memory = memory;

				return VK_SUCCESS;
			} else {
				// Copy the buffer's memory requirements to the higher scope variable
				memRequirements = memoryRequirements.memoryRequirements;
			}
		} else {
			// Get the image's memory requirements
			device->GetLoader()->vkGetImageMemoryRequirements(device->GetDevice(), image, &memRequirements);
		}

		// Get the buffer's memory type index
		uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryType, memRequirements.memoryTypeBits);
		if(memoryTypeIndex == UINT32_T_MAX)
			return VK_ERROR_FEATURE_NOT_PRESENT;

		// Allocate the memory using the allocator's internal function
		return InternalAllocMemory(memRequirements, imageFreeLists[memoryTypeIndex], imageMemoryTypeIndices, memoryTypeIndex, memoryBlock);
	}
	void VulkanAllocator::FreeMemory(const MemoryBlock& memoryBlock) {
		// Get the memory block's memory type index
		FreeList* freeList;

		auto memoryTypeIndexIter = bufferMemoryTypeIndices.find(memoryBlock.memory);
		if(memoryTypeIndexIter != bufferMemoryTypeIndices.end()) {
			// Set the free list pointer to the corresponding buffer free list
			freeList = bufferFreeLists.data() + memoryTypeIndexIter->second;
		} else {
			memoryTypeIndexIter = imageMemoryTypeIndices.find(memoryBlock.memory);
			if(memoryTypeIndexIter != imageMemoryTypeIndices.end()) {
				// Set the free list pointer to the corresponding image free list
				freeList = imageFreeLists.data() + memoryTypeIndexIter->second;
			} else {
				// The memory must have been allocated separately; free it separately and exit the function
				device->GetLoader()->vkFreeMemory(device->GetDevice(), memoryBlock.memory, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);

				return;
			}
		}

		// Loop through the free list and find the first free block after the given memory block
		size_t prevIndex = SIZE_T_MAX;
		size_t nextIndex = freeList->first;
		for(; nextIndex != SIZE_T_MAX; nextIndex = freeBlocks[nextIndex].next) {
			// Exit the loop if the previous memory was passed without a free block after the allocation
			if(prevIndex != SIZE_T_MAX && freeBlocks[prevIndex].block.memory == memoryBlock.memory && freeBlocks[nextIndex].block.memory != memoryBlock.memory)
				break;
			
			// Exit the loop if the current free block is after the given memory block
			if(freeBlocks[nextIndex].block.memory == memoryBlock.memory && freeBlocks[nextIndex].block.offset > memoryBlock.offset)
				break;
			
			// Set the new previous index
			prevIndex = nextIndex;
		}

		// Check if the previous and next free blocks are adjacent to the given memory block
		bool8_t prevAdjacent = prevIndex != SIZE_T_MAX && freeBlocks[prevIndex].block.memory == memoryBlock.memory && (freeBlocks[prevIndex].block.offset + freeBlocks[prevIndex].block.size) == memoryBlock.offset;
		bool8_t nextAdjacent = nextIndex != SIZE_T_MAX && freeBlocks[nextIndex].block.memory == memoryBlock.memory && (memoryBlock.offset + memoryBlock.size) == freeBlocks[nextIndex].block.offset;

		if(prevAdjacent && nextAdjacent) {
			// Remove the previous block from its list
			if(freeBlocks[prevIndex].prev != SIZE_T_MAX) {
				freeBlocks[freeBlocks[prevIndex].prev].next = freeBlocks[prevIndex].next;
			} else {
				freeList->first = freeBlocks[prevIndex].next;
			}
			freeBlocks[nextIndex].prev = freeBlocks[prevIndex].prev;

			// Add the block to the free block list
			freeBlocks[prevIndex].prev = SIZE_T_MAX;
			freeBlocks[prevIndex].next = freeBlockList.first;
			freeBlockList.first = prevIndex;

			if(freeBlocks[prevIndex].next != SIZE_T_MAX) {
				freeBlocks[freeBlocks[prevIndex].next].prev = prevIndex;
			} else {
				freeBlockList.last = prevIndex;
			}

			// Set the next block's new offset and size
			freeBlocks[nextIndex].block.offset -= memoryBlock.size + freeBlocks[prevIndex].block.size;
			freeBlocks[nextIndex].block.size += memoryBlock.size + freeBlocks[prevIndex].block.size;
		} else if(prevAdjacent) {
			// Set the previous block's new size
			freeBlocks[prevIndex].block.size += memoryBlock.size;
		} else if(nextAdjacent) {
			// Set the next block's new offset and size
			freeBlocks[nextIndex].block.offset -= memoryBlock.size;
			freeBlocks[nextIndex].block.size += memoryBlock.size;
		} else {
			// Get a free block to write the new data in
			size_t newFreeBlockIndex;
			if(freeBlockList.first == SIZE_T_MAX) {
				// Save the free blocks list's old size
				size_t oldSize = freeBlocks.size();

				// Double the free block vector's size
				freeBlocks.resize(oldSize << 1);

				// Set the new free blocks's values
				freeBlocks[oldSize + 1].prev = SIZE_T_MAX;
				for(size_t i = oldSize + 1; i != freeBlocks.size() - 1; ++i) {
					freeBlocks[i].next = i + 1;
					freeBlocks[i + 1].prev = i;
				}
				freeBlocks.back().next = SIZE_T_MAX;

				// Modify the first new free block
				newFreeBlockIndex = oldSize;
			} else {
				// Get the first first free block in the list
				newFreeBlockIndex = freeBlockList.first;

				// Remove the block from the free list
				freeBlockList.first = freeBlocks[newFreeBlockIndex].next;
				if(freeBlocks[newFreeBlockIndex].next != SIZE_T_MAX) {
					freeBlocks[freeBlocks[newFreeBlockIndex].next].prev = SIZE_T_MAX;
				} else {
					freeBlockList.last = SIZE_T_MAX;
				}
			}

			// Set the new free block's data
			freeBlocks[newFreeBlockIndex].block = memoryBlock;

			// Add the new free block to the list
			freeBlocks[newFreeBlockIndex].prev = prevIndex;
			freeBlocks[newFreeBlockIndex].next = nextIndex;

			if(prevIndex != SIZE_T_MAX) {
				freeBlocks[prevIndex].next = newFreeBlockIndex;
			} else {
				freeList->first = newFreeBlockIndex;
			}
			if(nextIndex != SIZE_T_MAX) {
				freeBlocks[nextIndex].prev = newFreeBlockIndex;
			} else {
				freeList->last = newFreeBlockIndex;
			}
		}
	}

	VkResult VulkanAllocator::BindBufferMemories(size_t bufferCount, VkBuffer* buffers, const MemoryBlock* memoryBlocks) const {
		// Check if bind2 is supported
		if(bind2Supported) {
			// Allocate the buffer memory bind infos array
			PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);
			VkBindBufferMemoryInfoKHR* bindInfos = (VkBindBufferMemoryInfoKHR*)wfe::AllocMemory(bufferCount * sizeof(VkBindBufferMemoryInfoKHR));
			PopMemoryUsageType();
			if(!bindInfos)
				return VK_ERROR_OUT_OF_HOST_MEMORY;
			
			// Set the buffer memory bind infos
			for(size_t i = 0; i != bufferCount; ++i) {
				bindInfos[i].sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;
				bindInfos[i].pNext = nullptr;
				bindInfos[i].buffer = buffers[i];
				bindInfos[i].memory = memoryBlocks[i].memory;
				bindInfos[i].memoryOffset = memoryBlocks[i].offset;
			}

			// Bind the buffer memories
			VkResult result = device->GetLoader()->vkBindBufferMemory2KHR(device->GetDevice(), (uint32_t)bufferCount, bindInfos);

			// Free the buffer memory bind infos array
			wfe::FreeMemory(bindInfos);

			return result;
		} else {
			// Loop through every buffer and bind its corresponding memory block
			for(size_t i = 0; i != bufferCount; ++i) {
				// Try to bind the current buffer's memory
				VkResult result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), buffers[i], memoryBlocks[i].memory, memoryBlocks[i].offset);
				if(result != VK_SUCCESS)
					return result;
			}

			return VK_SUCCESS;
		}
	}
	VkResult VulkanAllocator::BindImageMemories(size_t imageCount, VkImage* images, const MemoryBlock* memoryBlocks) const {
		// Check if bind2 is supported
		if(bind2Supported) {
			// Allocate the image memory bind infos array
			PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);
			VkBindImageMemoryInfoKHR* bindInfos = (VkBindImageMemoryInfoKHR*)wfe::AllocMemory(imageCount * sizeof(VkBindImageMemoryInfoKHR));
			PopMemoryUsageType();
			if(!bindInfos)
				return VK_ERROR_OUT_OF_HOST_MEMORY;
			
			// Set the image memory bind infos
			for(size_t i = 0; i != imageCount; ++i) {
				bindInfos[i].sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
				bindInfos[i].pNext = nullptr;
				bindInfos[i].image = images[i];
				bindInfos[i].memory = memoryBlocks[i].memory;
				bindInfos[i].memoryOffset = memoryBlocks[i].offset;
			}

			// Bind the image memories
			VkResult result = device->GetLoader()->vkBindImageMemory2KHR(device->GetDevice(), (uint32_t)imageCount, bindInfos);

			// Free the image memory bind infos array
			wfe::FreeMemory(bindInfos);

			return result;
		} else {
			// Loop through every image and bind its corresponding memory block
			for(size_t i = 0; i != imageCount; ++i) {
				// Try to bind the current image's memory
				VkResult result = device->GetLoader()->vkBindImageMemory(device->GetDevice(), images[i], memoryBlocks[i].memory, memoryBlocks[i].offset);
				if(result != VK_SUCCESS)
					return result;
			}

			return VK_SUCCESS;
		}
	}

	void VulkanAllocator::Trim() {
		// Trim every buffer free list
		for(size_t i = 0; i != bufferFreeLists.size(); ++i)
			InternalTrimFreeList(bufferFreeLists[i], bufferMemoryTypeIndices);
		
		// Trim every image free list
		for(size_t i = 0; i != imageFreeLists.size(); ++i)
			InternalTrimFreeList(imageFreeLists[i], imageMemoryTypeIndices);
	}

	VulkanAllocator::~VulkanAllocator() {
		// Free every block of used memory
		for(auto& memoryPair : bufferMemoryTypeIndices)
			device->GetLoader()->vkFreeMemory(device->GetDevice(), memoryPair.first, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);
		for(auto& memoryPair : imageMemoryTypeIndices)
			device->GetLoader()->vkFreeMemory(device->GetDevice(), memoryPair.first, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);
	}
}