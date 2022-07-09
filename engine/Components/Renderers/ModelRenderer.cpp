#include "ModelRenderer.hpp"
#include "Vulkan/EngineVulkan.hpp"
#include "Assets/Rendering/Pipeline.hpp"
#include "Assets/Rendering/Shader.hpp"
#include "Components/Lighting/Light.hpp"
#include "Components/Lighting/LightingSystem.hpp"

namespace wfe {
    void ModelRenderer::Bind(VkCommandBuffer commandBuffer, Camera* camera) {
        // If the model renderer doesn't have any material then it should neither be binded nor drawn; exit the function.
        if(!material || !model)
            return;
        
        // Bind the pipeline
        Pipeline* pipeline = material->GetShader()->GetPipeline();
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

        // Calculate all needed matrices
        const Transform& transform = GetGameObject()->transform;
        const Transform& cameraTransform = camera->GetGameObject()->transform;

        Matrix4x4 modelScaling = transform.GetScalingMatrix();
        Matrix4x4 modelRotation = transform.GetRotationMatrix();
        Matrix4x4 modelPosition = transform.GetTranslationMatrix();
        Matrix4x4 cameraInvPosition = cameraTransform.GetInvTranslationMatrix();
        Matrix4x4 cameraInvRotation = cameraTransform.GetInvRotationMatrix();
        Matrix4x4 cameraPerspective = camera->GetProjectionMatrix();

        // Create and push the push constants
        PushConstants pushConstants;

        pushConstants.modelRotation = modelRotation;
        pushConstants.modelRotation.Transpose();
        pushConstants.modelTransform = modelScaling * modelRotation * modelPosition;
        pushConstants.modelTransform.Transpose();

        vkCmdPushConstants(commandBuffer, pipeline->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

        // Update the lighting ubo based on the camera
        LightingUbo* lightingUbo = GetLightingUbo();
        lightingUbo->cameraTransform = cameraInvPosition * cameraInvRotation * cameraPerspective;
        lightingUbo->cameraTransform.Transpose();
        lightingUbo->cameraWorldPos = cameraTransform.position;

        // Update the global buffer
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

        // Bind vertex and index buffers
        VkBuffer buffers[] = { model->GetVertexBuffer()->GetBuffer() };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, model->GetIndexBuffer()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
    void ModelRenderer::Draw(VkCommandBuffer commandBuffer, Camera* camera) {
        // Only draw the model if the model renderer has a material
        if(material && model)
            vkCmdDrawIndexed(commandBuffer, model->GetIndexCount(), 1, 0, 0, 0);
    }

    WFE_COMPONENT(ModelRenderer)
    WFE_COMPONENT_PROPERTY(material, PUBLIC)
    WFE_COMPONENT_PROPERTY(model, PUBLIC)
    WFE_END_COMPONENT(ModelRenderer)
}