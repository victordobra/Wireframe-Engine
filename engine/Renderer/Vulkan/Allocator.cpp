#include "Allocator.hpp"

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

    // Internal variables
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
}