#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include "Core.hpp"

namespace wfe {
    /// @brief Returns the width of the main window.
    size_t GetMainWindowWidth();
    /// @brief Returns the height of the main window.
    size_t GetMainWindowHeight();
    /// @brief Returns the name of the application.
    string GetMainWindowName();
    /// @brief Sets the name of the application.
    /// @param newName The new name of the application.
    void SetMainWindowName(const string& newName);

    /// @brief Returns the handle of the window. Meant for internal use.
    HWND GetMainWindowHandle();
    /// @brief Returns the handle of the Windows instance. Meant for internal use.
    HINSTANCE GetWindowsInstance();
}