#include "Input.hpp"
#include "General/Application.hpp"
#include "Platform/Platform.hpp"

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(PLATFORM_LINUX)
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#endif

namespace wfe {
    // Variables
    uint8_t statuses[128]{};
    MousePos mousePos;
    MouseMovement mouseMovement;
    int32_t mouseDisplayCount;

    // External functions
    void ProcessInputEvents() {
        // Reset every key status
        for(size_t i = 0; i < 128; ++i)
            statuses[i] &= 0x11; // 00010001 binary

        // Get every key event
        auto keyEvents = GetEventsOfType(EVENT_TYPE_KEY);

        for(auto& keyEvent : keyEvents) {
            KeyEventInfo* keyEventInfo = (KeyEventInfo*)keyEvent.eventInfo;
            uint8_t status = statuses[keyEventInfo->keyCode << 1];

            if(keyEventInfo->keyCode & 1)
                status <<= 4;

            if(keyEventInfo->pressed) {
                // Update the key's status
                if(!(status & 1))
                    status |= 2;
                status |= 1;
            } else {
                // Update the key's status
                if(!(status & 2)) {
                    status |= 5;
                }
            }

            // Insert the status back into the vector
            if(keyEventInfo->keyCode & 1) {
                statuses[keyEventInfo->keyCode << 1] &= 0x0f; // 00001111 binary
                statuses[keyEventInfo->keyCode << 1] |= status << 4;
            } else {
                statuses[keyEventInfo->keyCode << 1] &= 0xf0; // 11110000 binary
                statuses[keyEventInfo->keyCode << 1] |= status;
            }
        }
        
        // Get every mouse move event
        auto mouseMoveEvents = GetEventsOfType(EVENT_TYPE_MOUSE_MOVE);

        // Check if any mouse move events exist
        if(mouseMoveEvents.size()) {
            MouseMoveEventInfo* mouseMoveEventInfo = (MouseMoveEventInfo*)mouseMoveEvents.back().eventInfo;

            // Set the new mouse pos
            MousePos newMousePos;
            newMousePos.x = mouseMoveEventInfo->mouseX;
            newMousePos.y = mouseMoveEventInfo->mouseY;

            // Set the mouse movement and pos
            mouseMovement.x = newMousePos.x - mousePos.x;
            mouseMovement.y = newMousePos.y - mousePos.y;

            mousePos = newMousePos;
        } else {
            // Reset the mouse movement;
            mouseMovement.x = 0;
            mouseMovement.y = 0;
        }

        // Reset the mouse's position if it is locked
        if(mouseDisplayCount < 0) {
#if defined(PLATFORM_WINDOWS)
            if(!SetCursorPos((int32_t)(GetMainWindowWidth() >> 1), (int32_t)(GetMainWindowWidth() >> 1))) {
                char_t error[256];
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error, 256 * sizeof(char_t), NULL);
                console::OutFatalError((string)"Failed to set the cursor's position! Reason: " + error, 1);
            }
#elif defined(PLATFORM_LINUX)
            PlatformInfo* platformInfo = GetPlatformInfo();

            XWarpPointer(platformInfo->display, platformInfo->window, platformInfo->window, 0, 0, GetMainWindowWidth(), GetMainWindowHeight(), GetMainWindowWidth() >> 1, GetMainWindowHeight() >> 1);
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
    int32_t GetMouseDisplayCount() {
        return mouseDisplayCount;
    }
    MouseState GetMouseState() {
        return (MouseState)(mouseDisplayCount >= 0);
    }
    void SetMouseState(MouseState newState) {
        PlatformInfo* platformInfo = GetPlatformInfo();

#if defined(PLATFORM_WINDOWS)
        mouseDisplayCount = ShowCursor(newState == MOUSE_STATE_UNLOCKED);
#elif defined(PLATFORM_LINUX)
        int32_t oldMouseDisplayCount = mouseDisplayCount;
        
        if(newState == MOUSE_STATE_UNLOCKED)
            ++mouseDisplayCount;
        else
            --mouseDisplayCount;
        
        if(mouseDisplayCount < 0 && oldMouseDisplayCount == 0) {
            XFixesHideCursor(platformInfo->display, platformInfo->window);
            XFlush(platformInfo->display);
        } else if(mouseDisplayCount >= 0 && oldMouseDisplayCount == -1) {
            XFixesShowCursor(platformInfo->display, platformInfo->window);
            XFlush(platformInfo->display);
        }
#endif
    }
}