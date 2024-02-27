#pragma once

#include <Core.hpp>

namespace wfe {
	class Window;

	/// @brief An input manager that polls all keyboard and mouse events.
	class InputManager {
	public:
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		InputManager& operator=(const InputManager&) = delete;
		InputManager& operator=(InputManager&&) = delete;

		/// @brief An enum containing all common keyboard keys.
		typedef enum Key : uint8_t {
			/// @brief Invalid key code.
			KEY_NONE,
			/// @brief Left mouse button key code.
			KEY_LEFT_MBUTTON,
			/// @brief Right mouse button key code.
			KEY_RIGHT_MBUTTON,
			/// @brief Middle mouse button key code.
			KEY_MIDDLE_MBUTTON,
			/// @brief X1 mouse button key code.
			KEY_X1_MBUTTON,
			/// @brief X2 mouse button key code.
			KEY_X2_MBUTTON,
			/// @brief Backspace key code.
			KEY_BACKSPACE,
			/// @brief Tab key code.
			KEY_TAB,
			/// @brief Clear key code.
			KEY_CLEAR,
			/// @brief Enter key code.
			KEY_ENTER,
			/// @brief Shift key code.
			KEY_SHIFT,
			/// @brief Control key code.
			KEY_CONTROL,
			/// @brief Alt key code.
			KEY_ALT,
			/// @brief Pause key code.
			KEY_PAUSE,
			/// @brief Caps lock key code.
			KEY_CAPS_LOCK,
			/// @brief Escape key code.
			KEY_ESCAPE,
			/// @brief Spacebar key code.
			KEY_SPACE,
			/// @brief Insert key code.
			KEY_INSERT,
			/// @brief Delete key code.
			KEY_DELETE,
			/// @brief Home key code.
			KEY_HOME,
			/// @brief End key code.
			KEY_END,
			/// @brief Page up key code.
			KEY_PAGE_UP,
			/// @brief Page down key code.
			KEY_PAGE_DOWN,
			/// @brief Left arrow key code.
			KEY_LEFT_ARROW,
			/// @brief Up arrow key code.
			KEY_UP_ARROW,
			/// @brief Right arrow key code.
			KEY_RIGHT_ARROW,
			/// @brief Down arrow key code.
			KEY_DOWN_ARROW,
			/// @brief Print screen key code.
			KEY_PRINT_SCREEN,
			/// @brief 0 key code.
			KEY_0,
			/// @brief 1 key code.
			KEY_1,
			/// @brief 2 key code.
			KEY_2,
			/// @brief 3 key code.
			KEY_3,
			/// @brief 4 key code.
			KEY_4,
			/// @brief 5 key code.
			KEY_5,
			/// @brief 6 key code.
			KEY_6,
			/// @brief 7 key code.
			KEY_7,
			/// @brief 8 key code.
			KEY_8,
			/// @brief 9 key code.
			KEY_9,
			/// @brief A key code.
			KEY_A,
			/// @brief B key code.
			KEY_B,
			/// @brief C key code.
			KEY_C,
			/// @brief D key code.
			KEY_D,
			/// @brief E key code.
			KEY_E,
			/// @brief F key code.
			KEY_F,
			/// @brief G key code.
			KEY_G,
			/// @brief H key code.
			KEY_H,
			/// @brief I key code.
			KEY_I,
			/// @brief J key code.
			KEY_J,
			/// @brief K key code.
			KEY_K,
			/// @brief L key code.
			KEY_L,
			/// @brief M key code.
			KEY_M,
			/// @brief N key code.
			KEY_N,
			/// @brief O key code.
			KEY_O,
			/// @brief P key code.
			KEY_P,
			/// @brief Q key code.
			KEY_Q,
			/// @brief R key code.
			KEY_R,
			/// @brief S key code.
			KEY_S,
			/// @brief T key code.
			KEY_T,
			/// @brief U key code.
			KEY_U,
			/// @brief V key code.
			KEY_V,
			/// @brief W key code.
			KEY_W,
			/// @brief X key code.
			KEY_X,
			/// @brief Y key code.
			KEY_Y,
			/// @brief Z key code.
			KEY_Z,
			/// @brief Left Windows key code.
			KEY_LWIN,
			/// @brief Right Windows key code.
			KEY_RWIN,
			/// @brief Computer sleep key code.
			KEY_SLEEP,
			/// @brief Numpad 0 key code.
			KEY_NUMPAD0,
			/// @brief Numpad 1 key code.
			KEY_NUMPAD1,
			/// @brief Numpad 2 key code.
			KEY_NUMPAD2,
			/// @brief Numpad 3 key code.
			KEY_NUMPAD3,
			/// @brief Numpad 4 key code.
			KEY_NUMPAD4,
			/// @brief Numpad 5 key code.
			KEY_NUMPAD5,
			/// @brief Numpad 6 key code.
			KEY_NUMPAD6,
			/// @brief Numpad 7 key code.
			KEY_NUMPAD7,
			/// @brief Numpad 8 key code.
			KEY_NUMPAD8,
			/// @brief Numpad 9 key code.
			KEY_NUMPAD9,
			/// @brief Add key code.
			KEY_NUMPAD_ADD,
			/// @brief Numpad subtract key code
			KEY_NUMPAD_SUBTRACT,
			/// @brief Numpad multiply key code.
			KEY_NUMPAD_MULTIPLY,
			/// @brief Numpad divide key code.
			KEY_NUMPAD_DIVIDE,
			/// @brief Numpad separator key code.
			KEY_NUMPAD_SEPARATOR,
			/// @brief Numpad decimal key code.
			KEY_NUMPAD_DECIMAL,
			/// @brief F1 key code.
			KEY_F1,
			/// @brief F2 key code.
			KEY_F2,
			/// @brief F3 key code.
			KEY_F3,
			/// @brief F4 key code.
			KEY_F4,
			/// @brief F5 key code.
			KEY_F5,
			/// @brief F6 key code.
			KEY_F6,
			/// @brief F7 key code.
			KEY_F7,
			/// @brief F8 key code.
			KEY_F8,
			/// @brief F9 key code.
			KEY_F9,
			/// @brief F10 key code.
			KEY_F10,
			/// @brief F11 key code.
			KEY_F11,
			/// @brief F12 key code.
			KEY_F12,
			/// @brief F13 key code.
			KEY_F13,
			/// @brief F14 key code.
			KEY_F14,
			/// @brief F15 key code.
			KEY_F15,
			/// @brief F16 key code.
			KEY_F16,
			/// @brief F17 key code.
			KEY_F17,
			/// @brief F18 key code.
			KEY_F18,
			/// @brief F19 key code.
			KEY_F19,
			/// @brief F20 key code.
			KEY_F20,
			/// @brief F21 key code.
			KEY_F21,
			/// @brief F22 key code.
			KEY_F22,
			/// @brief F23 key code.
			KEY_F23,
			/// @brief F24 key code.
			KEY_F24,
			/// @brief Keypad num lock key code.
			KEY_NUM_LOCK,
			/// @brief Scroll lock key code.
			KEY_SCROLL_LOCK,
			/// @brief Left shift key code.
			KEY_LSHIFT,
			/// @brief Right shift key code.
			KEY_RSHIFT,
			/// @brief Left control key code.
			KEY_LCONTROL,
			/// @brief Right control key code.
			KEY_RCONTROL,
			/// @brief Left alt key code.
			KEY_LALT,
			/// @brief Right alt key code.
			KEY_RALT,
			/// @brief Browser back key code.
			KEY_BROWSER_BACK,
			/// @brief Browser forward key code.
			KEY_BROWSER_FORWARD,
			/// @brief Browser refresh key code.
			KEY_BROWSER_REFRESH,
			/// @brief Browser stop key code.
			KEY_BROWSER_STOP,
			/// @brief Browser search key code.
			KEY_BROWSER_SEARCH,
			/// @brief Browser favorites key code.
			KEY_BROWSER_FAVORITES,
			/// @brief Browser home key code.
			KEY_BROWSER_HOME,
			/// @brief Volume mute key code.
			KEY_VOLUME_MUTE,
			/// @brief Volume down key code.
			KEY_VOLUME_DOWN,
			/// @brief Volume up key code.
			KEY_VOLUME_UP,
			/// @brief Media next key code.
			KEY_MEDIA_NEXT,
			/// @brief Media previous key code.
			KEY_MEDIA_PREV,
			/// @brief Media stop key code.
			KEY_MEDIA_STOP,
			/// @brief Media play/pause key code.
			KEY_MEDIA_PLAY_PAUSE,
			/// @brief Semicolon key code.
			KEY_SEMICOLON,
			/// @brief Equal key code.
			KEY_EQUAL,
			/// @brief Minus key code.
			KEY_MINUS,
			/// @brief Comma key code.
			KEY_COMMA,
			/// @brief Period key code.
			KEY_PERIOD,
			/// @brief Slash key code.
			KEY_SLASH,
			/// @brief Slash key code.
			KEY_BACKTICK,
			/// @brief Open square parenthesis key code.
			KEY_OPEN_SQR_PARENTHESIS,
			/// @brief Closed square parenthesis key code.
			KEY_CLOSED_SQR_PARENTHESIS,
			/// @brief Backslash key code.
			KEY_BACKSLASH,
			/// @brief Quotes key code.
			KEY_QUOTES
		} Key;

