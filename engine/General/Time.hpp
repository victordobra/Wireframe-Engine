#pragma once

#include "Core.hpp"

namespace wfe {
    /// @brief Starts the update timer. Internal use only.
    void StartUpdateTimer();
    /// @brief Stops the update timer. Interal use only.
    void StopUpdateTimer();
    
    /// @brief Returns the current frame's delta time.
    float32_t GetDeltaTime();
    /// @brief Returns the total program's runtime.
    float32_t GetProgramRuntime();
    /// @brief Returns the current frame's index.
    size_t GetFrameIndex();
}