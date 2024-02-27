#include <BuildInfo.hpp>

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform/Input.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	// Key code converter
	static InputManager::Key ConvertKeyCode(uint64_t keyCode) {
		switch(keyCode) {
		case VK_LBUTTON:
			return InputManager::KEY_LEFT_MBUTTON;
		case VK_RBUTTON:
			return InputManager::KEY_RIGHT_MBUTTON;
		case VK_MBUTTON:
			return InputManager::KEY_MIDDLE_MBUTTON;
		case VK_XBUTTON1:
			return InputManager::KEY_X1_MBUTTON;
		case VK_XBUTTON2:
			return InputManager::KEY_X2_MBUTTON;
		case VK_BACK:
			return InputManager::KEY_BACKSPACE;
		case VK_TAB:
			return InputManager::KEY_TAB;
		case VK_CLEAR:
			return InputManager::KEY_CLEAR;
		case VK_RETURN:
			return InputManager::KEY_ENTER;
		case VK_SHIFT:
			return InputManager::KEY_SHIFT;
		case VK_CONTROL:
			return InputManager::KEY_CONTROL;
		case VK_MENU:
			return InputManager::KEY_ALT;
		case VK_PAUSE:
			return InputManager::KEY_PAUSE;
		case VK_CAPITAL:
			return InputManager::KEY_CAPS_LOCK;
		case VK_ESCAPE:
			return InputManager::KEY_ESCAPE;
		case VK_SPACE:
			return InputManager::KEY_SPACE;
		case VK_INSERT:
			return InputManager::KEY_INSERT;
		case VK_DELETE:
			return InputManager::KEY_DELETE;
		case VK_HOME:
			return InputManager::KEY_HOME;
		case VK_END:
			return InputManager::KEY_END;
		case VK_PRIOR:
			return InputManager::KEY_PAGE_UP;
		case VK_NEXT:
			return InputManager::KEY_PAGE_DOWN;
		case VK_LEFT:
			return InputManager::KEY_LEFT_ARROW;
		case VK_UP:
			return InputManager::KEY_UP_ARROW;
		case VK_DOWN:
			return InputManager::KEY_DOWN_ARROW;
		case VK_SNAPSHOT:
			return InputManager::KEY_PRINT_SCREEN;
		case 0x30:
			return InputManager::KEY_0;
		case 0x31:
			return InputManager::KEY_1;
		case 0x32:
			return InputManager::KEY_2;
		case 0x33:
			return InputManager::KEY_3;
		case 0x34:
			return InputManager::KEY_4;
		case 0x35:
			return InputManager::KEY_5;
		case 0x36:
			return InputManager::KEY_6;
		case 0x37:
			return InputManager::KEY_7;
		case 0x38:
			return InputManager::KEY_8;
		case 0x39:
			return InputManager::KEY_9;
		case 0x41:
			return InputManager::KEY_A;
		case 0x42:
			return InputManager::KEY_B;
		case 0x43:
			return InputManager::KEY_C;
		case 0x44:
			return InputManager::KEY_D;
		case 0x45:
			return InputManager::KEY_E;
		case 0x46:
			return InputManager::KEY_F;
		case 0x47:
			return InputManager::KEY_G;
		case 0x48:
			return InputManager::KEY_H;
		case 0x49:
			return InputManager::KEY_I;
		case 0x4a:
			return InputManager::KEY_J;
		case 0x4b:
			return InputManager::KEY_K;
		case 0x4c:
			return InputManager::KEY_L;
		case 0x4d:
			return InputManager::KEY_M;
		case 0x4e:
			return InputManager::KEY_N;
		case 0x4f:
			return InputManager::KEY_O;
		case 0x50:
			return InputManager::KEY_P;
		case 0x51:
			return InputManager::KEY_Q;
		case 0x52:
			return InputManager::KEY_R;
		case 0x53:
			return InputManager::KEY_S;
		case 0x54:
			return InputManager::KEY_T;
		case 0x55:
			return InputManager::KEY_U;
		case 0x56:
			return InputManager::KEY_V;
		case 0x57:
			return InputManager::KEY_W;
		case 0x58:
			return InputManager::KEY_X;
		case 0x59:
			return InputManager::KEY_Y;
		case 0x5A:
			return InputManager::KEY_Z;
		case VK_LWIN:
			return InputManager::KEY_LWIN;
		case VK_RWIN:
			return InputManager::KEY_RWIN;
		case VK_NUMPAD0:
			return InputManager::KEY_NUMPAD0;
		case VK_NUMPAD1:
			return InputManager::KEY_NUMPAD1;
		case VK_NUMPAD2:
			return InputManager::KEY_NUMPAD2;
		case VK_NUMPAD3:
			return InputManager::KEY_NUMPAD3;
		case VK_NUMPAD4:
			return InputManager::KEY_NUMPAD4;
		case VK_NUMPAD5:
			return InputManager::KEY_NUMPAD5;
		case VK_NUMPAD6:
			return InputManager::KEY_NUMPAD6;
		case VK_NUMPAD7:
			return InputManager::KEY_NUMPAD7;
		case VK_NUMPAD8:
			return InputManager::KEY_NUMPAD8;
		case VK_NUMPAD9:
			return InputManager::KEY_NUMPAD9;
		case VK_ADD:
			return InputManager::KEY_NUMPAD_ADD;
		case VK_SUBTRACT:
			return InputManager::KEY_NUMPAD_SUBTRACT;
		case VK_MULTIPLY:
			return InputManager::KEY_NUMPAD_MULTIPLY;
		case VK_DIVIDE:
			return InputManager::KEY_NUMPAD_DIVIDE;
		case VK_SEPARATOR:
			return InputManager::KEY_NUMPAD_SEPARATOR;
		case VK_DECIMAL:
			return InputManager::KEY_NUMPAD_DECIMAL;
		case VK_F1:
			return InputManager::KEY_F1;
		case VK_F2:
			return InputManager::KEY_F2;
		case VK_F3:
			return InputManager::KEY_F3;
		case VK_F4:
			return InputManager::KEY_F4;
		case VK_F5:
			return InputManager::KEY_F5;
		case VK_F6:
			return InputManager::KEY_F6;
		case VK_F7:
			return InputManager::KEY_F7;
		case VK_F8:
			return InputManager::KEY_F8;
		case VK_F9:
			return InputManager::KEY_F9;
		case VK_F10:
			return InputManager::KEY_F10;
		case VK_F11:
			return InputManager::KEY_F11;
		case VK_F12:
			return InputManager::KEY_F12;
		case VK_F13:
			return InputManager::KEY_F13;
		case VK_F14:
			return InputManager::KEY_F14;
		case VK_F15:
			return InputManager::KEY_F15;
		case VK_F16:
			return InputManager::KEY_F16;
		case VK_F17:
			return InputManager::KEY_F17;
		case VK_F18:
			return InputManager::KEY_F18;
		case VK_F19:
			return InputManager::KEY_F19;
		case VK_F20:
			return InputManager::KEY_F20;
		case VK_F21:
			return InputManager::KEY_F21;
		case VK_F22:
			return InputManager::KEY_F22;
		case VK_F23:
			return InputManager::KEY_F23;
		case VK_F24:
			return InputManager::KEY_F24;
		case VK_NUMLOCK:
			return InputManager::KEY_NUM_LOCK;
		case VK_SCROLL:
			return InputManager::KEY_SCROLL_LOCK;
		case VK_LSHIFT:
			return InputManager::KEY_LSHIFT;
		case VK_RSHIFT:
			return InputManager::KEY_RSHIFT;
		case VK_LCONTROL:
			return InputManager::KEY_LCONTROL;
		case VK_RCONTROL:
			return InputManager::KEY_RCONTROL;
		case VK_LMENU:
			return InputManager::KEY_LALT;
		case VK_RMENU:
			return InputManager::KEY_RALT;
		case VK_BROWSER_BACK:
			return InputManager::KEY_BROWSER_BACK;
		case VK_BROWSER_FORWARD:
			return InputManager::KEY_BROWSER_FORWARD;
		case VK_BROWSER_REFRESH:
			return InputManager::KEY_BROWSER_REFRESH;
		case VK_BROWSER_STOP:
			return InputManager::KEY_BROWSER_STOP;
		case VK_BROWSER_FAVORITES:
			return InputManager::KEY_BROWSER_FAVORITES;
		case VK_BROWSER_HOME:
			return InputManager::KEY_BROWSER_HOME;
		case VK_VOLUME_MUTE:
			return InputManager::KEY_VOLUME_MUTE;
		case VK_VOLUME_DOWN:
			return InputManager::KEY_VOLUME_DOWN;
		case VK_VOLUME_UP:
			return InputManager::KEY_VOLUME_UP;
		case VK_MEDIA_NEXT_TRACK:
			return InputManager::KEY_MEDIA_NEXT;
		case VK_MEDIA_PREV_TRACK:
			return InputManager::KEY_MEDIA_PREV;
		case VK_MEDIA_STOP:
			return InputManager::KEY_MEDIA_STOP;
		case VK_MEDIA_PLAY_PAUSE:
			return InputManager::KEY_MEDIA_PLAY_PAUSE;
		case VK_OEM_1:
			return InputManager::KEY_SEMICOLON;
		case VK_OEM_PLUS:
			return InputManager::KEY_EQUAL;
		case VK_OEM_MINUS:
			return InputManager::KEY_MINUS;
		case VK_OEM_COMMA:
			return InputManager::KEY_COMMA;
		case VK_OEM_PERIOD:
			return InputManager::KEY_PERIOD;
		case VK_OEM_2:
			return InputManager::KEY_SLASH;
		case VK_OEM_3:
			return InputManager::KEY_BACKTICK;
		case VK_OEM_4:
			return InputManager::KEY_OPEN_SQR_PARENTHESIS;
		case VK_OEM_6:
			return InputManager::KEY_CLOSED_SQR_PARENTHESIS;
		case VK_OEM_5:
			return InputManager::KEY_BACKSLASH;
		case VK_OEM_7:
			return InputManager::KEY_QUOTES;
		default:
			return InputManager::KEY_NONE;
		}
	}

	// Internal functions
	void InputManager::BeginInputEvents() {
		// Reset the pressed and released flags for every key
		for(uint32_t i = 0; i != 128; ++i)
			keyStates[i] &= 0b10011001;
		
		// Reset the mouse movement
		mouseMovement.x = 0;
		mouseMovement.y = 0;
		mouseMovement.scroll = 0.f;
	}
	void InputManager::InternalKeyDown(uint64_t keyCode) {
		// Convert the virtual key code to a key
		Key key = ConvertKeyCode(keyCode);

		// Get the key state value and bit indices
		size_t valueIndex = (size_t)key >> 1;
		size_t bitIndex = ((size_t)key & 1) << 2;

		// Set the key down and key pressed flags
		keyStates[valueIndex] |= 0b0010 << bitIndex;

		// Set the key event info
		KeyEventInfo eventInfo;

		eventInfo.key = key;

		// Call the key down event
		keyDownEvent.CallEvent(&eventInfo);
	}
	void InputManager::InternalKeyUp(uint64_t keyCode) {
		// Convert the virtual key code to a key
		Key key = ConvertKeyCode(keyCode);

		// Get the key state value and bit indices
		size_t valueIndex = (size_t)key >> 1;
		size_t bitIndex = ((size_t)key & 1) << 2;

		// Set the key released and reset the key down flags in one XOR operation
		keyStates[valueIndex] ^= 0b0101 << bitIndex;

		// Set the key event info
		KeyEventInfo eventInfo;

		eventInfo.key = key;

		// Call the key up event
		keyUpEvent.CallEvent(&eventInfo);
	}
	void InputManager::InternalMouseMove(MouseMovement movement) {
		// Add the movements to the manager's mouse movement
		mouseMovement.x += movement.x;
		mouseMovement.y += movement.y;
		mouseMovement.scroll += movement.scroll;
	}
	void InputManager::EndInputEvents() {
		// Check if any mouse movement took place
		if(mouseMovement.x || mouseMovement.y || mouseMovement.scroll) {
			// Set the mouse's new position
			mousePos.x += mouseMovement.x;
			mousePos.y += mouseMovement.y;

			// Set the mosue move event info
			MouseMoveEventInfo mouseMoveInfo;

			mouseMoveInfo.newPos = mousePos;
			mouseMoveInfo.scrollDelta = mouseMovement.scroll;
		}
	}
}

#endif