#pragma once

#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief Reserves a block of virtual address space.
	/// @param address The address to reserve, or nullptr if the operating system should choose the address.
	/// @param size The size of the block to reserve in bytes.
	/// @return A pointer to the reserved memory block, or nullptr if the reservation failed.
	void* VirtualMemoryReserve(void* address, size_t size);
	/// @brief Releases a block of virtual address space.
	/// @param address The address of the block to release.
	/// @return True if the release was successful, otherwise false.
	bool VirtualMemoryRelease(void* address);
	/// @brief Commits a block from reserved virtual address space.
	/// @param address The address of the block to commit.
	/// @param size The size of the block to commit in bytes.
	/// @return True if the commit was successful, otherwise false.
	bool VirtualMemoryCommit(void* address, size_t size);
	/// @brief Decommits a block of committed virtual address space.
	/// @param address The address of the block to decommit.
	/// @param size The size of the block to decommit in bytes.
	/// @return True if the decommit was successful, otherwise false.
	bool VirtualMemoryDecommit(void* address, size_t size);
}