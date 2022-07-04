#pragma once

#include <windows.h>
#include "Core.hpp"

namespace wfe {
    typedef HWND WindowHandle;

    /// @brief Returns the width of the main window.
    size_t GetWidth();
    /// @brief Returns the height of the main window.
    size_t GetHeight();
    /// @brief Returns the name of the application.
    const char_t* GetName();
    /// @brief Sets the name of the application.
    /// @param newName The new name of the application.
    void SetName(const char_t* newName);
    /// @brief Returns the handle of the window. Meant for internal use.
    WindowHandle GetHandle();
    /// @brief Returns the handle of the Windows instance. Meant for internal use.
    HINSTANCE GetWindowsInstance();
}