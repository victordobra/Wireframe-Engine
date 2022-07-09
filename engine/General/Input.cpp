#include "Input.hpp"
#include "Window/MainWindow.hpp"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace wfe {
    // Variables
    uint8_t statuses[128]{};
    MousePos mousePos;
    MouseMovement mouseMovement;
    MouseState mouseState{MOUSE_STATE_UNLOCKED};

    // Internal helper functions
    static bool8_t InternalKeyPressed(size_t keyCode) {
#ifdef PLATFORM_WINDOWS
        return GetAsyncKeyState((int)keyCode) >> 15;
#endif
    }
    static MousePos InternalGetMousePos() {
#ifdef PLATFORM_WINDOWS
        POINT point;

        BOOL result = GetCursorPos(&point);

        if(!result) {
            int32_t errorCode = GetLastError();
            console::OutFatalError((string)"Failed to retrieve the cursor's position! Error code: " + ToString(errorCode), 1);
        }

        MousePos pos;
        pos.x = point.x;
        pos.y = point.y;

        return pos;
#endif
    }

    // External functions
    void UpdateInputValues() {
        // Update the status of every single key
        for(size_t i = 0; i < 128; ++i) {
            // Get the old status variables
            uint8_t status = statuses[i];
            statuses[i] = 0;

            bool8_t prevDown     = status & 1;
            bool8_t prevPressed  = status & 2;
            bool8_t prevReleased = status & 4;

            // Get if the current key is down
            bool8_t newDown = InternalKeyPressed(2 * i);

            // Modify the status of the key
            prevDown = newDown;
            prevPressed = newDown && !prevDown;
            prevReleased = prevDown && !newDown;
            statuses[i] |= prevDown + (prevPressed << 1) + (prevReleased << 2);

            // Repeat the same thing for the other key value
            status <<= 4;

            prevDown     = status & 1;
            prevPressed  = status & 2;
            prevReleased = status & 4;

            // Get if the current key is down
            newDown = InternalKeyPressed(2 * i + 1);

            // Modify the status of the key
            prevPressed = newDown && !prevDown;
            prevReleased = prevDown && !newDown;
            prevDown = newDown;
            statuses[i] |= (prevDown << 4) + (prevPressed << 5) + (prevReleased << 6);
        }

        // Get the new mouse position
        MousePos newPos = InternalGetMousePos();

        // Set the mouse's movement and position
        mouseMovement.x = newPos.x - mousePos.x;
        mouseMovement.y = newPos.y - mousePos.y;
        mousePos = newPos;

        // Reset the mouse's position if it is locked
        if(mouseState == MOUSE_STATE_LOCKED) {
#ifdef PLATFORM_WINDOWS
            BOOL result = SetCursorPos((int)(GetWidth() >> 1), (int)(GetHeight() >> 1));

            if(!result) {
                int32_t errorCode = GetLastError();
                console::OutFatalError((string)"Failed to set the cursor's position! Error code: " + ToString(errorCode), 1);
            }
#endif
        }
    }

    bool8_t KeyDown    (KeyCode key) { 
        // Get the down bit of the requested status
        size_t statusIndex = key >> 1;
        size_t statusPosition = (key & 1) * 4;

        return (statuses[statusIndex] >> statusPosition) & 1;
    }
    bool8_t KeyUp      (KeyCode key) { 
        // Get the down bit of the requested status and reverse it
        size_t statusIndex = key >> 1;
        size_t statusPosition = (key & 1) * 4;

        return !((statuses[statusIndex] >> statusPosition) & 1);
    }
    bool8_t KeyPressed (KeyCode key) { 
        // Get the pressed bit of the requested status
        size_t statusIndex = key >> 1;
        size_t statusPosition = (key & 1) * 4;

        return (statuses[statusIndex] >> (statusPosition + 1)) & 1;
    }
    bool8_t KeyReleased(KeyCode key) { 
        // Get the released bit of the requested status
        size_t statusIndex = key >> 1;
        size_t statusPosition = (key & 1) * 4;

        return (statuses[statusIndex] >> (statusPosition + 1)) & 1;
    }

    MousePos GetMousePos() {
        return mousePos;
    }
    MouseMovement GetMouseMovement() {
        return mouseMovement;
    }
    MouseState GetMouseState() {
        return mouseState;
    }
    void SetMouseState(MouseState newState) {
        mouseState = newState;
#ifdef PLATFORM_WINDOWS
        if(mouseState == MOUSE_STATE_LOCKED) {
            while(ShowCursor(FALSE)) { }
        } else {
            ShowCursor(TRUE);
        }
#endif
    }
}