#include "Renderer.hpp"
#include "Vulkan/Renderer.hpp"

namespace wfe {
	// Variables
	RendererBackendType backendType = RENDERER_BACKEND_COUNT;

	// Public functions
	void CreateRenderer() {
		// Try to create the Vulkan renderer
		if(CreateVulkanRenderer()) {
			// Set the renderer backend type and exit the function
			backendType = RENDERER_BACKEND_VULKAN;
			WFE_LOG_INFO("Successfully created Vulkan renderer.");

			return;
		}
		WFE_LOG_INFO("Vulkan not supported; reverting to other graphics APIs.");

		// No available graphics API is supported; throw an error
		WFE_LOG_FATAL("No available graphics API is supported!");
	}
	void DestroyRenderer() {
		// Destroy the current rendering backend
		switch(backendType) {
		case RENDERER_BACKEND_VULKAN:
			DestroyVulkanRenderer();
			WFE_LOG_INFO("Destroyed Vulkan renderer.");
			break;
		default:
			WFE_LOG_FATAL("No rendering API was created!");
			break;
		}
	}

	RendererBackendType GetRendererBackendType() {
		return backendType;
	}
}