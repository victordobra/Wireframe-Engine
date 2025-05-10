#include "Core/Types/BuildInfo.hpp"

#if defined(WFE_PLATFORM_WINDOWS)

#include "Platform/Window.hpp"

#include <unordered_map>

namespace wfe {
	// Constants
	static constexpr size_t ERR_BUFFER_SIZE = 256;
	static constexpr const char* CLASS_NAME = "WFEWindowClass";

	// Internal static variables
	static std::unordered_map<HWND, Window*> windowMap;
	static atomic_uint32_t windowMutex;

	static HINSTANCE hInstance;
	static ATOM winClassID;

	// Internal functions
	static void ThrowError(const std::string& message) {
		// Format the error message
		char errBuffer[ERR_BUFFER_SIZE] = "Unknown error.";
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

			// Trigger the move event
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

			// Trigger the resize event
			this->resizeEvent.TriggerEvent(&data);

			break;
		}
		case WM_SETTEXT: {
			// Update the window's title
			this->title = (const char*)lParam;

			// Set the rename event data
			RenameEventData data {
				.title = this->title
			};

			// Trigger the rename event
			this->renameEvent.TriggerEvent(&data);

			break;
		}
		case WM_KEYDOWN: 
		case WM_SYSKEYDOWN: {
			// Call the input manager's internal key down function
			this->inputManager->InternalKeyDown((uint64_t)wParam);

			return 0;
		}
		case WM_KEYUP: 
		case WM_SYSKEYUP: {
			// Call the input manager's internal key up function
			this->inputManager->InternalKeyUp((uint64_t)wParam);

			return 0;
		}
		case WM_LBUTTONDOWN: {
			// Call the input manager's internal key down function
			this->inputManager->InternalKeyDown((uint64_t)VK_LBUTTON);

			return 0;
		}
		case WM_LBUTTONUP: {
			// Call the input manager's internal key up function
			this->inputManager->InternalKeyUp((uint64_t)VK_LBUTTON);

			return 0;
		}
		case WM_RBUTTONDOWN: {
			// Call the input manager's internal key down function
			this->inputManager->InternalKeyDown((uint64_t)VK_RBUTTON);

			return 0;
		}
		case WM_RBUTTONUP: {
			// Call the input manager's internal key up function
			this->inputManager->InternalKeyUp((uint64_t)VK_RBUTTON);

			return 0;
		}
		case WM_MBUTTONDOWN: {
			// Call the input manager's internal key down function
			this->inputManager->InternalKeyDown((uint64_t)VK_MBUTTON);

			return 0;
		}
		case WM_MBUTTONUP: {
			// Call the input manager's internal key up function
			this->inputManager->InternalKeyUp((uint64_t)VK_MBUTTON);

			return 0;
		}
		case WM_XBUTTONDOWN: {
			// Call the input manager's internal key down function for the current X button
			if(HIWORD(wParam) == XBUTTON1) {
				this->inputManager->InternalKeyDown((uint64_t)VK_XBUTTON1);
			} else {
				this->inputManager->InternalKeyDown((uint64_t)VK_XBUTTON2);
			}

			return TRUE;
		}
		case WM_XBUTTONUP: {
			// Call the input manager's internal key up function for the current X button
			if(HIWORD(wParam) == XBUTTON1) {
				this->inputManager->InternalKeyUp((uint64_t)VK_XBUTTON1);
			} else {
				this->inputManager->InternalKeyUp((uint64_t)VK_XBUTTON2);
			}

			return TRUE;
		}
		case WM_MOUSEMOVE: {
			// Set the mouse movement of the current event
			InputManager::MouseMovement mouseMovement {
				.x = (int32_t)(int16_t)LOWORD(lParam),
				.y = (int32_t)(int16_t)HIWORD(lParam),
				.scroll = 0
			};

			// Call the input manager's internal mouse move function
			this->inputManager->InternalMouseMove(mouseMovement);

			return 0;
		}
		case WM_MOUSEWHEEL: {
			// Set the mouse movement of the current event
			InputManager::MouseMovement mouseMovement {
				.x = 0,
				.y = 0,
				.scroll = (int32_t)(int16_t)HIWORD(wParam)
			};

			// Call the input manager's internal mouse move function
			this->inputManager->InternalMouseMove(mouseMovement);

			return 0;
		}
		case WM_CLOSE:
			// Trigger the close event
			this->closeEvent.TriggerEvent(nullptr);

