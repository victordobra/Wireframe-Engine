#include "Renderer.hpp"
#include "Instance.hpp"
#include <vulkan/vulkan.h>

namespace wfe {
    // Public functions
	bool8_t CreateVulkanRenderer() {
        // Create every Vulkan component, exiting the function if any of them return false
        if(!CreateVulkanInstance()) return false;

        return true;
    }
	void DeleteVulkanRenderer() {
        // Delete every Vulkan component
        DeleteVulkanInstance();
    }
}