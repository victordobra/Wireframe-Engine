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
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_UK), errBuffer, ERR_BUFFER_SIZE, nullptr);

		// Throw an error
		throw std::runtime_error(message + " Error: " + errBuffer);
	}

	LRESULT CALLBACK Window::GlobalProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Get the window object corresponding to the current hWnd
		uint32_t lock = 0;
		while(!windowMutex.compare_exchange_weak(lock, 1))
			lock = 0;
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
		case WM_MOVE: {
			// Update the window's position
			this->x = LOWORD(lParam);
			this->y = HIWORD(lParam);

			// Set the move event data
			MoveEventData data {
				.x = this->x,
				.y = this->y
			};

			// Call the move event
			this->moveEvent.TriggerEvent(&data);

			break;
		}
		case WM_SIZE: {
			// Update the window's size and minimized/maximized state
			this->width = LOWORD(lParam);
			this->height = HIWORD(lParam);
			this->minimized = (wParam == SIZE_MINIMIZED);
			this->maximized = (wParam == SIZE_MAXIMIZED);

			// Set the resize event data
			ResizeEventData data {
				.width = this->width,
				.height = this->height,
				.minimized = this->minimized,
				.maximized = this->maximized
			};

			// Call the resize event
			this->resizeEvent.TriggerEvent(&data);

			break;
		}
		case WM_SETTEXT: {
			// Update the window's title
			this->title = (const char_t*)lParam;

			// Set the rename event data
			RenameEventData data {
				.title = this->title
			};

			// Call the rename event
			this->renameEvent.TriggerEvent(&data);

			break;
		}
		case WM_CLOSE:
			// Call the close event
			this->closeEvent.TriggerEvent(nullptr);

			return 0;
		};

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	// Public functions
	Window::Window(int32_t x, int32_t y, uint32_t width, uint32_t height, const std::string& title) : x(x), y(y), width(width), height(height), title(title) {
		// Lock the window mutex
		uint32_t lock = 0;
		while(!windowMutex.compare_exchange_weak(lock, 1))
			lock = 0;

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

		// Unlock the window mutex
		windowMutex = 0;

		// Assign all internal infos to the platform data struct
		this->platformData.hInstance = hInstance;
		this->platformData.winClassID = winClassID;

		// Create the window
		this->platformData.hWnd = CreateWindowExA(WS_EX_APPWINDOW, (LPCSTR)(size_t)winClassID, this->title.c_str(), WS_OVERLAPPEDWINDOW, this->x, this->y, this->width, this->height, nullptr, nullptr, hInstance, this);
		if(!this->platformData.hWnd)
			ThrowError("Failed to create Win32 window!");

		// Add the window to the map
		while(!windowMutex.compare_exchange_weak(lock, 1))
			lock = 0;
		windowMap.insert({ this->platformData.hWnd, this });
		windowMutex = 0;

		// Show the window
		ShowWindow(this->platformData.hWnd, SW_SHOW);
	}

	void Window::SetPos(int32_t x, int32_t y) {
		// Set the window's position
		if(!SetWindowPos(this->platformData.hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE)) {
			// Throw an error
			ThrowError("Failed to set Win32 window position!");
		}
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
		// Remove the window from the map
		uint32_t lock = 0;
		while(!windowMutex.compare_exchange_weak(lock, 1))
			lock = 0;
		windowMap.erase(this->platformData.hWnd);
		windowMutex = 0;

		// Destroy the window
		DestroyWindow(this->platformData.hWnd);

		// Lock the window mutex
		while(!windowMutex.compare_exchange_weak(lock, 1))
			lock = 0;

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
