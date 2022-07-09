#pragma once

#include "Vulkan/VulkanInclude.hpp"
#include "ECS/EngineECS.hpp"
#include "Camera.hpp"

namespace wfe {
    class Renderer : public Component {
    public:
        Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        virtual void Bind(VkCommandBuffer commandBuffer, Camera* camera) { }
        virtual void Draw(VkCommandBuffer commandBuffer, Camera* camera) { }

        virtual ~Renderer() = default;
    };
}