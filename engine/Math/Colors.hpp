#pragma once
#include "Core.hpp"

namespace wfe {
    struct Color8 {
        uint8_t r = 0, g = 0, b = 0, a = 0;
    };
    struct Color16 {
        uint16_t r = 0, g = 0, b = 0, a = 0;
    };
    struct Color32 {
        uint32_t r = 0, g = 0, b = 0, a = 0;
    };
    struct Color64 {
        uint64_t r = 0, g = 0, b = 0, a = 0;
    };
    struct Color32f {
        float32_t r = 0.f, g = 0.f, b = 0.f, a = 0.f;
    };
    struct Color64f {
        float64_t r = 0., g = 0., b = 0., a = 0.;
    };
}