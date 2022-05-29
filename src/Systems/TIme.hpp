#pragma once

#include "Core.hpp"

namespace mge {
    void StartUpdateTimer();
    void StopUpdateTimer();
    
    float32_t GetDeltaTime();
    float32_t GetProgramRuntime();
    size_t GetFrameIndex();
}