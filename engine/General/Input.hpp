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

    /// @brief Updates every input value. Meant for internal use.
    void UpdateInputValues();

    /// @brief Returns if the specified key is down.
    bool8_t KeyDown    (KeyCode key);
    /// @brief Returns if the specified key is up.
    bool8_t KeyUp      (KeyCode key);
    /// @brief Returns if the specified key was pressed this frame.
    bool8_t KeyPressed (KeyCode key);
    /// @brief Returns if the specified key was released this frame.
    bool8_t KeyReleased(KeyCode key);

    /// @brief Returns the mouse's position.
    MousePos GetMousePos();
    /// @brief Returns the mouse's movement this frame.
    MouseMovement GetMouseMovement();
    /// @brief Returns the mouse's display count.
    int32_t GetMouseDisplayCount();
    /// @brief Returns the mouse's state.
    MouseState GetMouseState();
    /// @brief Sets the mouse's state.
    void SetMouseState(MouseState newState);
}