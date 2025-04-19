#pragma once

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

		/// @brief Creates a window.
		/// @param width The width of the window.
		/// @param height The height of the window.
		/// @param title The title of the window.
		Window(uint32_t width, uint32_t height, const std::string& title);
		Window(const Window&) = delete;
		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

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
		/// @brief Gets the window's title.
		/// @return The window's title.
		const std::string& GetTitle() const {
			return this->title; 
		}
		/// @brief Checks if the window is running.
		/// @return True if the window is running, otherwise false.
		bool IsRunning() const { 
			return this->running; 
		}
		/// @brief Gets the window's platform-specific data.
		/// @return The window's platform-specific data.
		const PlatformData& GetPlatformData() const {
			return this->platformData; 
		}

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

		uint32_t width;
		uint32_t height;
		std::string title;
		bool running = true;
		PlatformData platformData;
	};  
}