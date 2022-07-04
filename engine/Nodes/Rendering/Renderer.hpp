#pragma once
#include "Base/Node.hpp"
#include "Nodes/General/Body.hpp"
#include "Assets/Rendering/Material.hpp"
#include "Vulkan/VulkanInclude.hpp"

namespace wfe {
    class Camera;

    class Renderer : public Node {
    public:
        Body* body;
        Material* material;

        Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept  = delete;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        void Start() override;

        virtual void Bind(VkCommandBuffer commandBuffer, Camera* camera) { }
        virtual void Draw(VkCommandBuffer commandBuffer, Camera* camera) { }

        virtual ~Renderer() = default;
    };
}