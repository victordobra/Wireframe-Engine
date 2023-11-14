#pragma once

#include <Core.hpp>
#include "KeyCode.hpp"

namespace wfe {
	/// @brief A struct containing the mouse's position.
	struct MousePosition {
		/// @brief The X coordinate of the mouse's position.
		int32_t x;
		/// @brief The Y coordinate of the mouse's position.
		int32_t y;
	};

	/// @brief A struct containing the info given on key down or key up events.
	struct KeyEventInfo {
		/// @brief The key code of the current event's key.
		KeyCode keyCode;
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
	Event& GetKeyDownEvent();
	/// @brief Gets the key up event.
	/// @return A reference to the key up event.
	Event& GetKeyUpEvent();
	/// @brief Gets the mouse move event.
	/// @return A reference to the mouse move event.
	Event& GetMouseMoveEvent();

	/// @brief Gets the mouse's position.
	/// @return The mouse's position.
	MousePosition GetMousePosition();
}