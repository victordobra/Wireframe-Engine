#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform/Window.hpp"
#include "ProjectInfo.hpp"
#include "Core.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef CreateWindow

namespace wfe {
	// Constants
	const char_t* CLASS_NAME = "MainWindow";

	// Internal variables
	HINSTANCE instance;
	ATOM classID;
	HWND window;

	// WinProc definition
	LRESULT CALLBACK WinProc(_In_ HWND wnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

	// Internal helper functions
	static void RegisterWindowClass() {
		// Get a handle to the current instance
		instance = GetModuleHandleA(nullptr);

		// Set the class info
		WNDCLASSEXA winClass;

		winClass.cbSize = sizeof(WNDCLASSEXA);
		winClass.style = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc = WinProc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = instance;
		winClass.hIcon = LoadIcon(instance, IDI_APPLICATION);
		winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winClass.hbrBackground = nullptr;
		winClass.lpszMenuName = nullptr;
		winClass.lpszClassName = CLASS_NAME;
		winClass.hIconSm = LoadIcon(instance, IDI_APPLICATION);

		// Register the class with the given info
		classID = RegisterClassExA(&winClass);

		if(!classID)
			WFE_LOG_FATAL("Failed to register window class!")
	}
	static void CreateWin32Window() {
		// Create the window
		window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, CLASS_NAME, WFE_PROJECT_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, instance, nullptr);

		if(!window)
			WFE_LOG_FATAL("Failed to create the game's window!")
		
		// Show the window as maximized and update it
		ShowWindow(window, SW_MAXIMIZE);
		UpdateWindow(window);
	}

	LRESULT CALLBACK WinProc(_In_ HWND wnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		switch(message) {
		case WM_CLOSE:
		 	// Send a call to destroy the window
			DestroyWindow(wnd);

			return 0;
		case WM_DESTROY:
			// Close the application
			wfe::CloseGame(0);

			return 0;
		}

		// Process the message as default
		return DefWindowProcA(wnd, message, wParam, lParam);
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

		while(PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
			// Translate and dispatch the message to be processed
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}
}

#endif