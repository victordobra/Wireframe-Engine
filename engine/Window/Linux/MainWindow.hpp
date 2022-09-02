#pragma once

#include <xcb/xcb.h>
#include <xcb/xfixes.h>
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

    /// @brief Returns a pointer to the XCB screen connection. Meant for internal use.
    xcb_connection_t* GetScreenConnection();
    /// @brief Returns the window's handle. Meant for internal use.
    xcb_window_t GetWindowHandle();
    /// @brief Platform specific function for if a key is down. Use wfe::KeyDown() instead.
    /// @param keyCode The keycode to check.
    /// @return True if the key is down, otherwise false.
    bool8_t IsLinuxKeyDown(KeyCode keyCode);
    /// @brief Platform specific function for the mouse's position. Use wfe::GetMousePos() instead.
    MousePos GetLinuxMousePos();
}