		/// @brief A struct containing the mouse's position.
		struct MousePosition {
			/// @brief The X coordinate of the mouse's position.
			int32_t x;
			/// @brief The Y coordinate of the mouse's position.
			int32_t y;
		};
		/// @brief A struct containing the mouse's movement between the two latest window event polls.
		struct MouseMovement {
			/// @brief The mouse movement on the X axis.
			int32_t x;
			/// @brief The mouse movement on the Y axis.
			int32_t y;
			/// @brief The scroll wheel's delta.
			int32_t scroll;
		};

		/// @brief A struct containing the info given on key down or key up events.
		struct KeyEventInfo {
			/// @brief The key that was pressed or released.
			Key key;
		};
		/// @brief A struct containing the info given on mouse move events.
		struct MouseMoveEventInfo {
			/// @brief The mouse's new position.
			MousePosition newPos;
			/// @brief The scroll wheel's delta.
			int32_t scrollDelta;
		};

		/// @brief Gets the key down event.
		/// @return A reference to the key down event.
		Event& GetKeyDownEvent() {
			return keyDownEvent;
		}
		/// @brief Gets the key up event.
		/// @return A reference to the key up event.
		Event& GetKeyUpEvent() {
			return keyUpEvent;
		}
		/// @brief Gets the mouse move event.
		/// @return A reference to the mouse move event.
		Event& GetMouseMoveEvent() {
			return mouseMoveEvent;
		}

