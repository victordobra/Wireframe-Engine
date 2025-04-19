#include "Core/Types/BuildInfo.hpp"

#if defined(WFE_PLATFORM_WINDOWS)

#include "Platform/Window.hpp"

#include <unordered_map>

namespace wfe {
	// Constants
	static constexpr size_t ERR_BUFFER_SIZE = 256;
	static constexpr const char_t* CLASS_NAME = "WFEWindowClass";

	// Internal static variables
	static std::unordered_map<HWND, Window*> windowMap;
	static atomic_uint32_t windowMutex;

	static HINSTANCE hInstance;
	static ATOM winClassID;

	// Internal functions
	static void ThrowError(const std::string& message) {
		// Format the error message
		char_t errBuffer[ERR_BUFFER_SIZE] = "Unknown error.";
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errBuffer, ERR_BUFFER_SIZE, nullptr);

		// Throw an error
		throw std::runtime_error(message + " Error: " + errBuffer);
	}

	LRESULT CALLBACK Window::GlobalProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Get the window object corresponding to the current hWnd
		uint32_t lock = 0;
		while(!windowMutex.compare_exchange_weak(lock, 1));
		auto iter = windowMap.find(hWnd);
		windowMutex = 0;

		if(iter != windowMap.end()) {
			// Call the window's procedure
			return iter->second->WindowProc(hWnd, msg, wParam, lParam);
		} else {
			// Call the default procedure
			return DefWindowProcA(hWnd, msg, wParam, lParam);
		}
	}
	LRESULT Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg) {
		case WM_SETTEXT:
			// Update the window's title
			this->title = (const char_t*)lParam;

			break;
		case WM_SIZE:
			// Update the window's size
			this->width = LOWORD(lParam);
			this->height = HIWORD(lParam);

			break;
		case WM_CLOSE:
			// Stop running the window
			this->running = false;

			break;
		};

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	// Public functions
	Window::Window(uint32_t width, uint32_t height, const std::string& title) {
		// Set the window's width, height and title
		this->width = width;
		this->height = height;
		this->title = title;

		// Lock the window mutex
		uint32_t lock = 0;
		while(!windowMutex.compare_exchange_weak(lock, 1));

		// Check if the window class has been registered
		if(!winClassID) {
			// Set the instance handle
			hInstance = GetModuleHandle(nullptr);

			// Set the window class's info
			WNDCLASSEXA winClassInfo {
				.cbSize = sizeof(WNDCLASSEXA),
				.style = CS_HREDRAW | CS_VREDRAW,
				.lpfnWndProc = GlobalProc,
				.cbClsExtra = 0,
				.cbWndExtra = 0,
				.hInstance = hInstance,
				.hIcon = LoadIconA(nullptr, IDI_APPLICATION),
				.hCursor = LoadCursorA(nullptr, IDC_ARROW),
				.hbrBackground = 0,
				.lpszMenuName = nullptr,
				.lpszClassName = CLASS_NAME,
				.hIconSm = LoadIconA(nullptr, IDI_APPLICATION)
			};
			
			// Register the window's class
			winClassID = RegisterClassExA(&winClassInfo);
			if(!winClassID) {
				// Unlock the window mutex and throw an error
				windowMutex = 0;
				ThrowError("Failed to register Win32 window class!");
			}
		}

		// Assign all internal infos to the platform data struct
		this->platformData.hInstance = hInstance;
		this->platformData.winClassID = winClassID;

		// Create the window
		this->platformData.hWnd = CreateWindowExA(WS_EX_APPWINDOW, (LPCSTR)(size_t)winClassID, this->title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, nullptr, nullptr, hInstance, this);
		if(!this->platformData.hWnd) {
			// Unlock the window mutex and throw an error
			windowMutex = 0;
			ThrowError("Failed to create Win32 window!");
		}

		// Add the window to the map
		windowMap.insert({ this->platformData.hWnd, this });

		// Unlock the window mutex
		windowMutex = 0;

		// Show the window
		ShowWindow(this->platformData.hWnd, SW_SHOW);
	}

	void Window::SetSize(uint32_t width, uint32_t height) {
		// Set the window's size
		if(!SetWindowPos(this->platformData.hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE)) {
			// Throw an error
			ThrowError("Failed to set Win32 window size!");
		}
	}
	void Window::SetTitle(const std::string& title) {
		// Set the window's title
		if(!SetWindowTextA(this->platformData.hWnd, title.c_str())) {
			// Throw an error
			ThrowError("Failed to set Win32 window title!");
		}
	}

	void Window::PollEvents() {
		// Handle the window's events
		MSG msg;
		while(PeekMessageA(&msg, this->platformData.hWnd, 0, 0, PM_REMOVE)) {
			// Translate and dispatch the message
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		// Handle the thread's non-window events
		while(PeekMessageA(&msg, (HWND)-1, 0, 0, PM_REMOVE)) {
			// Translate and dispatch the message
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	Window::~Window() {
		// Lock the window mutex
		uint32_t lock = 0;
		while(!windowMutex.compare_exchange_weak(lock, 1));

		// Remove the window from the map
		windowMap.erase(this->platformData.hWnd);

		// Destroy the window
		DestroyWindow(this->platformData.hWnd);

		// Unregister the window class if there are no more windows
		if(windowMap.empty()) {
			UnregisterClassA((LPCSTR)(size_t)this->platformData.winClassID, this->platformData.hInstance);
			winClassID = 0;
		}

		// Unlock the window mutex
		windowMutex = 0;
	}
}

#endif
