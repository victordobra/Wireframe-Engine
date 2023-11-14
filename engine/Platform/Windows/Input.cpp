#include <BuildInfo.hpp>

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform/Input.hpp"
#include "Internal.hpp"

namespace wfe {
	// Variables
	static Event keyDownEvent{};
	static Event keyUpEvent{};
	static Event mouseMoveEvent{};

	static MousePosition mousePos;

	// Internal functions
	LRESULT CALLBACK InputWinProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		switch(message) {
		case WM_LBUTTONDOWN: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = KEY_LEFT_MBUTTON;

			// Call the key down event
			keyDownEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_LBUTTONUP: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = KEY_LEFT_MBUTTON;

			// Call the key up event
			keyUpEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_RBUTTONDOWN: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = KEY_RIGHT_MBUTTON;

			// Call the key down event
			keyDownEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_RBUTTONUP: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = KEY_RIGHT_MBUTTON;

			// Call the key up event
			keyUpEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_MBUTTONDOWN: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = KEY_MIDDLE_MBUTTON;

			// Call the key down event
			keyDownEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_MBUTTONUP: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = KEY_MIDDLE_MBUTTON;

			// Call the key up event
			keyUpEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_XBUTTONDOWN: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = (HIWORD(wParam) == XBUTTON1) ? KEY_X1_MBUTTON : KEY_X2_MBUTTON;

			// Call the key down event
			keyDownEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_XBUTTONUP: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = (HIWORD(wParam) == XBUTTON1) ? KEY_X1_MBUTTON : KEY_X2_MBUTTON;

			// Call the key up event
			keyUpEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = VKToKeyCode(wParam);

			// Exit the current case if an invalid key was pressed
			if(eventInfo.keyCode == KEY_NONE)
				break;
			
			// Call the key down event
			keyDownEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			// Set the key event info
			KeyEventInfo eventInfo;
			eventInfo.keyCode = VKToKeyCode(wParam);

			// Exit the current case if an invalid key was pressed
			if(eventInfo.keyCode == KEY_NONE)
				break;
			
			// Call the key up event
			keyUpEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_MOUSEWHEEL: {
			// Set the mouse move event info
			MouseMoveEventInfo eventInfo;
			eventInfo.newPos = mousePos;
			eventInfo.scrollDelta = ((int32_t)(int16_t)HIWORD(wParam)) / WHEEL_DELTA;

			// Call the mouse move event
			mouseMoveEvent.CallEvent(&eventInfo);

			break;
		}
		case WM_MOUSEMOVE: {
			// Set the mouse's new position
			mousePos.x = (int32_t)(int16_t)LOWORD(lParam);
			mousePos.y = (int32_t)(int16_t)HIWORD(lParam);

			// Set the mouse move event info
			MouseMoveEventInfo eventInfo;
			eventInfo.newPos = mousePos;
			eventInfo.scrollDelta = 0;

			// Call the mouse move event
			mouseMoveEvent.CallEvent(&eventInfo);

			break;
		}
		}

		return DefWindowProcA(hWnd, message, wParam, lParam);
	}

	// Public functions
	Event& GetKeyDownEvent() {
		return keyDownEvent;
	}
	Event& GetKeyUpEvent() {
		return keyUpEvent;
	}
	Event& GetMouseMoveEvent() {
		return mouseMoveEvent;
	}

	MousePosition GetMousePosition() {
		return mousePos;
	}
}

#endif