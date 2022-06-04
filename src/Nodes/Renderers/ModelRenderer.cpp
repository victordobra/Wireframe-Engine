#include "ModelRenderer.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Assets/Rendering/Pipeline.hpp"

namespace mge {
    void ModelRenderer::Bind(VkCommandBuffer commandBuffer, Camera* camera) {
        // Bind the pipeline
        Pipeline* pipeline = material->GetShader()->GetPipeline();
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

        // Calculate push constants
        PushConstant pushConstant;

        Matrix4x4 modelScaling = Matrix4x4::Scaling(body->scale);
        Matrix4x4 modelRotation = Matrix4x4::Rotation(body->rotation);
        Matrix4x4 modelPosition = Matrix4x4::Translation(body->position);
        Matrix4x4 cameraPosition = Matrix4x4::Translation(-camera->position);
        Matrix4x4 cameraRotation = Matrix4x4::Rotation(camera->rotation.Inverted());
        Matrix4x4 cameraPerspective = Matrix4x4::PerspectiveProjection(camera->fov * RAD_TO_DEG_MULTIPLIER, ExtentAspectRatio(), camera->nearPlane, camera->farPlane);

        pushConstant.modelRotation = modelRotation;
        pushConstant.transformMatrix = modelScaling * modelRotation * modelPosition * cameraPosition * cameraRotation * cameraPerspective;

        pushConstant.modelRotation.Transpose();
        pushConstant.transformMatrix.Transpose();

        // Push the constants
        vkCmdPushConstants(commandBuffer, pipeline->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pushConstant);

        // Bind the vertex and index buffers
        VkBuffer buffers[] = { model->GetVertexBuffer()->GetBuffer() };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, model->GetIndexBuffer()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
    void ModelRenderer::Draw(VkCommandBuffer commandBuffer, Camera* camera) {
        vkCmdDrawIndexed(commandBuffer, model->GetIndexCount(), 1, 0, 0, 0);
    }

    MGE_NODE(ModelRenderer)
    MGE_ADD_PROPERTY(ModelRenderer, Model*, model, PUBLIC)
    MGE_ADD_PROPERTY(ModelRenderer, Material*, material, PUBLIC)
    MGE_END_NODE(ModelRenderer)
}