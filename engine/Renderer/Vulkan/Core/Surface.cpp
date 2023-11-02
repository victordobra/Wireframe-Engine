#include "Surface.hpp"
#include "Allocator.hpp"
#include "Device.hpp"
#include "Instance.hpp"
#include "Platform/Window.hpp"

// Include Vulkan with the current platform define
#if defined(WFE_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(WFE_PLATFORM_LINUX)
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Variables
	static VkSurfaceKHR surface;
	static vector<VkSurfaceFormatKHR> supportedSurfaceFormats;
	static vector<VkPresentModeKHR> supportedSurfacePresentModes;

	// Public functions
	void CreateVulkanSurface() {
		// Get the window platform info
		WindowPlatformInfo platformInfo = GetWindowPlatformInfo();

#if defined(WFE_PLATFORM_WINDOWS)
		// Set the Win32 surface create info
		VkWin32SurfaceCreateInfoKHR createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.hinstance = platformInfo.hInstance;
		createInfo.hwnd = platformInfo.hWnd;

		// Create the Win32 surface
		VkResult result = vkCreateWin32SurfaceKHR(GetVulkanInstance(), &createInfo, GetVulkanAllocCallbacks(), &surface);
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Win32 Vulkan window surface! Error code: %s", string_VkResult(result));
#elif defined(WFE_PLATFORM_LINUX)
		// Set the Xlib surface create info
		VkXlibSurfaceCreateInfoKHR createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.dpy = platformInfo.display;
		createInfo.window = platformInfo.window;

		// Create the Xlib surface
		VkResult result = vkCreateXlibSurfaceKHR(GetVulkanInstance(), &createInfo, GetVulkanAllocCallbacks(), &surface);
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Xlib Vulkan window surface! Error code: %s", string_VkResult(result));
#endif
	}
	void DestroyVulkanSurface() {
		// Destroy the surface
		vkDestroySurfaceKHR(GetVulkanInstance(), surface, GetVulkanAllocCallbacks());
	}
	void GenerateVulkanSurfaceSupportDetails() {
		// Get the surface's supported formats
		uint32_t supportedSurfaceFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(GetVulkanPhysicalDevice(), surface, &supportedSurfaceFormatCount, nullptr);
		supportedSurfaceFormats.resize(supportedSurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(GetVulkanPhysicalDevice(), surface, &supportedSurfaceFormatCount, supportedSurfaceFormats.data());

		// Get the surface's supported present modes
		uint32_t supportedSurfacePresentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(GetVulkanPhysicalDevice(), surface, &supportedSurfacePresentModeCount, nullptr);
		supportedSurfacePresentModes.resize(supportedSurfacePresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(GetVulkanPhysicalDevice(), surface, &supportedSurfacePresentModeCount, supportedSurfacePresentModes.data());

		// Shrink the supported surface formats and present modes vectors to fit
		supportedSurfaceFormats.shrink_to_fit();
		supportedSurfacePresentModes.shrink_to_fit();
	}

	VkSurfaceKHR GetVulkanSurface() {
		return surface;
	}
	const vector<VkSurfaceFormatKHR>& GetVulkanSurfaceSupportedFormats() {
		return supportedSurfaceFormats;
	}
	const vector<VkPresentModeKHR>& GetVulkanSurfaceSupportedPresentModes() {
		return supportedSurfacePresentModes;
	}
}