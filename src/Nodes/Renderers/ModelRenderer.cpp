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
        pushConstant.modelTransform = modelScaling * modelRotation * modelPosition;

        pushConstant.modelRotation.Transpose();
        pushConstant.modelTransform.Transpose();

        // Push the constants
        vkCmdPushConstants(commandBuffer, pipeline->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pushConstant);

        // Copy to the lighting buffer
        LightingUbo lightingUbo{};

        lightingUbo.cameraTransform = cameraPosition * cameraRotation * cameraPerspective;
        lightingUbo.cameraTransform.Transpose();
        lightingUbo.ambientLightColor = Light::ambientLightColor;

        for(const auto* light : Light::GetLights())
            light->AppendToLightingUbo(lightingUbo);
        
        Buffer* globalBuffer = pipeline->GetGlobalBuffer();
        globalBuffer->Map();
        globalBuffer->WriteToBuffer(&lightingUbo);
        globalBuffer->Flush();
        globalBuffer->Unmap();

        // Bind the descriptor sets
        DescriptorPool* descriptorPool = pipeline->GetDescriptorPool();
        set<Material*> materials = material->GetShader()->GetMaterials();
        size_t materialIndex = (size_t)(&(materials[material]) - materials.begin()) + 1;

        VkDescriptorSet lightingDescriptorSet = descriptorPool->GetDescriptorSets()[descriptorPool->GetDescriptorSetIndex(0)];
        VkDescriptorSet materialDescriptorSet = descriptorPool->GetDescriptorSets()[descriptorPool->GetDescriptorSetIndex(materialIndex)];
        VkDescriptorSet descriptorSets[2] = { lightingDescriptorSet, materialDescriptorSet };

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipelineLayout(), 0, 2, descriptorSets, 0, nullptr);

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
    MGE_ADD_NODE_PROPERTY(model, PUBLIC)
    MGE_ADD_NODE_PROPERTY(material, PUBLIC)
    MGE_END_NODE(ModelRenderer)
}