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
	static HINSTANCE hInstance;
	static ATOM classID;
	static HWND hWindow;

	// WinProc definition
	static LRESULT CALLBACK WinProc(_In_ HWND wnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

	// Internal helper functions
	static void RegisterWindowClass() {
		// Get a handle to the current instance
		hInstance = GetModuleHandleA(nullptr);

		// Set the class info
		WNDCLASSEXA winClass;

		winClass.cbSize = sizeof(WNDCLASSEXA);
		winClass.style = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc = WinProc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = hInstance;
		winClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winClass.hbrBackground = nullptr;
		winClass.lpszMenuName = nullptr;
		winClass.lpszClassName = CLASS_NAME;
		winClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

		// Register the class with the given info
		classID = RegisterClassExA(&winClass);

		if(!classID)
			WFE_LOG_FATAL("Failed to register window class!")
	}
	static void CreateWin32Window() {
		// Create the window
		hWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, CLASS_NAME, WFE_PROJECT_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

		if(!hWindow)
			WFE_LOG_FATAL("Failed to create the game's window!")
		
		// Show the window as maximized and update it
		ShowWindow(hWindow, SW_MAXIMIZE);
		UpdateWindow(hWindow);
	}

	LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		switch(message) {
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
	void DeleteWindow() {
		// No action required to delete the window
	}
	void PollWindowEvents() {
		// Loop through every pending message
		MSG message;

		while(PeekMessageA(&message, hWindow, 0, 0, PM_REMOVE)) {
			// Translate and dispatch the message to be processed
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}

	WindowPlatformInfo GetWindowPlatformInfo() {
		return { hInstance, hWindow };
	}
}

#endif