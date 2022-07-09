#pragma once

#include "Components/Rendering/Renderer.hpp"
#include "Assets/Rendering/Material.hpp"
#include "Assets/General/Model.hpp"

namespace wfe {
    class ModelRenderer : public Renderer {
    public:
        struct PushConstants {
            Matrix4x4 modelTransform;
            Matrix4x4 modelRotation;
        };

        Material* material;
        Model* model;

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