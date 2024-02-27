#include "VulkanSurface.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"

#if defined(WFE_PLATFORM_WINDOWS)
#include <vulkan/vulkan_win32.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <vulkan/vulkan_xcb.h>
#endif
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
	VulkanSurface::VulkanSurface(VulkanInstance* instance, Window* window) : instance(instance), window(window) {
		// Load the window's platform info
		Window::PlatformInfo platformInfo = window->GetPlatformInfo();

#if defined(WFE_PLATFORM_WINDOWS)
		// Set the Win32 surface create info
		VkWin32SurfaceCreateInfoKHR createInfo {
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.hinstance = platformInfo.hInstance,
			.hwnd = platformInfo.hWnd
		};

		// Create the Win32 surface
		VkResult result = instance->GetLoader()->vkCreateWin32SurfaceKHR(instance->GetInstance(), &createInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &surface);
		if(result != VK_SUCCESS)
			throw Exception("Failed to create Win32 Vulkan surface! Error code: %s", string_VkResult(result));
#elif defined(WFE_PLATFORM_LINUX)
#endif
	}

	VulkanSurface::~VulkanSurface() {
		// Destroy the surface
		instance->GetLoader()->vkDestroySurfaceKHR(instance->GetInstance(), surface, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);
	}
}