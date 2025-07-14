#include "Allocator.hpp"
#include "VirtualMemory.hpp"
#include "Core/Types/BuildInfo.hpp"
#include <bit>
#include <new>
#include <stdlib.h>
#include <string.h>

namespace wfe {
	// Constants
#if defined(WFE_ARCHITECTURE_64_BIT)
	static const size_t VIRTUAL_ADDRESS_SMALL_SPACE_SIZE = 0x100000000000ull; // 16 TiB
	static const size_t VIRTUAL_ADDRESS_LARGE_SPACE_SIZE = 0x100000000000ull; // 16 TiB
#else
	static const size_t VIRTUAL_ADDRESS_SMALL_SPACE_SIZE = 0x20000000; // 512 MiB
	static const size_t VIRTUAL_ADDRESS_LARGE_SPACE_SIZE = 0x40000000; // 1 GiB
#endif

	static const size_t DEFAULT_ALIGNMENT = sizeof(size_t) << 1;
	static const size_t SMALL_HEAP_SIZE = 0x40000; // 256 KiB
	static const size_t LARGE_HEAP_SIZE_STEP = 0x1000000; // 16 MiB
	static const size_t SMALL_SIZE_COUNT = sizeof(uint64_t) << 3;
	static const size_t MAX_SMALL_SIZE = SMALL_SIZE_COUNT * DEFAULT_ALIGNMENT;
	static const size_t FREE_BLOCK_FLAG = 1;

	// Structs
	struct ListItem {
		ListItem* prev;
		ListItem* next;
	};
	struct SmallHeap {
		atomic_size_t mutex;
		SmallHeap* nextHeap;
		uint64_t freeAvail;
		ListItem freeLists[SMALL_SIZE_COUNT];
		ListItem largeFreeList;
	
#if defined(WFE_ARCHITECTURE_64_BIT)
		size_t padding;
#endif
	};

	// Global variables
	static void* smallHeapAddressStart = nullptr;
	static void* largeHeapAddressStart = nullptr;
	static size_t smallHeapSize = 0;
	static size_t largeHeapSize = 0;
	static atomic_size_t smallHeapMutex = 0;
	static atomic_size_t largeFreeListMutex = 0;

	static thread_local SmallHeap* heapList = nullptr;
	static ListItem largeFreeList;

	// Internal helper functions
	static void InsertFreeList(ListItem* list, void* block, size_t size) {
		// Write the block's size at the ends of the block
		*(size_t*)block = size | FREE_BLOCK_FLAG;
		*(size_t*)((size_t)block + size - sizeof(size_t)) = size | FREE_BLOCK_FLAG;

		// Exit the function if the block is too small
		if(size == DEFAULT_ALIGNMENT)
			return;
		
		// Insert the block into the free list
		ListItem* item = (ListItem*)((size_t)block + sizeof(size_t));
		item->prev = list;
		item->next = list->next;
		list->next = item;
		item->next->prev = item;
	}
	static void RemoveFreeList(void* block) {
		// Get the size of the block
		size_t size = *(size_t*)block ^ FREE_BLOCK_FLAG;

		// Exit the function if the block is too small
		if(size == DEFAULT_ALIGNMENT)
			return;
		
		// Remove the block from the free list
		ListItem* item = (ListItem*)((size_t)block + sizeof(size_t));
		item->prev->next = item->next;
		item->next->prev = item->prev;
	}

