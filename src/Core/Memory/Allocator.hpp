#pragma once

#include "Core/Types/Defines.hpp"
#include <type_traits>
#include <memory>

namespace wfe {
	/// @brief Allocates the requested memory.
	/// @param size The size, in bytes, of the memory to allocate.
	/// @param alignment The alignment, in bytes, of the memory to allocate. Must be a power of two.
	/// @return A pointer to the allocated memory, or nullptr if the allocation failed.
	void* AllocMemory(size_t size, size_t alignment = sizeof(size_t) << 1);
	/// @brief Reallocates the given memory.
	/// @param memory The memory to reallocate.
	/// @param size The new size, in bytes, of the given memory block.
	/// @param alignment The alignment, in bytes, of the memory to reallocate. Must be a power of two.
	/// @return A pointer to the new reallocated memory, or nullptr if the allocation failed.
	void* ReallocMemory(void* memory, size_t size, size_t alignment = sizeof(size_t) << 1);
	/// @brief Frees the given memory.
	/// @param memory A pointer to the memory block to free.
	void FreeMemory(void* memory);
}