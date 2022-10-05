#pragma once

#include "Core.hpp"
#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

namespace wfe {
#if defined(PLATFORM_WINDOWS)
    /// @brief The Vulkan platform extension name (Windows).
#define WFE_VK_PLATFORM_EXTENSION VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(PLATFORM_LINUX)
    /// @brief The Vulkan platform extension name (Linux).
#define WFE_VK_PLATFORM_EXTENSION VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
    /// @brief The default width of the window.
    const size_t DEFAULT_WINDOW_WIDTH  = 1200;
    /// @brief The default height of the window.
    const size_t DEFAULT_WINDOW_HEIGHT = 700;

#if defined(PLATFORM_WINDOWS)
    /// @brief The platform specific info (Windows).
    struct PlatformInfo {
        /// @brief The handle to the Windows instance.
        HINSTANCE hInstance;
        /// @brief The handle to the main window.
        HWND hWnd;
    };
#elif defined(PLATFORM_LINUX)
    /// @brief The platform specific info (Linux).
    struct PlatformInfo {        
        /// @brief A pointer to the X connection.
        Display* display;
        /// @brief The index of the main screen.
        int32_t screenIndex;
        /// @brief A pointer to the main screen.
        Screen* screen;
        /// @brief The main window.
        Window window;
        /// @brief The state of the window manager.
        Atom windowManagerState;
        /// @brief The window delete message atom.
        Atom deleteMessageAtom;
        /// @brief The window fullscreen message atom.
        Atom fullscreenMessageAtom;
    };
#endif

    /// @brief Creates the platform. Internal use only.
    void CreatePlatform();
    /// @brief Deletes the platform. Internal use only.
    void DeletePlatform();
    /// @brief Polls all new platform events. Meant for internal use.
    void PollPlatformEvents();

    /// @brief Gets the platform info.
    /// @return A pointer to the platform info.
    PlatformInfo* GetPlatformInfo();

    /// @brief Creates a Vulkan surface for the platform.
    /// @param instance The Vulkan instance.
    /// @param pAllocator A pointer to the Vulkan allocation callbacks.
    /// @param pSurface A pointer to store the resulting surface in.
    /// @return VK_SUCCESS if the function succeeded, otherwise the appropriate return code.
    VkResult CreatePlatformSurface(VkInstance instance, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
}