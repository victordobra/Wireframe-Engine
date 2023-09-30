#include "Renderer.hpp"
#include "Instance.hpp"
#include "SwapChain.hpp"
#include <vulkan/vulkan.h>

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
		if(!CreateVulkanInstance(debugEnabled)) return false;
		CreateVulkanSurface();

		return true;
	}
	void DeleteVulkanRenderer() {
		// Delete every Vulkan component
		DeleteVulkanSurface();
		DeleteVulkanInstance();
	}
}