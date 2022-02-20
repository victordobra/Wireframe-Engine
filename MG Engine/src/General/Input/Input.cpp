#include "Input.h"
#include "InputInternal.h"
#include "BuildInfo.h"
#include "GraphicsInfo.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace mge {
	//Key data
	bool8_t keysDown[255]{false};
	bool8_t keysPressed[255];
	bool8_t keysReleased[255];

	//Other variables
	MousePosition mousePosition{0, 0};
	MouseMovement mouseMovement{0, 0};
	bool8_t cursorLocked = false;

	//Platform specific functions
#ifdef PLATFORM_WINDOWS
	void StartInput() {
		//Set the mouse's position
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		mousePosition = { (size_t)mousePoint.x, (size_t)mousePoint.y };
	}
	void UpdateInput() {
		//Check every key
		for (size_t i = 0; i < 255; i++) {
			//Check if the key is pressed
			bool8_t keyDown = GetAsyncKeyState((int)i);
			
			if (!keysDown[i] && keyDown)
				keysPressed[i] = true;
			else
				keysPressed[i] = false;

			if (keysDown[i] && !keyDown)
				keysReleased[i] = true;
			else
				keysReleased[i] = false;

			keysDown[i] = keyDown;
		}

		//Get the mouse position
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		MousePosition newMousePosition = { (size_t)mousePoint.x, (size_t)mousePoint.y };

		//Compare it with the previous position to get the new position
		mouseMovement = { (sint64_t)newMousePosition.x - (sint64_t)mousePosition.x, (sint64_t)newMousePosition.y - (sint64_t)mousePosition.y };
		mousePosition = newMousePosition;

		//Set the cursor's position to the center of the screen if the cursor is locked
		if (cursorLocked) {
			SetCursorPos((sint32_t)(GetScreenWidth() >> 1), (sint32_t)(GetScreenHeight() >> 1));
			mousePosition = { GetScreenWidth() >> 1, GetScreenHeight() >> 1 };
		}
	}

	void LockCursor() {
		if (cursorLocked)
			return;
		cursorLocked = true;
		ShowCursor(FALSE);
	}
	void UnlockCursor() {
		if (!cursorLocked)
			return;
		cursorLocked = false;
		ShowCursor(TRUE);
	}
#endif

	bool8_t IsKeyDown(Key key) {
		return keysDown[(uint8_t)key];
	}
	bool8_t IsKeyUp(Key key) {
		return !keysDown[(uint8_t)key];
	}
	bool8_t IsKeyPressed(Key key) {
		return keysPressed[(uint8_t)key];
	}
	bool8_t IsKeyReleased(Key key) {
		return keysReleased[(uint8_t)key];
	}

	size_t GetMouseX() {
		return mousePosition.x;
	}
	size_t GetMouseY() {
		return mousePosition.y;
	}
	MousePosition GetMousePosition() {
		return mousePosition;
	}
	MouseMovement GetMouseMovement() {
		return mouseMovement;
	}

	bool8_t IsCursorLocked() {
		return cursorLocked;
	}
}