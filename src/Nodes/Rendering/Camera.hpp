#pragma once

#include "Vulkan/VulkanInclude.hpp"
#include "Base/Node.hpp"
#include "Math/EngineMath.hpp"

namespace mge {
    class Camera : public Node {
    public:
        Vector3 position{0.f, 0.f, 0.f};
        Quaternion rotation{};

        float32_t fov{60.f};
        Color8 clearColor{10, 10, 10, 255};

        Camera() = default;
        Camera(const Camera&) = delete;
        Camera(Camera&&) noexcept = delete;

        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) noexcept = delete;

        void Render() override;

        ~Camera() = default;
    };
}