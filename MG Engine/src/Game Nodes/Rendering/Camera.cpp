#include "Camera.h"
#include "ModelRenderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#include "VulkanModel.h"
#include "SwapChain.h"
#include "RenderingPipeline.h"
#include "DescriptorPool.h"
#include "ShaderData.h"

#include "GraphicsInfo.h"
#include "EngineTime.h"
#include "EngineMath.h"

#include "Debugger.h"

namespace mge {
	void Camera::Start() {
		aspectRatio = (float32_t)GetScreenWidth() / (float32_t)GetScreenHeight();
	}

	void Camera::Render() {
		//Create the camera and projection matrix
		Matrix4x4 view = GetInvCameraMatrix();
		Matrix4x4 projection = Matrix4x4::PerspectiveProjection(fov * DEG_TO_RAD_MULTIPLIER, aspectRatio, zNear, zFar);

		view.Transpose();
		projection.Transpose();

		//Set all buffer values
		size_t frameIndex = GetFrameIndex() % MAX_FRAMES_IN_FLIGHT;

		Buffer* cameraBuffer = GetCameraBuffers()[frameIndex];
		Buffer* lightingBuffer = GetLightingBuffers()[frameIndex];
		Buffer* materialBuffer = GetMaterialBuffers()[frameIndex];

		//Write to the camera buffer
		CameraUbo cameraUbo;
		cameraUbo.view = view;
		cameraUbo.projection = projection;

		cameraBuffer->WriteToBuffer(&cameraUbo);
		cameraBuffer->Flush();

		//Write to the lighting buffer
		LightingUbo lightingUbo;

		std::vector<DirectionalLight*> dirLights;
		std::vector<PointLight*> pointLights;

		GetNodesOfType(dirLights);
		GetNodesOfType(pointLights);

		if (dirLights.size() > MAX_LIGHT_COUNT)
			OutFatalError("Too many directional lights have been created. To increase the maximum amount of lights allowed, modify MAX_LIGHT_COUNT in Rendering\\ShaderData.h");
		if (pointLights.size() > MAX_LIGHT_COUNT)
			OutFatalError("Too many point lights have been created. To increase the maximum amount of lights allowed, modify MAX_LIGHT_COUNT in Rendering\\ShaderData.h");

		lightingUbo.ambientLightColor = { 1.f, 1.f, 1.f, .1f };

		lightingUbo.directionalLightCount = (uint32_t)dirLights.size();
		lightingUbo.pointLightCount = (uint32_t)pointLights.size();

		for (size_t i = 0; i < dirLights.size(); i++) {
			lightingUbo.directionalLightColors[i] = dirLights[i]->color;
			lightingUbo.directionalLightDirections[i] = { dirLights[i]->direction.x, dirLights[i]->direction.y, dirLights[i]->direction.z, 1.f };
		}
		for (size_t i = 0; i < pointLights.size(); i++) {
			lightingUbo.pointLightColors[i] = pointLights[i]->color;
			lightingUbo.pointLightPositions[i] = { pointLights[i]->position.x, pointLights[i]->position.y, pointLights[i]->position.z, 1.f };
		}

		lightingBuffer->WriteToBuffer(&lightingUbo);
		lightingBuffer->Flush();

		//Save the material buffer's size
		uint32_t materialBufferSize = (uint32_t)materialBuffer->GetAlignmentSize();

		VkCommandBuffer commandBuffer;

		//Acquire the next image
		uint32_t imageIndex;
		auto result = AcquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			OutFatalError("Failed to acquire swap chain image!");

		//Allocate the command buffer
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GetCommandPool();
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
			OutFatalError("Failed to allocate command buffers!");

		//Begin recording the command buffer
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			OutFatalError("Failed to begin recording command buffer!");

		//Begin the render pass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = GetRenderPass();
		renderPassInfo.framebuffer = GetFrameBuffer((sint32_t)imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = GetSwapChainExtent();

		VkClearValue clearValues[2]{};
		if (clearMode == ClearMode::COLOR) {
			clearValues[0].color = { clearColor.x, clearColor.y, clearColor.z, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
		} else {
			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
		}
		
		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//Set the viewport and scissor
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float32_t)GetScreenWidth();
		viewport.height = (float32_t)GetScreenHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{ { 0, 0 }, GetSwapChainExtent() };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		//Copy all of the material data
		VkCommandBuffer imageCommandBuffer = BeginSingleTimeCommands();

		Image* diffuseImage = GetDiffuseImages()[frameIndex];
		diffuseImage->TransitionImageLayout(imageCommandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		VkImageSubresourceLayers subresourceLayers;
		subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceLayers.mipLevel = 0;
		subresourceLayers.baseArrayLayer = 0;
		subresourceLayers.layerCount = 1;

		for (size_t i = 0; i < Material::materials.size(); i++) {
			Material::materials[i]->WriteMaterialData(frameIndex);

			VkImageCopy imageCopyInfo;
			imageCopyInfo.srcSubresource = subresourceLayers;
			imageCopyInfo.dstSubresource = subresourceLayers;
			imageCopyInfo.extent = { targetImageSize, targetImageSize, 1 };
			imageCopyInfo.srcOffset = { 0, 0, 0 };
			imageCopyInfo.dstOffset = { (sint32_t)(targetImageSize * i), 0, 0 };

			vkCmdCopyImage(imageCommandBuffer, Material::materials[i]->image->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, diffuseImage->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyInfo);
		}

		diffuseImage->TransitionImageLayout(imageCommandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		EndSingleTimeCommands(imageCommandBuffer);

		std::vector<ModelRenderer*> renderers;
		GetNodesOfType(renderers);

		for (size_t i = 0; i < renderers.size(); i++) {
			ModelRenderer* modelRenderer = renderers[i];

			//Bind the pipeline
			PipelineBind(commandBuffer);

			//Bind the model renderer
			modelRenderer->Bind(commandBuffer);

			//Bind the descriptor sets
			VkDescriptorSet descriptorSets[2] = { GetDescriptorSets()[frameIndex], GetDescriptorSets()[frameIndex + MAX_FRAMES_IN_FLIGHT] };
			uint32_t dynamicOffsets[1] = { (uint32_t)modelRenderer->material->materialIndex * materialBufferSize };
			
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(), 0, 2, descriptorSets, 1, dynamicOffsets);

			//Push the transformation matrices
			Matrix4x4 meshPosition = Matrix4x4::Translation(modelRenderer->position);
			Matrix4x4 meshRotation = Matrix4x4::Rotation(modelRenderer->rotation);
			Matrix4x4 meshScale = Matrix4x4::Scaling(modelRenderer->scale);
			Matrix4x4 meshTransform = meshScale * meshRotation * meshPosition;
			
			meshRotation.Transpose();
			meshTransform.Transpose();

			PushConstantData push{};
			push.mesh = meshTransform;
			push.meshRot = meshRotation;
			
			vkCmdPushConstants(commandBuffer, GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantData), &push);
			
			//Draw the model
			modelRenderer->Draw(commandBuffer);
		}

		//End the render pass
		vkCmdEndRenderPass(commandBuffer);

		//End recording the command buffer
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			OutFatalError("Failed to end recording command buffer!");

		//Submit the command buffer
		result = SubmitCommandBuffers(&commandBuffer, &imageIndex);
		if (result != VK_SUCCESS)
			OutFatalError("Failed to submit command buffers!");

		//Clear the command buffers
		vkDeviceWaitIdle(GetDevice());
		vkFreeCommandBuffers(GetDevice(), GetCommandPool(), 1, &commandBuffer);
	}

	Matrix4x4 Camera::GetCameraMatrix() const {
		return Matrix4x4::Rotation(rotation) * Matrix4x4::Translation(position);
	}
	Matrix4x4 Camera::GetInvCameraMatrix() const {
		return Matrix4x4::Translation(-position) * Matrix4x4::Rotation(rotation.Inverted());
	}

	template<class T>
	void Camera::GetNodesOfType(std::vector<T*>& nodeVector, Node* parent) const {
		const std::vector<Node*>& children = parent->GetChildren();

		for (size_t i = 0; i < children.size(); i++) {
			T* newChild = dynamic_cast<T*>(children[i]);

			if (newChild != nullptr)
				nodeVector.push_back(newChild);

			GetNodesOfType(nodeVector, children[i]);
		}
	}
}