#include <BuildInfo.hpp>

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform/Window.hpp"
#include "ProjectInfo.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef CreateWindow

namespace wfe {
	// Constants
	static const char_t* const CLASS_NAME = "MainWindow";

	// Internal variables
	static WindowInfo windowInfo;
	static WindowPlatformInfo platformInfo;

	static Event resizeEvent{};
	static Event moveEvent{};

	// WinProc definition
	static LRESULT CALLBACK WinProc(_In_ HWND wnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

	// Internal helper functions
	static void RegisterWindowClass() {
		// Get a handle to the current instance
		platformInfo.hInstance = GetModuleHandleA(nullptr);

		// Set the class info
		WNDCLASSEXA winClass;

		winClass.cbSize = sizeof(WNDCLASSEXA);
		winClass.style = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc = WinProc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = platformInfo.hInstance;
		winClass.hIcon = LoadIconA(platformInfo.hInstance, IDI_APPLICATION);
		winClass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
		winClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		winClass.lpszMenuName = nullptr;
		winClass.lpszClassName = CLASS_NAME;
		winClass.hIconSm = LoadIconA(platformInfo.hInstance, IDI_APPLICATION);

		// Register the class with the given info
		platformInfo.winClassID = RegisterClassExA(&winClass);

		if(!platformInfo.winClassID)
			WFE_LOG_FATAL("Failed to register window class!");
		
		WFE_LOG_INFO("Registered Win32 window class.");
	}
	static void CreateWin32Window() {
		// Create the window
		platformInfo.hWnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, CLASS_NAME, WFE_PROJECT_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, platformInfo.hInstance, nullptr);

		if(!platformInfo.hWnd)
			WFE_LOG_FATAL("Failed to create the game's window!");
		
		WFE_LOG_INFO("Created Win32 window");
		
		// Show the window as maximized
		ShowWindow(platformInfo.hWnd, SW_MAXIMIZE);

		// Get the window's size
		RECT clientRect;
		GetClientRect(platformInfo.hWnd, &clientRect);

		// Set the window's info
		windowInfo.posX = 0;
		windowInfo.posY = 0;
		windowInfo.width = (uint32_t)clientRect.right;
		windowInfo.height = (uint32_t)clientRect.bottom;
		windowInfo.isMaximized = true;
		windowInfo.isMinimized = false;
	}

	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		switch(message) {
		case WM_SIZE: {
			// Set the window resize event info
			WindowResizeEventInfo resizeInfo;
			resizeInfo.newWidth = (uint32_t)LOWORD(lParam);
			resizeInfo.newHeight = (uint32_t)HIWORD(lParam);

			switch(wParam) {
			case SIZE_RESTORED:
				// Set the resize info flags
				resizeInfo.windowMaximized = false;
				resizeInfo.windowUnmaximized = windowInfo.isMaximized;
				resizeInfo.windowMinimized = false;
				resizeInfo.windowUnminimized = windowInfo.isMinimized;
				
				// Set the maximized and minimized flags
				windowInfo.isMaximized = false;
				windowInfo.isMinimized = false;
				
				break;
			case SIZE_MINIMIZED:
				// Set the resize info flags
				resizeInfo.windowMaximized = false;
				resizeInfo.windowUnmaximized = false;
				resizeInfo.windowMinimized = true;
				resizeInfo.windowUnminimized = false;
				
				// Set the minimized flag
				windowInfo.isMinimized = true;
				
				break;
			case SIZE_MAXIMIZED:
				// Set the resize info flags
				resizeInfo.windowMaximized = true;
				resizeInfo.windowUnmaximized = false;
				resizeInfo.windowMinimized = false;
				resizeInfo.windowUnminimized = false;
				
				// Set the maximized flag
				windowInfo.isMaximized = true;
				
				break;
			}

			// Call the resize event
			resizeEvent.CallEvent(&resizeInfo);

			// Set the window's new width and height
			windowInfo.width = resizeInfo.newWidth;
			windowInfo.height = resizeInfo.newHeight;

			break;
		}
		case WM_MOVE: {
			// Set the window move event info
			WindowMoveEventInfo moveInfo;
			moveInfo.newX = (int32_t)(int16_t)LOWORD(lParam);
			moveInfo.newY = (int32_t)(int16_t)HIWORD(lParam);

			// Call the move event
			moveEvent.CallEvent(&moveInfo);

			// Set the window's new position
			windowInfo.posX = moveInfo.newX;
			windowInfo.posY = moveInfo.newY;

			break;
		}
		case WM_CLOSE:
		 	// Send a call to destroy the window
			DestroyWindow(hWnd);

			return 0;
		case WM_DESTROY:
			// Close the application
			wfe::CloseGame(0);

			return 0;
		}

		// Process the message as default
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}

	// Public functions
	void CreateWindow() {
		RegisterWindowClass();
		CreateWin32Window();
	}
	void DestroyWindow() {
		// No action required to destroy the window
	}
	void PollWindowEvents() {
		// Loop through every pending message
		MSG message;

		while(PeekMessageA(&message, platformInfo.hWnd, 0, 0, PM_REMOVE)) {
			// Translate and dispatch the message to be processed
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}

	Event& GetWindowResizeEvent() {
		return resizeEvent;
	}
	Event& GetWindowMoveEvent() {
		return moveEvent;
	}

	WindowInfo GetWindowInfo() {
		return windowInfo;
	}
	WindowPlatformInfo GetWindowPlatformInfo() {
		return platformInfo;
	}
}

#endif