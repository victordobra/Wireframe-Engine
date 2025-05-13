#pragma once

#include "Core/Types/Defines.hpp"
#include "Platform/Window.hpp"
#include "VulkanInstance.hpp"

namespace wfe {
	/// @brief A class implementing a Vulkan window surface.
	class VulkanSurface {
	public:
		/// @brief Creates a Vulkan window surface.
		/// @param instance The Vulkan instance owning the surface.
		/// @param window The window to create the surface of.
		VulkanSurface(VulkanInstance* instance, Window* window);
		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface(VulkanSurface&&) = delete;

		VulkanSurface& operator=(const VulkanSurface&) = delete;
		VulkanSurface& operator=(VulkanSurface&&) = delete;

		/// @brief Gets the Vulkan instance owning this surface.
		/// @return The Vulkan instance owning this surface.
		VulkanInstance* GetInstance() const {
			return instance;
		}
		/// @brief Gets the surface's window.
		/// @return The surface's window.
		Window* GetWindow() const {
			return window;
		}

		/// @brief Gets the Vulkan surface's internal handle.
		/// @return The Vulkan surface's internal handle.
		VkSurfaceKHR GetSurface() const {
			return surface;
		}

		/// @brief Destroys the Vulkan surface
		~VulkanSurface();
	private:
		VulkanInstance* instance;
		Window* window;

		VkSurfaceKHR surface;
	};
}