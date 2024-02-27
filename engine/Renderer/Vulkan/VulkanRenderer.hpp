#pragma once

#include "Instance/VulkanAllocator.hpp"
#include "Instance/VulkanCommandPool.hpp"
#include "Instance/VulkanDevice.hpp"
#include "Instance/VulkanInstance.hpp"
#include "Instance/VulkanSurface.hpp"
#include "Instance/VulkanSwapChain.hpp"
#include "Loader/VulkanLoader.hpp"

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A renderer that uses the Vulkan API.
	class VulkanRenderer {
	public:
	 	/// @brief The maximum number of frames in flight at any moment.
	 	static const size_t MAX_FRAMES_IN_FLIGHT = 2;
		/// @brief The allocation callbacks used by all Vulkan functions.
		static const VkAllocationCallbacks VULKAN_ALLOC_CALLBACKS;

		/// @brief Creates a renderer that uses the Vulkan API.
		/// @param window The window the renderer will display to, or nullptr if the renderer will be compute only.
		/// @param debugEnabled True if debugging should be enabled, otherwise false.
		/// @param logger The logger to use for general messages.
		VulkanRenderer(Window* window, bool8_t debugEnabled, Logger* logger);
		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) noexcept = delete;

		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

		/// @brief Destroys the Vulkan renderer.
		~VulkanRenderer();
	private:
		Window* window;
		Logger* logger;

		VulkanLoader* loader;
		VulkanInstance* instance;
		VulkanSurface* surface;
		VulkanDevice* device;
		VulkanCommandPool* graphicsCommandPool;
		VulkanCommandPool* presentCommandPool;
		VulkanCommandPool* transferCommandPool;
		VulkanCommandPool* computeCommandPool;
		VulkanAllocator* allocator;
		VulkanSwapChain* swapChain;
	};
}
