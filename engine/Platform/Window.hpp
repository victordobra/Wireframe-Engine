#pragma once

#include <Core.hpp>
#include "Input.hpp"

// Platform includes
#if defined(WFE_PLATFORM_WINDOWS)
#include <windef.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

namespace wfe {
	/// @brief A normal window to be used by the program.
	class Window {
	public:
		/// @brief A struct containing general information about the window.
		struct WindowInfo {
			/// @brief The window's name.
			string name;
			/// @brief The window's X position.
			int32_t posX;
			/// @brief The window's Y position.
			int32_t posY;
			/// @brief The window's width.
			uint32_t width;
			/// @brief The window's height.
			uint32_t height;
			/// @brief True if the window is maximized, otherwise false.
			bool8_t isMaximized;
			/// @brief True if the window is minimized, otherwise false.
			bool8_t isMinimized;
		};

#if defined(WFE_PLATFORM_WINDOWS)
		/// @brief The game window's Windows specific info.
		struct PlatformInfo {
			/// @brief The handle to the Windows instance.
			HINSTANCE hInstance;
			/// @brief The unique identifier of the window's class.
			ATOM winClassID;
			/// @brief The handle to the window.
			HWND hWnd;
		};
#elif defined(WFE_PLATFORM_LINUX)
		/// @brief The game window's Linux specific info.
		struct PlatformInfo {
			/// @brief A pointer to the X display.
			Display* display;
			/// @brief The index of the current screen.
			int32_t screenIndex;
			/// @brief The screen with the previous index.
			Screen* screen;
			/// @brief The handle to the window.
			Window window;
		};
#endif

		/// @brief A struct containing the info packed with window resize events.
		struct ResizeEventInfo {
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
		/// @brief A struct containing the info packed with window move events.
		struct MoveEventInfo {
			/// @brief The new X coordinate of the window.
			int32_t newX;
			/// @brief The new Y coordinate of the window.
			int32_t newY;
		};
		/// @brief A struct containing the info packed with window rename events.
		struct RenameEventInfo {
			/// @brief The new name of the window.
			string newName;
		};

		/// @brief Creates a new window.
		/// @param windowInfo The window's info.
		Window(WindowInfo windowInfo);
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		/// @brief Polls the window's events.
		void PollEvents();

		/// @brief Gets the window resize event.
		/// @return A reference to the window resize event.
		Event& GetResizeEvent() {
			return resizeEvent;
		}
		/// @brief Gets the window move event.
		/// @return A reference to the window move event.
		Event& GetMoveEvent() {
			return moveEvent;
		}
		/// @brief Gets the window rename event.
		/// @return A reference to the window rename event.
		Event& GetRenameEvent() {
			return renameEvent;
		}
		/// @brief Gets the window close event.
		/// @return A reference to the window close event.
		Event& GetCloseEvent() {
			return closeEvent;
		}

		/// @brief Gets the window's general info.
		/// @return The window info struct.
		const WindowInfo& GetWindowInfo() const {
			return windowInfo;
		}
		/// @brief Gets the window's platform specific info.
		/// @return The window platform info struct.
		const PlatformInfo& GetPlatformInfo() const {
			return platformInfo;
		}
		/// @brief Gets the window's input manager.
		/// @return A pointer to the window's input manager.
		InputManager* GetInputManager() {
			return &inputManager;
		}
		/// @brief Gets the window's input manager.
		/// @return A const pointer to the window's input manager.
		const InputManager* GetInputManager() const {
			return &inputManager;
		}

		/// @brief Sets the window's name.
		/// @param newName The window's new name.
		void SetWindowName(const string& newName);
		/// @brief Sets the window's position.
		/// @param newX The window's new X coordinate.
		/// @param newY The window's new Y coordinate.
		void SetWindowPos(int32_t newX, int32_t newY);
		/// @brief Set the window's size.
		/// @param newWidth The window's new width.
		/// @param newHeight The window's new height.
		/// @param maximized True if the window will be set to maximized, otherwise false.
		/// @param minimized True if the window will be set to minimized, otherwise false.
		void SetWindowSize(uint32_t newWidth, uint32_t newHeight, bool8_t maximized, bool8_t minimized);

		/// @brief Destroys the window.
		~Window();
	private:
		uint64_t ProcessInternalEvent(void* args);

		WindowInfo windowInfo;
		PlatformInfo platformInfo;
		InputManager inputManager;

		Event resizeEvent{};
		Event moveEvent{};
		Event renameEvent{};
		Event closeEvent{};
	};
}