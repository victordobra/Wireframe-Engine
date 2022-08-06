#pragma once

#include "ECS/EngineECS.hpp"
#include "LightingUbo.hpp"

namespace wfe {
    /// @brief Returns the lighting system.
    System* GetLightingSystem();
    /// @brief Returns a pointer of the lighting ubo.
    LightingUbo* GetLightingUbo();
}