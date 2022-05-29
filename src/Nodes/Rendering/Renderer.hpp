#pragma once
#include "Base/Node.hpp"
#include "Nodes/General/Body.hpp"
#include "Vulkan/VulkanInclude.hpp"

namespace mge {
    class Renderer : public Node {
    public:
        Body* body;

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept  = delete;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        virtual void Bind(VkCommandBuffer commandBuffer) { }
        virtual void Draw(VkCommandBuffer commandBuffer) { }

        virtual ~Renderer() = default;
    };
}