		/// @brief Gets the mouse's position.
		/// @return The mouse's position.
		MousePosition GetMousePosition() const {
			return mousePos;
		}
		/// @brief Gets the mouse's movement between the two latest window event polls.
		/// @return A struct containing the mouse's movement in the specified time interval.
		MouseMovement GetMouseMovement() const {
			return mouseMovement;
		}
		/// @brief Checks if the given key is down.
		/// @param key The key to check.
		/// @return True if the key is down, otherwise false.
		bool8_t IsKeyDown(Key key) const {
			// Get the key state value and bit indices
			size_t valueIndex = (size_t)key >> 1;
			size_t bitIndex = ((size_t)key & 1) << 2;

			// Check the down bit of the current key
			return (keyStates[valueIndex] >> bitIndex) & 1;
		}
		/// @brief Checks if the given key is pressed.
		/// @param key The key to check.
		/// @return True if the key is pressed, otherwise false.
		bool8_t IsKeyPressed(Key key) const {
			// Get the key state value and bit indices
			size_t valueIndex = (size_t)key >> 1;
			size_t bitIndex = (((size_t)key & 1) << 2) + 1;

			// Check the pressed bit of the current key
			return (keyStates[valueIndex] >> bitIndex) & 1;
		}
		/// @brief Checks if the given key is released.
		/// @param key The key to check.
		/// @return True if the key is released, otherwise false.
		bool8_t IsKeyReleased(Key key) const {
			// Get the key state value and bit indices
			size_t valueIndex = (size_t)key >> 1;
			size_t bitIndex = (((size_t)key & 1) << 2) + 2;

			// Check the released bit of the current key
			return (keyStates[valueIndex] >> bitIndex) & 1;
		}
	private:
		friend Window;

		InputManager() = default;
		~InputManager() = default;

		void BeginInputEvents();
		void InternalKeyDown(uint64_t keyCode);
		void InternalKeyUp(uint64_t keyCode);
		void InternalMouseMove(MouseMovement movement);
		void EndInputEvents();

		Event keyDownEvent;
		Event keyUpEvent;
		Event mouseMoveEvent;

		uint8_t keyStates[128];
		MousePosition mousePos;
		MouseMovement mouseMovement;
	};
}