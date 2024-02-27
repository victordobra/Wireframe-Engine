#include <BuildInfo.hpp>

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform/Window.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	// HWND hash function
	struct HashHWND {
		uint64_t operator()(HWND hWnd) {
			return Hash<size_t>()((size_t)hWnd);
		}
	};

	// Internal structs
	struct EventInfo {
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;
	};
	struct InternalWindow {
		Window* window;
		uint64_t (Window::*processEvent)(void* params);
	};

	// Constants
	static const size_t ERR_BUFFER_SIZE = 256;

	static const char_t* BASE_64_CHARS = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";
	static const size_t CLASS_NAME_LENGTH = 10;

	// Internal static variables
	static unordered_map<HWND, InternalWindow, HashHWND> windows;
	static AtomicMutex windowsMutex;

	static atomic_uint64_t classCount = 0;

	// WinProc function
	static LRESULT WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Retrieve the window 
		windowsMutex.Lock();
		auto windowIter = windows.find(hWnd);
		auto windowsEnd = windows.end();
		windowsMutex.Unlock();

		// Exit the function if the current hWnd is invalid
		if(windowIter == windowsEnd)
			return DefWindowProcA(hWnd, msg, wParam, lParam);
		
		// Set the event info
		EventInfo eventInfo;

		eventInfo.msg = msg;
		eventInfo.wParam = wParam;
		eventInfo.lParam = lParam;

		// Run the window's internal event process function
		InternalWindow window = windowIter->second;

		return (LRESULT)(window.window->*(window.processEvent))(&eventInfo);
	}

	// Internal functions
	uint64_t Window::ProcessInternalEvent(void* args) {
		// Get the event's info
		EventInfo* eventInfo = (EventInfo*)args;

		UINT msg = eventInfo->msg;
		WPARAM wParam = eventInfo->wParam;
		LPARAM lParam = eventInfo->lParam;

		switch(msg) {
		case WM_SETTEXT: {
			// Set the window's name in the window info
			windowInfo.name = (const char_t*)lParam;

			// Set the window rename event info
			Window::RenameEventInfo renameInfo;

			renameInfo.newName = windowInfo.name;

			// Trigger the rename event
			renameEvent.CallEvent(&renameInfo);

			break;
		}
		case WM_MOVE: {
			// Set the window's position in the window info
			windowInfo.posX = (int32_t)(int16_t)LOWORD(lParam);
			windowInfo.posY = (int32_t)(int16_t)HIWORD(lParam);

			// Set the window move event info
			Window::MoveEventInfo moveInfo;

			moveInfo.newX = windowInfo.posX;
			moveInfo.newY = windowInfo.posY;

			// Trigger the move event
			moveEvent.CallEvent(&moveInfo);

			break;
		}
		case WM_SIZE: {
			// Check if the window is maximized or minimized
			bool8_t maximized = wParam == SIZE_MAXIMIZED;
			bool8_t minimized = wParam == SIZE_MINIMIZED;

			// Set the window resize size event info
			Window::ResizeEventInfo resizeInfo;

			resizeInfo.newWidth = (uint32_t)LOWORD(lParam);
			resizeInfo.newHeight = (uint32_t)HIWORD(lParam);
			resizeInfo.windowMaximized = !windowInfo.isMaximized && maximized;
			resizeInfo.windowUnmaximized = windowInfo.isMaximized && !maximized;
			resizeInfo.windowMinimized = !windowInfo.isMinimized && minimized;
			resizeInfo.windowUnminimized = windowInfo.isMinimized && !minimized;

			// Set the window's size in the window info
			windowInfo.width = resizeInfo.newWidth;
			windowInfo.height = resizeInfo.newHeight;
			windowInfo.isMaximized = maximized;
			windowInfo.isMinimized = minimized;

			// Trigger the resize event
			resizeEvent.CallEvent(&resizeInfo);

			break;
		}
		case WM_CLOSE: {
			// Trigger the close event
			closeEvent.CallEvent(nullptr);

			return 0;
		}
		case WM_GETTEXT: {
			// Get the max length of the buffer and the length of the window's name
			char_t* buffer = (char_t*)lParam;
			size_t nameLen = windowInfo.name.size(), bufferLen = (size_t)wParam;
			
			// Copy the window's name to the given buffer
			if(nameLen + 1 > bufferLen) {
				memcpy(buffer, windowInfo.name.c_str(), bufferLen);

				return (uint64_t)bufferLen;
			} else {
				memcpy(buffer, windowInfo.name.c_str(), nameLen + 1);

				return (uint64_t)nameLen;
			}
		}
		case WM_GETTEXTLENGTH: {
			// Return the length of the name
			return (uint64_t)windowInfo.name.size();
		}
		case WM_KEYDOWN: 
		case WM_SYSKEYDOWN: {
			// Call the input manager's internal key down function
			inputManager.InternalKeyDown((uint64_t)wParam);

			return 0;
		}
		case WM_KEYUP: 
		case WM_SYSKEYUP: {
			// Call the input manager's internal key up function
			inputManager.InternalKeyUp((uint64_t)wParam);

			return 0;
		}
		case WM_LBUTTONDOWN: {
			// Call the input manager's internal key down function
			inputManager.InternalKeyDown((uint64_t)VK_LBUTTON);

			return 0;
		}
		case WM_LBUTTONUP: {
			// Call the input manager's internal key up function
			inputManager.InternalKeyUp((uint64_t)VK_LBUTTON);

			return 0;
		}
		case WM_RBUTTONDOWN: {
			// Call the input manager's internal key down function
			inputManager.InternalKeyDown((uint64_t)VK_RBUTTON);

			return 0;
		}
		case WM_RBUTTONUP: {
			// Call the input manager's internal key up function
			inputManager.InternalKeyUp((uint64_t)VK_RBUTTON);

			return 0;
		}
		case WM_MBUTTONDOWN: {
			// Call the input manager's internal key down function
			inputManager.InternalKeyDown((uint64_t)VK_MBUTTON);

			return 0;
		}
		case WM_MBUTTONUP: {
			// Call the input manager's internal key up function
			inputManager.InternalKeyUp((uint64_t)VK_MBUTTON);

			return 0;
		}
		case WM_XBUTTONDOWN: {
			// Call the input manager's internal key down function for the current X button
			if(HIWORD(wParam) == XBUTTON1)
				inputManager.InternalKeyDown((uint64_t)VK_XBUTTON1);
			else
				inputManager.InternalKeyDown((uint64_t)VK_XBUTTON2);

			return TRUE;
		}
		case WM_XBUTTONUP: {
			// Call the input manager's internal key up function for the current X button
			if(HIWORD(wParam) == XBUTTON1)
				inputManager.InternalKeyUp((uint64_t)VK_XBUTTON1);
			else
				inputManager.InternalKeyUp((uint64_t)VK_XBUTTON2);

			return TRUE;
		}
		case WM_MOUSEMOVE: {
			// Set the mouse movement of the current event
			InputManager::MouseMovement mouseMovement;

			mouseMovement.x = (int32_t)(int16_t)LOWORD(lParam);
			mouseMovement.y = (int32_t)(int16_t)HIWORD(lParam);
			mouseMovement.scroll = 0;

			// Call the input manager's internal mouse move function
			inputManager.InternalMouseMove(mouseMovement);

			return 0;
		}
		case WM_MOUSEWHEEL: {
			// Set the mouse movement of the current event
			InputManager::MouseMovement mouseMovement;

			mouseMovement.x = 0;
			mouseMovement.y = 0;
			mouseMovement.scroll = (int32_t)(int16_t)HIWORD(wParam);

			// Call the input manager's internal mouse move function
			inputManager.InternalMouseMove(mouseMovement);

			return 0;
		}
		}

		return (uint64_t)DefWindowProcA(platformInfo.hWnd, msg, wParam, lParam);
	}

	// Public functions
	Window::Window(WindowInfo windowInfo) : windowInfo(windowInfo) {
		// Get the program's HINSTANCE
		platformInfo.hInstance = GetModuleHandleA(nullptr);

		// Generate the class' name
		char_t className[CLASS_NAME_LENGTH + 1];
		uint64_t classIndex = classCount++;

		for(uint32_t i = 0; i != CLASS_NAME_LENGTH; ++i) {
			// Set the next character in the class name and update the class index for the next char
			className[i] = BASE_64_CHARS[classIndex & 63];
			classIndex >>= 6;
		}

		// Add the null termination character
		className[CLASS_NAME_LENGTH] = 0;

		// Set the window's class info
		WNDCLASSEXA winClassInfo;

		winClassInfo.cbSize = sizeof(WNDCLASSEXA);
		winClassInfo.style = 0;
		winClassInfo.lpfnWndProc = WinProc;
		winClassInfo.cbClsExtra = 0;
		winClassInfo.cbWndExtra = 0;
		winClassInfo.hInstance = platformInfo.hInstance;
		winClassInfo.hIcon = nullptr;
		winClassInfo.hCursor = nullptr;
		winClassInfo.hbrBackground = nullptr;
		winClassInfo.lpszMenuName = nullptr;
		winClassInfo.lpszClassName = className;
		winClassInfo.hIconSm = nullptr;

		// Register the window's class
		platformInfo.winClassID = RegisterClassExA(&winClassInfo);

		if(!platformInfo.winClassID) {
			// Format the message
			char_t err[ERR_BUFFER_SIZE] = "Unknown.";
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, err, ERR_BUFFER_SIZE, nullptr);

			// Throw an exception
			throw Exception("Failed to register Win32 window class! Error: %s", err);
		}

		// Create the window
		platformInfo.hWnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, (LPCSTR)(size_t)platformInfo.winClassID, windowInfo.name.c_str(), WS_OVERLAPPEDWINDOW, windowInfo.posX, windowInfo.posY, (int32_t)windowInfo.width, (int32_t)windowInfo.height, nullptr, nullptr, platformInfo.hInstance, nullptr);

		if(!platformInfo.hWnd) {
			// Format the message
			char_t err[ERR_BUFFER_SIZE] = "Unknown.";
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, err, ERR_BUFFER_SIZE, nullptr);

			// Throw an exception
			throw Exception("Failed to create Win32 window! Error: %s", err);
		}

		// Add the window to the map
		windowsMutex.Lock();
		windows.insert({ platformInfo.hWnd, { this, ProcessInternalEvent } });
		windowsMutex.Unlock();

		// Show the window based on the given params
		if(windowInfo.isMaximized)
			ShowWindow(platformInfo.hWnd, SW_MAXIMIZE);
		else
			ShowWindow(platformInfo.hWnd, SW_NORMAL);
		if(windowInfo.isMinimized)
			ShowWindow(platformInfo.hWnd, SW_MINIMIZE);
	}

	void Window::PollEvents() {
		// Begin the input manager's event management
		inputManager.BeginInputEvents();

		// Handle the window's events
		MSG msg;
		while(PeekMessageA(&msg, platformInfo.hWnd, 0, 0, PM_REMOVE)) {
			// Translate the message and dispatch it
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		// Handle the thread's non-window events
		while(PeekMessageA(&msg, (HWND)-1, 0, 0, PM_REMOVE)) {
			// Translate the message and dispatch it
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		// Begin the input manager's event management
		inputManager.EndInputEvents();
	}

	void Window::SetWindowName(const string& newName) {
		// Set the window's new name
		if(!SetWindowTextA(platformInfo.hWnd, newName.c_str())) {
			// Format the message
			char_t err[ERR_BUFFER_SIZE] = "Unknown.";
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, err, ERR_BUFFER_SIZE, nullptr);

			// Throw an exception
			throw Exception("Failed to set window name! Error: %s", err);
		}
	}
	void Window::SetWindowPos(int32_t newX, int32_t newY) {
		// Set the window's position
		if(!::SetWindowPos(platformInfo.hWnd, nullptr, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER)) {
			// Format the message
			char_t err[ERR_BUFFER_SIZE] = "Unknown.";
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, err, ERR_BUFFER_SIZE, nullptr);

			// Throw an exception
			throw Exception("Failed to set window position! Error: %s", err);
		}
	}
	void Window::SetWindowSize(uint32_t newWidth, uint32_t newHeight, bool8_t maximized, bool8_t minimized) {
		// Set the window's size
		if(!::SetWindowPos(platformInfo.hWnd, nullptr, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER)) {
			// Format the message
			char_t err[ERR_BUFFER_SIZE] = "Unknown.";
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, err, ERR_BUFFER_SIZE, nullptr);

			// Throw an exception
			throw Exception("Failed to set window size! Error: %s", err);
		}

		// Show the window based on the given params
		if(maximized)
			ShowWindow(platformInfo.hWnd, SW_MAXIMIZE);
		else
			ShowWindow(platformInfo.hWnd, SW_NORMAL);
		if(minimized)
			ShowWindow(platformInfo.hWnd, SW_MINIMIZE);
	}

	Window::~Window() {
		// Remove the window from the map
		windowsMutex.Lock();
		windows.erase(platformInfo.hWnd);
		windowsMutex.Unlock();

		// Destroy the window
		DestroyWindow(platformInfo.hWnd);

		// Unregister the window class
		UnregisterClassA((LPCSTR)(size_t)platformInfo.winClassID, platformInfo.hInstance);
	}
}

#endif