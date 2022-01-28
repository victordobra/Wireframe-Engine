#include "Camera.h"
#include "MeshRenderer.h"
#include "VulkanModel.h"
#include "SwapChain.h"
#include "RenderingPipeline.h"
#include "PushConstantData.h"

namespace mge {
	void Camera::GameRender() {
		//Create the camera and projection matrix
		Matrix4x4 camera = GetCameraMatrix();
		Matrix4x4 projection;
		float tanHalf = Tan(fov / 2.0f * degToRad);

		projection[0][0] = 1.0f / (tanHalf * aspectRatio);
		projection[1][1] = 1.0f / tanHalf;
		projection[2][2] = zFar / (zFar - zNear);
		projection[2][3] = -(zFar * zNear) / (zFar - zNear);
		projection[3][2] = 1.0f;
		projection[3][3] = 0.0f;

		Vector4 test{ 0.0f, -0.5f, 1.0f, 1.0f };
		Vector4 testResult = projection * test;

		VkCommandBuffer commandBuffer;

		//Acquire the next image
		uint32_t imageIndex;
		auto result = AcquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire swap chain image!");

		//Allocate the command buffer
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GetCommandPool();
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device(), &allocInfo, &commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers!");

		//Begin recording the command buffer
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("Failed to begin recording command buffer!");

		//Begin the render pass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = GetRenderPass();
		renderPassInfo.framebuffer = GetFrameBuffer((int)imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { clearColor.x, clearColor.y, clearColor.z, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//Set the viewport and scissor
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)OSMGetGameWidth();
		viewport.height = (float)OSMGetGameHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ { 0, 0 }, GetSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		//Bind the pipeline
		PipelineBind(commandBuffer);

		for (size_t i = 0; i < GameNode::gameNodeCount; i++) {
			GameNode* gameNode = GameNode::gameNodes[i];
			MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(gameNode);
			//Check if the current game node is a mesh renderer
			if (meshRenderer == nullptr)
				continue;

			//Bind the model
			meshRenderer->Bind(commandBuffer);
			
			//Push the transformation matrices
			Matrix4x4 meshTransform = meshRenderer->GetTransformationMatrix();
			PushConstantData push{ meshTransform, camera * projection };
			vkCmdPushConstants(commandBuffer, GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantData), &push);

			//Draw the model
			meshRenderer->Draw(commandBuffer);
		}

		//End the render pass
		vkCmdEndRenderPass(commandBuffer);

		//End recording the command buffer
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to end recording command buffer!");

		//Submit the command buffer
		result = SubmitCommandBuffers(&commandBuffer, &imageIndex);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to submit command buffers!");

		//Clear the command buffers
		vkDeviceWaitIdle(device());
		vkFreeCommandBuffers(device(), GetCommandPool(), 1, &commandBuffer);
	}

	Matrix4x4 Camera::GetCameraMatrix() {
		Matrix4x4 translation = Matrix4x4::Translation(-position);
		Matrix4x4 rot = Matrix4x4::Rotation(rotation.Inverted());
		Matrix4x4 product = translation * rot;
		return product;
	}
}