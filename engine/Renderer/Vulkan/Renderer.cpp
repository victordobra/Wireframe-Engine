#include "Renderer.hpp"

namespace wfe {
	// Internal variables
#ifdef WFE_BUILD_MODE_DEBUG
	static bool8_t debugEnabled = true;
#else
	static bool8_t debugEnabled = false;
#endif

	// Public functions
	bool8_t CreateVulkanRenderer() {
		// Create every Vulkan component, exiting the function if any of them return false
		if(!LoadVulkanFunctions()) return false;
		if(!CreateVulkanInstance(debugEnabled)) return false;
		CreateVulkanSurface();
		if(!CreateVulkanDevice()) {
			WFE_LOG_INFO("No suitable Vulkan device found; destroying previously created Vulkan objects.");
			DestroyVulkanSurface();
			DestroyVulkanInstance();
			FreeVulkanFunctions();

			return false;
		}

		// Vulkan support is guaranteed; create all other Vulkan elements without checking for support
		CreateVulkanCommandPools();

		return true;
	}
	void DestroyVulkanRenderer() {
		// Destroy every Vulkan component
		DestroyVulkanCommandPools();
		DestroyVulkanDevice();
		DestroyVulkanSurface();
		DestroyVulkanInstance();
		FreeVulkanFunctions();
	}
}