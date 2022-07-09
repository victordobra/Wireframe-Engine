#pragma once

#include "ECS/EngineECS.hpp"
#include "LightingUbo.hpp"

namespace wfe {
    System* GetLightingSystem();
    LightingUbo* GetLightingUbo();
}