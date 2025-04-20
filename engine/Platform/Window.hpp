#pragma once

#include "Core/Management/Event.hpp"
#include "Core/Types/BuildInfo.hpp"
#include "Core/Types/Defines.hpp"
#include <string>

#if defined(WFE_PLATFORM_WINDOWS)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#elif defined(WFE_PLATFORM_LINUX)
#endif

namespace wfe {
	/// @brief A class describing a graphical window.
	class Window {
	public:
#if defined(WFE_PLATFORM_WINDOWS)
		/// @brief A structure containing the window's platform-specific data.
		struct PlatformData {
			/// @brief The instance handle.
			HINSTANCE hInstance;
			/// @brief The window class's ID.
			ATOM winClassID;
			/// @brief The window's handle.
			HWND hWnd;
		};
#elif defined(WFE_PLATFORM_LINUX)
#endif

		/// @brief A structure containing the window's move data, passed as an argument to all listeners of the move event. 
		struct MoveEventData {
			/// @brief The new x position of the window.
			int32_t x;
			/// @brief The new y position of the window.
			int32_t y;
		};
		/// @brief A structure containing the window's resize data, passed as an argument to all listeners of the resize event.
		struct ResizeEventData {
			/// @brief The new width of the window.
			uint32_t width;
			/// @brief The new height of the window.
			uint32_t height;
			/// @brief True if the window is minimized, otherwise false.
			bool minimized;
			/// @brief True if the window is maximized, otherwise false.
			bool maximized;
		};
		/// @brief A structure containing the window's rename data, passed as an argument to all listeners of the rename event.
		struct RenameEventData {
			/// @brief The new title of the window.
			std::string title;
		};

		/// @brief Creates a window.
		/// @param x The X position of the window.
		/// @param y The Y position of the window.
		/// @param width The width of the window.
		/// @param height The height of the window.
		/// @param title The title of the window.
		Window(int32_t x, int32_t y, uint32_t width, uint32_t height, const std::string& title);
		Window(const Window&) = delete;
		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		/// @brief Gets the window's X position.
		/// @return The window's X position.
		int32_t GetX() const {
			return this->x; 
		}
		/// @brief Gets the window's Y position.
		/// @return The window's Y position.
		int32_t GetY() const {
			return this->y; 
		}
		/// @brief Gets the window's width.
		/// @return The window's width.
		uint32_t GetWidth() const {
			return this->width;
		}
		/// @brief Gets the window's height.
		/// @return The window's height.
		uint32_t GetHeight() const {
			return this->height; 
		}
		/// @brief Checks if the window is minimized.
		/// @return True if the window is minimized, otherwise false.
		bool IsMinimized() const {
			return this->minimized; 
		}
		/// @brief Checks if the window is maximized.
		/// @return True if the window is maximized, otherwise false.
		bool IsMaximized() const {
			return this->maximized; 
		}
		/// @brief Gets the window's title.
		/// @return The window's title.
		const std::string& GetTitle() const {
			return this->title; 
		}
		/// @brief Gets the window's platform-specific data.
		/// @return The window's platform-specific data.
		const PlatformData& GetPlatformData() const {
			return this->platformData; 
		}

		/// @brief Gets the window's move event.
		/// @return A reference to the window's move event.
		Event& GetMoveEvent() {	
			return this->moveEvent; 
		}
		/// @brief Gets the window's resize event.
		/// @return A reference to the window's resize event.
		Event& GetResizeEvent() {
			return this->resizeEvent; 
		}
		/// @brief Gets the window's rename event.
		/// @return A reference to the window's rename event.
		Event& GetRenameEvent() {
			return this->renameEvent; 
		}
		/// @brief Gets the window's close event.
		/// @return A reference to the window's close event.
		Event& GetCloseEvent() {
			return this->closeEvent; 
		}

		/// @brief Set the window's position.
		/// @param x The new X position of the window.
		/// @param y The new Y position of the window.
		void SetPos(int32_t x, int32_t y);
		/// @brief Sets the window's size.
		/// @param width The new width of the window.
		/// @param height The new height of the window.
		void SetSize(uint32_t width, uint32_t height);
		/// @brief Sets the window's title.
		/// @param title The new title of the window.
		void SetTitle(const std::string& title);

		/// @brief Polls the window's events.
		void PollEvents();

		/// @brief Destroys the window.
		~Window();
	private:
#if defined(WFE_PLATFORM_WINDOWS)
		static LRESULT CALLBACK GlobalProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

		int32_t x, y;
		uint32_t width, height;
		bool minimized = false, maximized = false;
		std::string title;
		PlatformData platformData;

		Event moveEvent;
		Event resizeEvent;
		Event renameEvent;
		Event closeEvent;
	};  
}