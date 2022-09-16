#pragma once

#include "Core.hpp"
#include <X11/Xlib.h>

namespace wfe::editor {
    /// @brief Returns the main window's height.
    size_t GetMainWindowWidth();
    /// @brief Returns the main window's width.
    size_t GetMainWindowHeight();

    /// @brief Returns the main window's name.
    const string& GetMainWindowName();
    /// @brief Set the main window's name.
    void SetMainWindowName(const string& newName);

    /// @brief Creates a open folder name dialog.
    /// @param canceled A reference to a bool which will be set to true if the dialog was canceled.
    /// @param startingLocation Te location for the dialog to start from.
    /// @return The absolute path of the selected folder.
    string OpenFolderDialog(bool8_t& canceled, const string& startingLocation = "");
    /// @brief Copies the contents of the given folder to the destination folder.
    /// @param srcFolder The path of the folder to copy from.
    /// @param dstFolder The path of the folder to copy to.
    void CopyFolder(const string& srcFolder, const string& dstFolder);

    /// @brief Returns a pointer to the XCB screen connection. Meant for internal use.
    Display* GetScreenConnection();
    /// @brief Returns the window's handle. Meant for internal use.
    Window GetWindowHandle();
}