			return 0;
		};

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	// Public functions
	Window::Window(int32_t x, int32_t y, uint32_t width, uint32_t height, const std::string& title, bool minimized, bool maximized, bool fullscreen) : x(x), y(y), width(width), height(height), title(title), minimized(minimized), maximized(maximized), fullscreen(fullscreen) {
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

		// Set the window's style based on the fullscreen flag
		DWORD style;
		if(this->fullscreen) {
			// Unmaximize the window
			maximized = false;
			style = WS_POPUP;
		} else {
			style = WS_OVERLAPPEDWINDOW;
		}

		// Override the window's position and size if the window is fullscreen
		if(this->fullscreen) {
			// Get the screen's size
			RECT screenRect;
			if(!SystemParametersInfoA(SPI_GETWORKAREA, 0, &screenRect, 0)) {
				// Throw an error
				ThrowError("Failed to get Win32 screen size!");
			}

			// Set the window's position and size to the screen's size
			this->x = screenRect.left;
			this->y = screenRect.top;
			this->width = screenRect.right - screenRect.left;
			this->height = screenRect.bottom - screenRect.top;
		}

		// Create the window
		this->platformData.hWnd = CreateWindowExA(WS_EX_APPWINDOW, (LPCSTR)(size_t)winClassID, this->title.c_str(), style, this->x, this->y, this->width, this->height, nullptr, nullptr, hInstance, this);
		if(!this->platformData.hWnd)
			ThrowError("Failed to create Win32 window!");

		// Add the window to the map
		while(!windowMutex.compare_exchange_weak(lock, 1))
			lock = 0;
		windowMap.insert({ this->platformData.hWnd, this });
		windowMutex = 0;

		// Create the input manager
		this->inputManager = new InputManager(this);

		// Show the window
		if(this->minimized) {
			// Show the window as minimized
			ShowWindow(this->platformData.hWnd, SW_MINIMIZE);
		} else if(this->maximized) {
			// Show the window as maximized
			ShowWindow(this->platformData.hWnd, SW_MAXIMIZE);
		} else {
			// Show the window as normal
			ShowWindow(this->platformData.hWnd, SW_SHOWNORMAL);
		}
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
	void Window::SetMinimized(bool minimized) {
		// Show the window as minimized or restored
		ShowWindow(this->platformData.hWnd, minimized ? SW_MINIMIZE : SW_RESTORE);
	}
	void Window::SetMaximized(bool maximized) {
		// Show the window as maximized or restored
		ShowWindow(this->platformData.hWnd, maximized ? SW_MAXIMIZE : SW_RESTORE);
	}
	void Window::SetFullscreen(bool fullscreen) {
		// Apply or unapply the fullscreen style, if required
		if(fullscreen && !this->fullscreen) {
			// Set the new fullscreen state and reset the maximized and minimized states
			this->fullscreen = fullscreen;

			// Get the screen's size
			RECT screenRect;
			if(!SystemParametersInfoA(SPI_GETWORKAREA, 0, &screenRect, 0)) {
				// Throw an error
				ThrowError("Failed to get Win32 screen size!");
			}

			// Set the window's new position and size
			this->x = screenRect.left;
			this->y = screenRect.top;
			this->width = screenRect.right - screenRect.left;
			this->height = screenRect.bottom - screenRect.top;

			if(!SetWindowPos(this->platformData.hWnd, nullptr, this->x, this->y, this->width, this->height, SWP_NOZORDER | SWP_NOACTIVATE)) {
				// Throw an error
				ThrowError("Failed to set Win32 window info!");
			}

			// Set the window's style
			SetWindowLongA(this->platformData.hWnd, GWL_STYLE, WS_POPUP);

			// Show the window as normal
			ShowWindow(this->platformData.hWnd, SW_SHOWNORMAL);
		} else if(!fullscreen && this->fullscreen) {
			// Set the new fullscreen state
			this->fullscreen = fullscreen;

			// Set the window's style
			SetWindowLongA(this->platformData.hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			// Show the window as maximized
			ShowWindow(this->platformData.hWnd, SW_MAXIMIZE);
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
		// Destroy the input manager
		delete this->inputManager;

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
