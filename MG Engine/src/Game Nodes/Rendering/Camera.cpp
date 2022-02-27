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

		std::vector<Node*> nodes = Node::scene->GetChildren();
		for (size_t i = 0; i < nodes.size(); i++) {
			Node* node = nodes[i];
			DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(node);
			PointLight* pointLight = dynamic_cast<PointLight*>(node);

			if (dirLight != nullptr) {
				lightingUbo.directionalLightDirections[lightingUbo.directionalLightCount] = { dirLight->direction.x, dirLight->direction.y, dirLight->direction.z, 1.f };
				lightingUbo.directionalLightColors[lightingUbo.directionalLightCount] = dirLight->color;

				lightingUbo.directionalLightCount++;
			} else if (pointLight != nullptr) {
				lightingUbo.pointLightPositions[lightingUbo.pointLightCount] = { pointLight->position.x, pointLight->position.y, pointLight->position.z, 1.f };
				lightingUbo.pointLightColors[lightingUbo.pointLightCount] = pointLight->color;

				lightingUbo.pointLightCount++;
			}
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

		//Bind the pipeline
		PipelineBind(commandBuffer);

		for (size_t i = 0; i < nodes.size(); i++) {
			Node* node = nodes[i];
			ModelRenderer* modelRenderer = dynamic_cast<ModelRenderer*>(node);
			//Check if the current game node is a mesh renderer
			if (modelRenderer == nullptr)
				continue;

			//Bind the model renderer
			modelRenderer->Bind(commandBuffer);

			//Bind the descriptor sets
			modelRenderer->material->WriteMaterialData(frameIndex);

			VkDescriptorSet descriptorSets[2] = { GetDescriptorSets()[frameIndex], GetDescriptorSets()[frameIndex + MAX_FRAMES_IN_FLIGHT] };
			uint32_t dynamicOffsets[1] = { (uint32_t)modelRenderer->material->materialIndex * materialBufferSize };

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(), 0, 2, descriptorSets, 1, dynamicOffsets);

			//Push the transformation matrices
			Matrix4x4 meshPosition = Matrix4x4::Translation(modelRenderer->position);
			Matrix4x4 meshRotation = Matrix4x4::Rotation(modelRenderer->rotation);
			Matrix4x4 meshScale = Matrix4x4::Scaling(modelRenderer->scale);
			Matrix4x4 meshTransform = modelRenderer->GetTransformationMatrix();

			Vector3 test;
			test = meshTransform * test;
			
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

	Matrix4x4 Camera::GetCameraMatrix() {
		return Matrix4x4::Rotation(rotation) * Matrix4x4::Translation(position);
	}
	Matrix4x4 Camera::GetInvCameraMatrix() {
		return Matrix4x4::Translation(-position) * Matrix4x4::Rotation(rotation.Inverted());
	}
}