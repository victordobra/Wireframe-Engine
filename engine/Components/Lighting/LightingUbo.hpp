#pragma once

#include "Math/EngineMath.hpp"

namespace wfe {
    struct LightingUbo {
        static const size_t MAX_LIGHT_COUNT = 128;

        struct DirectionalLight {
            Vector4 direction;
            Vector4 color;
        };
        struct PointLight {
            Vector4 position;
            Vector4 color;
        };

        Matrix4x4 cameraTransform;
        Vector4 cameraWorldPos;

        Vector4 ambientLightColor;
        
        DirectionalLight directionalLights[MAX_LIGHT_COUNT];
        PointLight       pointLights      [MAX_LIGHT_COUNT];

        uint32_t directionalLightCount = 0;
        uint32_t pointLightCount = 0;
    };
}