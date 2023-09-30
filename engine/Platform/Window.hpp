#pragma once

#include <Core.hpp>

// Platform includes
#if defined(WFE_PLATFORM_WINDOWS)
#include <windef.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

namespace wfe {
#if defined(WFE_PLATFORM_WINDOWS)
	/// @brief The game window's Windows specific info.
	struct WindowPlatformInfo {
		/// @brief The handle to the Windows instance.
		HINSTANCE hInstance;
		/// @brief The handle to the game window.
		HWND hWnd;
	};
#elif defined(WFE_PLATFORM_LINUX)
	/// @brief The game window's Linux specific info.
	struct WindowPlatformInfo {
		/// @brief A pointer to the X display.
		Display* display;
		/// @brief The index of the current screen.
		int32_t screenIndex;
		/// @brief The screen with the previous index.
		Screen* screen;
		/// @brief The handle to the game window.
		Window window;
	};
#endif

	/// @brief Creates the game's window.
	void CreateWindow();
	/// @brief Deletes the game's window.
	void DeleteWindow();
	/// @brief Polls the window's events.
	void PollWindowEvents();

	/// @brief Gets the window's platform specific info.
	/// @return The window platform info struct.
	WindowPlatformInfo GetWindowPlatformInfo();
}