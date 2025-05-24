#include "Allocator.hpp"
#include <stdlib.h>

namespace wfe {
	// Public functions
	void* AllocMemory(size_t size, size_t alignment) {
		return malloc(size);
	}
	void* ReallocMemory(void* memory, size_t size, size_t alignment) {
		return realloc(memory, size);
	}
	void FreeMemory(void* memory) {
		free(memory);
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

void operator delete(void* memory) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete[](void* memory) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete(void* memory, std::align_val_t alignment) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete[](void* memory, std::align_val_t alignment) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete(void* memory, size_t size) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete[](void* memory, size_t size) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete(void* memory, size_t size, std::align_val_t alignment) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}
void operator delete[](void* memory, size_t size, std::align_val_t alignment) noexcept {
	if(memory) {
		// Free the memory
		wfe::FreeMemory(memory);
	}
}