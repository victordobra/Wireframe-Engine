#pragma once
#include "Defines.h"
#include "Key.h"

namespace mge {
	struct MousePosition {
		size_t x;
		size_t y;
	};
	struct MouseMovement {
		sint64_t x;
		sint64_t y;
	};

	/// @brief Returns if a key is currently pressed
	bool8_t IsKeyDown(Key key);
	/// @brief Returns if a key is currently not pressed
	bool8_t IsKeyUp(Key key);
	/// @brief Returns true on the first frame that the key is pressed
	bool8_t IsKeyPressed(Key key);
	/// @brief Returns true on the frame that the key is released
	bool8_t IsKeyReleased(Key key);

	/// @brief Returns the mouse's X position
	size_t GetMouseX();
	/// @brief Returns the mouse's Y position
	size_t GetMouseY();
	/// @brief Returns the mouse's position
	MousePosition GetMousePosition();
	/// @brief Returns the mouse's movement compared to the previous frame
	MouseMovement GetMouseMovement(); 

	/// @brief Returns if the cursor is locked
	bool8_t IsCursorLocked();
	/// @brief Locks the cursor
	void LockCursor();
	/// @brief Unlocks the cursor
	void UnlockCursor();
}