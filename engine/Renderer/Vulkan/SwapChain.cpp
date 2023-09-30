#include "SwapChain.hpp"
#include "Allocator.hpp"
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
    // Internal variables
    static VkSurfaceKHR surface;

    // Public functions
    void CreateVulkanSurface() {
        // Get the window platform info
        WindowPlatformInfo platformInfo = GetWindowPlatformInfo();

#if defined(WFE_PLATFORM_WINDOWS)
        // Create the Win32 surface
        VkWin32SurfaceCreateInfoKHR createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.hinstance = platformInfo.hInstance;
        createInfo.hwnd = platformInfo.hWnd;

        VkResult result = vkCreateWin32SurfaceKHR(GetVulkanInstance(), &createInfo, GetVulkanAllocCallbacks(), &surface);
        if(result != VK_SUCCESS)
            WFE_LOG_FATAL("Failed to create Win32 Vulkan window surface! Error code: %s", string_VkResult(result));
#elif defined(WFE_PLATFORM_LINUX)
        // Create the Xlib surface
        VkXlibSurfaceCreateInfoKHR createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.dpy = platformInfo.display;
        createInfo.window = platformInfo.window;

        VkResult result = vkCreateXlibSurfaceKHR(GetVulkanInstance(), &createInfo, GetVulkanAllocCallbacks(), &surface);
        if(result != VK_SUCCESS)
            WFE_LOG_FATAL("Failed to create Xlib Vulkan window surface! Error code: %s", string_VkResult(result));
#endif
    }
    void DeleteVulkanSurface() {
        // Destroy the surface
        vkDestroySurfaceKHR(GetVulkanInstance(), surface, GetVulkanAllocCallbacks());
    }
}