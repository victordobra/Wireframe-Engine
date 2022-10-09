#pragma once

#include "Core.hpp"
#include "ECS/EngineECS.hpp"
#include "Math/EngineMath.hpp"
#include "Vulkan/SwapChain.hpp"

namespace wfe {
    class Camera : public Component {
    public:
        float32_t fov = 60.f;
        float32_t nearPlane = .01f;
        float32_t farPlane = 1000.f;
        Color32 clearColor{ .01f, .01f, .01f, 1.f };

        Matrix4x4 GetProjectionMatrix() {
            return Matrix4x4::PerspectiveProjection(fov * RAD_TO_DEG_MULTIPLIER, ExtentAspectRatio(), nearPlane, farPlane);
        }
    };
}