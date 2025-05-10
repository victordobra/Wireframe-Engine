#include "VulkanSurface.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

#if defined(WFE_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(WFE_PLATFORM_LINUX)
#endif

namespace wfe {
	// Public functions
	VulkanSurface::VulkanSurface(VulkanInstance* instance, Window* window) {
#if defined(WFE_PLATFORM_WINDOWS)
		// Set the surface create info
		VkWin32SurfaceCreateInfoKHR createInfo {
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.hinstance = window->GetPlatformData().hInstance,
			.hwnd = window->GetPlatformData().hWnd
		};

		// Create the surface
		VkResult result = instance->GetLoader()->vkCreateWin32SurfaceKHR(instance->GetInstance(), &createInfo, nullptr, &this->surface);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan Win32 window surface! Error code: " + string_VkResult(result));
#elif defined(WFE_PLATFORM_LINUX)
#endif
	}

	VulkanSurface::~VulkanSurface() {
		// Destroy the surface
		vkDestroySurfaceKHR(instance->GetInstance(), this->surface, nullptr);
	}
}