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

	/// @brief A struct containing all info packed with window resize events.
	struct WindowResizeEventInfo {
		/// @brief The new width of the window.
		uint32_t newWidth;
		/// @brief The new height of the window.
		uint32_t newHeight;
		/// @brief True if the window was maximized, otherwise false.
		bool8_t windowMaximized;
		/// @brief True if the window was unmaximized, otherwise false.
		bool8_t windowUnmaximized;
		/// @brief True if the window was minimized, otherwise false.
		bool8_t windowMinimized;
		/// @brief True if the window was unminimized, otherwise false.
		bool8_t windowUnminimized;
	};
	/// @brief A struct containing all info packed with window move events.
	struct WindowMoveEventInfo {
		/// @brief The new X coordinate of the window.
		int32_t newX;
		/// @brief The new Y coordinate of the window.
		int32_t newY;
	};

	/// @brief Creates the game's window.
	void CreateWindow();
	/// @brief Destroys the game's window.
	void DestroyWindow();
	/// @brief Polls the window's events.
	void PollWindowEvents();

	/// @brief Gets the window resize event.
	/// @return A reference to the window resize event.
	Event& GetWindowResizeEvent();
	/// @brief Gets the window move event.
	/// @return A reference to the window move event.
	Event& GetWindowMoveEvent();

	/// @brief Gets the window's platform specific info.
	/// @return The window platform info struct.
	WindowPlatformInfo GetWindowPlatformInfo();
}