#pragma once

#include "Assets/General/Model.hpp"
#include "Nodes/Rendering/Renderer.hpp"
#include "Math/EngineMath.hpp"
#include "Nodes/Rendering/Camera.hpp"

namespace mge {
    class ModelRenderer : public Renderer {
    public:
        struct PushConstant {
            Matrix4x4 transformMatrix;
            Matrix4x4 modelRotation;
        };

        Model* model{nullptr};

        ModelRenderer() = default;
        ModelRenderer(const ModelRenderer&) = delete;
        ModelRenderer(ModelRenderer&&) noexcept = delete;
        
        ModelRenderer& operator=(const ModelRenderer&) = delete;
        ModelRenderer& operator=(ModelRenderer&&) noexcept = delete;

        void Bind(VkCommandBuffer commandBuffer, Camera* camera) override;
        void Draw(VkCommandBuffer commandBuffer, Camera* camera) override;

        ~ModelRenderer() = default;
    };
}