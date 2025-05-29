#include "VirtualMemory.hpp"
#include "Core/Types/BuildInfo.hpp"

#if defined(WFE_PLATFORM_WINDOWS)
namespace wfe {
	void* VirtualMemoryReserve(void* address, size_t size) {
		return VirtualAlloc(address, size, MEM_RESERVE, PAGE_NOACCESS);
	}
	bool VirtualMemoryRelease(void* address) {
		return VirtualFree(address, 0, MEM_RELEASE);
	}
	bool VirtualMemoryCommit(void* address, size_t size) {
		return VirtualAlloc(address, size, MEM_COMMIT, PAGE_READWRITE) != nullptr;
	}
	bool VirtualMemoryDecommit(void* address, size_t size) {
		return VirtualFree(address, size, MEM_DECOMMIT);
	}
}
#endif
