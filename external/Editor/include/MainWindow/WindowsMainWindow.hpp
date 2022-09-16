#pragma once

#include "Core.hpp"
#include <windows.h>

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

    /// @brief Returns the window's handle. Internal use only.
    HWND GetWindowHandle();
    /// @brief Returns the Windows instance's handle. Internal use only.
    HINSTANCE GetWindowsInstance();
}