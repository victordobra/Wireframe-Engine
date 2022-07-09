#pragma once

#include "KeyCode.hpp"

namespace wfe {
    typedef struct {
        int64_t x, y;
    } MousePos, MouseMovement;
    typedef enum : uint8_t {
        MOUSE_STATE_LOCKED,
        MOUSE_STATE_UNLOCKED
    } MouseState;

    void UpdateInputValues();

    bool8_t KeyDown    (KeyCode key);
    bool8_t KeyUp      (KeyCode key);
    bool8_t KeyPressed (KeyCode key);
    bool8_t KeyReleased(KeyCode key);

    MousePos GetMousePos();
    MouseMovement GetMouseMovement();
    MouseState GetMouseState();
    void SetMouseState(MouseState newState);
}