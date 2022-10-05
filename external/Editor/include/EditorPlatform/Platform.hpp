#pragma once

#include "Core.hpp"
#include "Vulkan/VulkanInclude.hpp"

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

namespace wfe::editor {
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
        /// @brief The window delete message atom.
        Atom deleteMessageAtom;

        /// @brief The X input method.
        XIM inputMethod;
        /// @brief The X input context.
        XIC inputContext;
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

    /// @brief Checks if the specified location exists.
    /// @param location The location to check.
    /// @return Whether the location exists or not.
    bool8_t LocationExists(const string& location);
    /// @brief Copies the two specified files/folders.
    /// @param srcFile The source file/folder.
    /// @param dstFile The destination file/folder.
    /// @param folders Whether to copy folders or files.
    void CopyFiles(const string& srcFile, const string& dstFile, bool8_t folders = false);
    /// @brief Opens a file open dialog.
    /// @param name The name of the file dialog.
    /// @param canceled A reference to whether the dialog was canceled or not.
    /// @param startingLocation The starting location of the dialog.
    /// @param folders Whether to open a file or a folder.
    /// @return The final returned path.
    string OpenFileDialog(const string& name, bool8_t& canceled, const string& startingLocation = "", bool8_t folders = false);

    /// @brief Creates a Vulkan surface for the platform.
    /// @param instance The Vulkan instance.
    /// @param pAllocator A pointer to the Vulkan allocation callbacks.
    /// @param pSurface A pointer to store the resulting surface in.
    /// @return VK_SUCCESS if the function succeeded, otherwise the appropriate return code.
    VkResult CreatePlatformSurface(VkInstance instance, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
}