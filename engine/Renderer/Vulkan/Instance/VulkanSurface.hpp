#pragma once

#include "Platform/Window.hpp"
#include "VulkanInstance.hpp"
#include "Renderer/Vulkan/Loader/VulkanLoader.hpp"

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A wrapper for a window's Vulkan surface.
	class VulkanSurface {
	public:
		/// @brief Creates the Vulkan surface of the given window.
		/// @param window The window whose surface to create.
		VulkanSurface(VulkanInstance* instance, Window* window);
		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface(VulkanSurface&&) noexcept = delete;

		VulkanSurface& operator=(const VulkanSurface&) = delete;
		VulkanSurface& operator=(VulkanSurface&&) = delete;

		/// @brief Gets the Vulkan instance that owns the surface.
		/// @return A pointer to the Vulkan instance wrapper object.
		VulkanInstance* GetInstance() {
			return instance;
		}
		/// @brief Gets the Vulkan instance that owns the surface.
		/// @return A const pointer to the Vulkan instance wrapper object.
		const VulkanInstance* GetInstance() const {
			return instance;
		}
		/// @brief Gets the window that owns the surface.
		/// @return A pointer to the window object.
		Window* GetWindow() {
			return window;
		}
		/// @brief Gets the window that owns the surface.
		/// @return A const pointer to the window object.
		const Window* GetWindow() const {
			return window;
		}

		/// @brief Gets the Vulkan surface of the implementation.
		/// @return A handle to the Vulkan surface.
		VkSurfaceKHR GetSurface() {
			return surface;
		}

		/// @brief Destroys the Vulkan surface.
		~VulkanSurface();
	private:
		VulkanInstance* instance;
		Window* window;
		VkSurfaceKHR surface;
	};
}