	// Public functions
	void* AllocMemory(size_t size, size_t alignment) {
		// Exit the function if the size is zero
		if(!size)
			return nullptr;

		// Check if the virtual address space needs to be allocated
		if(!smallHeapAddressStart || !largeHeapAddressStart) {
			// Reverve the small heap address space
			smallHeapAddressStart = VirtualMemoryReserve(nullptr, VIRTUAL_ADDRESS_SMALL_SPACE_SIZE);
			if(!smallHeapAddressStart)
				return nullptr;
			
			// Reserve the large heap address space
			largeHeapAddressStart = VirtualMemoryReserve(nullptr, VIRTUAL_ADDRESS_LARGE_SPACE_SIZE);
			if(!largeHeapAddressStart) {
				VirtualMemoryRelease(smallHeapAddressStart);
				return nullptr;
			}

			// Initialize the large free list
			if(!VirtualMemoryCommit(largeHeapAddressStart, LARGE_HEAP_SIZE_STEP)) {
				VirtualMemoryRelease(smallHeapAddressStart);
				VirtualMemoryRelease(largeHeapAddressStart);
				return nullptr;
			}

			largeHeapSize = LARGE_HEAP_SIZE_STEP;
			largeFreeList.prev = &largeFreeList;
			largeFreeList.next = &largeFreeList;

			*(size_t*)largeHeapAddressStart = 0;
			*(size_t*)((size_t)largeHeapAddressStart + LARGE_HEAP_SIZE_STEP - sizeof(size_t)) = 0;
			InsertFreeList(&largeFreeList, (void*)((size_t)largeHeapAddressStart + sizeof(size_t)), LARGE_HEAP_SIZE_STEP - (sizeof(size_t) << 1));
		}

		// Set the new alignment and required size
		if(alignment < DEFAULT_ALIGNMENT)
			alignment = DEFAULT_ALIGNMENT;
		size = (size + alignment - 1) & ~(alignment - 1);
		size_t alignedSize = size + alignment - DEFAULT_ALIGNMENT;

		// Check if the size is small or large
		if(alignedSize < MAX_SMALL_SIZE) {
			// Loop through all heaps, allocating in the first suitable one
			for(SmallHeap* heap = heapList; heap; heap = heap->nextHeap) {
				// Lock the current heap's mutex
				size_t locked = 0;
				while(!heap->mutex.compare_exchange_strong(locked, 1))
					locked = 0;
				
				// Check if the heap has a free block with all required space
				size_t sizeIndex = alignedSize / DEFAULT_ALIGNMENT;
				uint64_t freeBitmask = heap->freeAvail & ~((1ull << sizeIndex) - 1);

				if(freeBitmask) {
					// Get the position of the first free block
					size_t freeBlockIndex = std::countr_zero(freeBitmask);
					size_t freeBlockSize = DEFAULT_ALIGNMENT * (freeBlockIndex + 1);
					ListItem* freeBlock = heap->freeLists[freeBlockIndex].next;

					// Get the leftover space to the left/right of the allocated block
					size_t alignedPos = ((size_t)freeBlock + alignment - 1) & ~(alignment - 1);
					size_t leftSpace = alignedPos - (size_t)freeBlock;
					size_t rightSpace = freeBlockSize - leftSpace - size - (sizeof(size_t) << 1);

					// Remove the current block from the free list
					RemoveFreeList((size_t*)freeBlock - 1);
					if(heap->freeLists[freeBlockIndex].next == &heap->freeLists[freeBlockIndex]) {
						// If the free list is empty, clear the bitmask
						heap->freeAvail ^= 1ull << freeBlockIndex;
					}
					
					// Add the two new blocks to the free list
					if(leftSpace) {
						size_t leftSpaceIndex = (leftSpace - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
						InsertFreeList(&heap->freeLists[leftSpaceIndex], (size_t*)freeBlock - 1, leftSpace);
						heap->freeAvail |= 1ull << leftSpaceIndex;
					}
					if(rightSpace) {
						size_t rightBlock = alignedPos + size + sizeof(size_t);
						size_t rightSpaceIndex = (rightSpace - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
						InsertFreeList(&heap->freeLists[rightSpaceIndex], (size_t*)rightBlock, rightSpace);
						heap->freeAvail |= 1ull << rightSpaceIndex;
					}

					// Set the block's size
					*(size_t*)(alignedPos - sizeof(size_t)) = size + (sizeof(size_t) << 1);
					*(size_t*)(alignedPos + size) = size + (sizeof(size_t) << 1);

					// Unlock the heap's mutex and exit the function
					heap->mutex = 0;
					return (void*)alignedPos;
				}

				// Check if the heap has a large free block
				if(heap->largeFreeList.next != &heap->largeFreeList) {
					// Get the large free block and its size
					ListItem* freeBlock = heap->largeFreeList.next;
					size_t freeBlockSize = *((size_t*)freeBlock - 1) ^ FREE_BLOCK_FLAG;

					// Get the leftover space to the left/right of the allocated block
					size_t alignedPos = ((size_t)freeBlock + alignment - 1) & ~(alignment - 1);
					size_t leftSpace = alignedPos - (size_t)freeBlock;
					size_t rightSpace = freeBlockSize - leftSpace - size - (sizeof(size_t) << 1);

					// Remove the current block from the free list
					RemoveFreeList((size_t*)freeBlock - 1);

					// Add the two new blocks to the free list
					if(leftSpace) {
						size_t leftSpaceIndex = (leftSpace - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
						InsertFreeList(&heap->freeLists[leftSpaceIndex], (size_t*)freeBlock - 1, leftSpace);
						heap->freeAvail |= 1ull << leftSpaceIndex;
					}
					if(rightSpace > MAX_SMALL_SIZE) {
						size_t rightBlock = alignedPos + size + sizeof(size_t);
						InsertFreeList(&heap->largeFreeList, (size_t*)rightBlock, rightSpace);
					} else if(rightSpace) {
						size_t rightBlock = alignedPos + size + sizeof(size_t);
						size_t rightSpaceIndex = (rightSpace - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
						InsertFreeList(&heap->freeLists[rightSpaceIndex], (size_t*)rightBlock, rightSpace);
						heap->freeAvail |= 1ull << rightSpaceIndex;
					}

					// Set the block's size
					*(size_t*)(alignedPos - sizeof(size_t)) = size + (sizeof(size_t) << 1);
					*(size_t*)(alignedPos + size) = size + (sizeof(size_t) << 1);

					// Unlock the heap's mutex and exit the function
					heap->mutex = 0;
					return (void*)alignedPos;
				}
			}

			// No suitable heap was found; create a new one
			size_t locked = 0;
			while(!smallHeapMutex.compare_exchange_strong(locked, 1))
				locked = 0;

			size_t heapOffset = smallHeapSize;
			if(heapOffset == VIRTUAL_ADDRESS_SMALL_SPACE_SIZE) {
				smallHeapMutex = 0;
				return nullptr;
			}
			smallHeapSize += SMALL_HEAP_SIZE;

			SmallHeap* newHeap = (SmallHeap*)((size_t)smallHeapAddressStart + heapOffset);

			// Commit the new heap's memory
			if(!VirtualMemoryCommit(newHeap, SMALL_HEAP_SIZE)) {
				smallHeapMutex = 0;
				return nullptr;
			}

			// Set the new heap's properties
			newHeap->mutex = 0;
			newHeap->nextHeap = heapList;
			newHeap->freeAvail = 0;
			
			for(size_t i = 0; i != SMALL_SIZE_COUNT; ++i) {
				newHeap->freeLists[i].prev = &newHeap->freeLists[i];
				newHeap->freeLists[i].next = &newHeap->freeLists[i];
			}

			newHeap->largeFreeList.prev = &newHeap->largeFreeList;
			newHeap->largeFreeList.next = &newHeap->largeFreeList;
			*(size_t*)(newHeap + 1) = 0;
			*(size_t*)((size_t)newHeap + SMALL_HEAP_SIZE - sizeof(size_t)) = 0;

			// Get the total free block's size and position
			size_t freeBlock = (size_t)newHeap + sizeof(SmallHeap) + (sizeof(size_t) << 1);
			size_t freeBlockSize = SMALL_HEAP_SIZE - sizeof(SmallHeap) - (sizeof(size_t) << 1);

			// Get the leftover space to the left/right of the allocated block
			size_t alignedPos = (freeBlock + alignment - 1) & ~(alignment - 1);
			size_t leftSpace = alignedPos - freeBlock;
			size_t rightSpace = freeBlockSize - leftSpace - size - (sizeof(size_t) << 1);

			// Add the two new blocks to the free list
			if(leftSpace) {
				size_t leftSpaceIndex = (leftSpace - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
				InsertFreeList(&newHeap->freeLists[leftSpaceIndex], (size_t*)freeBlock - 1, leftSpace);
				newHeap->freeAvail |= 1ull << leftSpaceIndex;
			}
			if(rightSpace > MAX_SMALL_SIZE) {
				size_t rightBlock = alignedPos + size + sizeof(size_t);
				InsertFreeList(&newHeap->largeFreeList, (size_t*)rightBlock, rightSpace);
			} else if(rightSpace) {
				size_t rightBlock = alignedPos + size + sizeof(size_t);
				size_t rightSpaceIndex = (rightSpace - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
				InsertFreeList(&newHeap->freeLists[rightSpaceIndex], (size_t*)rightBlock, rightSpace);
				newHeap->freeAvail |= 1ull << rightSpaceIndex;
			}

			// Set the block's size
			*(size_t*)(alignedPos - sizeof(size_t)) = size + (sizeof(size_t) << 1);
			*(size_t*)(alignedPos + size) = size + (sizeof(size_t) << 1);

			// Add the new heap to the heap list and unlock the mutex
			heapList = newHeap;
			smallHeapMutex = 0;

			return (void*)alignedPos;
		} else {
			// Lock the large free list mutex
			size_t locked = 0;
			while(!largeFreeListMutex.compare_exchange_strong(locked, 1))
				locked = 0;

			// Loop through all free blocks
			for(ListItem* freeBlock = largeFreeList.next; freeBlock != &largeFreeList; freeBlock = freeBlock->next) {
				// Get the size of the free block
				size_t freeBlockSize = *((size_t*)freeBlock - 1) ^ FREE_BLOCK_FLAG;

				// Check if the free block is large enough
				if(freeBlockSize >= alignedSize) {
					// Get the leftover space to the left/right of the allocated block
					size_t alignedPos = ((size_t)freeBlock + alignment - 1) & ~(alignment - 1);
					size_t leftSpace = alignedPos - (size_t)freeBlock;
					size_t rightSpace = freeBlockSize - leftSpace - size - (sizeof(size_t) << 1);

					// Remove the current block from the free list
					RemoveFreeList((size_t*)freeBlock - 1);

					// Add the two new blocks to the free list
					if(leftSpace)
						InsertFreeList(&largeFreeList, (size_t*)freeBlock - 1, leftSpace);
					if(rightSpace) {
						size_t rightBlock = alignedPos + size + sizeof(size_t);
						InsertFreeList(&largeFreeList, (size_t*)rightBlock, rightSpace);
					}

					// Set the block's size
					*(size_t*)(alignedPos - sizeof(size_t)) = size + (sizeof(size_t) << 1);
					*(size_t*)(alignedPos + size) = size + (sizeof(size_t) << 1);

					// Unlock the mutex and exit the function
					largeFreeListMutex = 0;
					return (void*)alignedPos;
				}
			}

			// No free block is large enough; allocate a new one
			size_t lastFreeSize = *(size_t*)((size_t)largeHeapAddressStart + largeHeapSize - (sizeof(size_t) << 1));
			size_t freeBlock = (size_t)largeHeapAddressStart + largeHeapSize - lastFreeSize;

			// Get the position of the new block
			size_t alignedPos = (freeBlock + alignment - 1) & ~(alignment - 1);
			size_t leftSpace = alignedPos - freeBlock;

			// Remove the last free block from the free list
			RemoveFreeList((size_t*)freeBlock - 1);

			// Commit the required memory for the new block
			size_t endPos = alignedPos + size + (sizeof(size_t) << 1);
			size_t newSpace = endPos - ((size_t)largeHeapAddressStart + largeHeapSize);
			newSpace = (newSpace + LARGE_HEAP_SIZE_STEP - 1) & ~(LARGE_HEAP_SIZE_STEP - 1);

			if(!VirtualMemoryCommit((void*)((size_t)largeHeapAddressStart + largeHeapSize), newSpace)) {
				largeFreeListMutex = 0;
				return nullptr;
			}
			largeHeapSize += newSpace;
			*(size_t*)((size_t)largeHeapAddressStart + largeHeapSize - sizeof(size_t)) = 0;

			// Set the new free blocks' properties
			lastFreeSize += newSpace;
			size_t rightSpace = lastFreeSize - leftSpace - size - (sizeof(size_t) << 1);

			// Add the two new blocks to the free list
			if(leftSpace)
				InsertFreeList(&largeFreeList, (size_t*)freeBlock - 1, leftSpace);
			if(rightSpace) {
				size_t rightBlock = alignedPos + size + sizeof(size_t);
				InsertFreeList(&largeFreeList, (size_t*)rightBlock, rightSpace);
			}

			// Set the block's size
			*(size_t*)(alignedPos - sizeof(size_t)) = size + (sizeof(size_t) << 1);
			*(size_t*)(alignedPos + size) = size + (sizeof(size_t) << 1);

			// Unlock the mutex and exit the function
			largeFreeListMutex = 0;
			return (void*)alignedPos;
		}
	}
	void* ReallocMemory(void* memory, size_t size, size_t alignment) {
		// Check if the function should behave as AllocMemory or FreeMemory
		if(!memory)
			return AllocMemory(size, alignment);
		if(!size) {
			FreeMemory(memory);
			return nullptr;
		}

		// Allocate the new memory
		void* newMemory = AllocMemory(size, alignment);
		if(!newMemory)
			return nullptr;
		
		// Copy the old memory to the new memory
		size_t oldSize = (*((size_t*)memory - 1) - (sizeof(size_t) << 1));
		size_t minSize = ((oldSize < size) ? oldSize : size);

		memcpy(newMemory, memory, minSize);

		// Free the old memory
		FreeMemory(memory);

		return newMemory;
	}
	void FreeMemory(void* memory) {
		// Exit the function if the memory does not exist
		if(!memory)
			return;
		
		// Get the size of the memory block
		memory = (void*)((size_t)memory - sizeof(size_t));
		size_t size = *(size_t*)memory;

		// Check if the memory if a small or large block
		if((size_t)memory >= (size_t)smallHeapAddressStart && (size_t)memory < (size_t)smallHeapAddressStart + VIRTUAL_ADDRESS_SMALL_SPACE_SIZE) {
			// Get the heap that contains the memory
			size_t heapOffset = ((size_t)memory - (size_t)smallHeapAddressStart) & ~(SMALL_HEAP_SIZE - 1);
			SmallHeap* heap = (SmallHeap*)((size_t)smallHeapAddressStart + heapOffset);

			// Lock the heap's mutex
			size_t locked = 0;
			while(!heap->mutex.compare_exchange_strong(locked, 1))
				locked = 0;
			
			// Get the size of the left and right blocks
			size_t leftSize = *((size_t*)memory - 1);
			if(leftSize & FREE_BLOCK_FLAG) {
				leftSize ^= FREE_BLOCK_FLAG;
			} else {
				leftSize = 0;
			}

			size_t rightSize = *(size_t*)((size_t)memory + size);
			if(rightSize & FREE_BLOCK_FLAG) {
				rightSize ^= FREE_BLOCK_FLAG;
			} else {
				rightSize = 0;
			}

			// Free the two old blocks
			if(leftSize) {
				RemoveFreeList((void*)((size_t)memory - leftSize));

				// Check if the left block is small aand its bitmask status must be updates
				if(leftSize <= MAX_SMALL_SIZE) {
					size_t leftSizeIndex = (leftSize - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
					if(heap->freeLists[leftSizeIndex].next == &heap->freeLists[leftSizeIndex])
						heap->freeAvail ^= 1ull << leftSizeIndex;
				}
			}
			if(rightSize) {
				RemoveFreeList((void*)((size_t)memory + size));

				// Check if the right block is small aand its bitmask status must be updates
				if(rightSize <= MAX_SMALL_SIZE) {
					size_t rightSizeIndex = (rightSize - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
					if(heap->freeLists[rightSizeIndex].next == &heap->freeLists[rightSizeIndex])
						heap->freeAvail ^= 1ull << rightSizeIndex;
				}
			}

			// Calculate the new free block's size
			size_t freeBlockSize = size + leftSize + rightSize;

			// Insert the new free block
			if(freeBlockSize > MAX_SMALL_SIZE) {
				InsertFreeList(&heap->largeFreeList, (void*)((size_t)memory - leftSize), freeBlockSize);
			} else {
				size_t sizeIndex = (freeBlockSize - DEFAULT_ALIGNMENT) / DEFAULT_ALIGNMENT;
				InsertFreeList(&heap->freeLists[sizeIndex], (void*)((size_t)memory - leftSize), freeBlockSize);
				heap->freeAvail |= 1ull << sizeIndex;
			}

			// Unlock the heap's mutex
			heap->mutex = 0;
		} else {
			// Lock the large free list mutex
			size_t locked = 0;
			while(!largeFreeListMutex.compare_exchange_strong(locked, 1))
				locked = 0;
			
			// Get the size of the left and right blocks
			size_t leftSize = *((size_t*)memory - 1);
			if(leftSize & FREE_BLOCK_FLAG) {
				leftSize ^= FREE_BLOCK_FLAG;
			} else {
				leftSize = 0;
			}

			size_t rightSize = *(size_t*)((size_t)memory + size);
			if(rightSize & FREE_BLOCK_FLAG) {
				rightSize ^= FREE_BLOCK_FLAG;
			} else {
				rightSize = 0;
			}

			// Free the two old blocks
			if(leftSize)
				RemoveFreeList((void*)((size_t)memory - leftSize));
			if(rightSize)
				RemoveFreeList((void*)((size_t)memory + size));

			// Calculate the new free block's size
			size_t freeBlockSize = size + leftSize + rightSize;

			InsertFreeList(&largeFreeList, (void*)((size_t)memory - leftSize), freeBlockSize);

			// Unlock the mutex
			largeFreeListMutex = 0;
		}
	}
}

// Operator new and delete overloads
void* operator new(size_t size) {
	// Allocate the memory
	void* memory = wfe::AllocMemory(size);
	if(!memory)
		throw std::bad_alloc();
	return memory;
}
void* operator new[](size_t size) {
	// Allocate the memory
	void* memory = wfe::AllocMemory(size);
	if(!memory)
		throw std::bad_alloc();
	return memory;
}
void* operator new(size_t size, std::align_val_t alignment) {
	// Allocate the memory
	void* memory = wfe::AllocMemory(size, (size_t)alignment);
	if(!memory)
		throw std::bad_alloc();
	return memory;
}
void* operator new[](size_t size, std::align_val_t alignment) {
	// Allocate the memory
	void* memory = wfe::AllocMemory(size, (size_t)alignment);
	if(!memory)
		throw std::bad_alloc();
	return memory;
}
void* operator new(size_t size, const std::nothrow_t& tag) noexcept {
	// Allocate the memory
	return wfe::AllocMemory(size);
}
void* operator new[](size_t size, const std::nothrow_t& tag) noexcept {
	// Allocate the memory
	return wfe::AllocMemory(size);
}
void* operator new(size_t size, std::align_val_t alignment, const std::nothrow_t& tag) noexcept {
	// Allocate the memory
	return wfe::AllocMemory(size, (size_t)alignment);
}
void* operator new[](size_t size, std::align_val_t alignment, const std::nothrow_t& tag) noexcept {
	// Allocate the memory
	return wfe::AllocMemory(size, (size_t)alignment);
}

void operator delete(void* memory) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete[](void* memory) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete(void* memory, std::align_val_t alignment) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete[](void* memory, std::align_val_t alignment) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete(void* memory, size_t size) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete[](void* memory, size_t size) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete(void* memory, size_t size, std::align_val_t alignment) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete[](void* memory, size_t size, std::align_val_t alignment) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete(void* memory, const std::nothrow_t& tag) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete[](void* memory, const std::nothrow_t& tag) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete(void* memory, std::align_val_t alignment, const std::nothrow_t& tag) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}
void operator delete[](void* memory, std::align_val_t alignment, const std::nothrow_t& tag) noexcept {
	// Free the memory
	wfe::FreeMemory(memory);
}