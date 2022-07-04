#include "Camera.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"

namespace wfe {
    void Camera::Render() {
		VkCommandBuffer commandBuffer;

        // Acquire the next image
		uint32_t imageIndex;
		VkResult result = AcquireNextImage(&imageIndex);

		if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			console::OutFatalError("Failed to acquire swap chain image!", 1);

		// Allocate the command buffer
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GetCommandPool();
		allocInfo.commandBufferCount = 1;

		if(vkAllocateCommandBuffers(GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
			console::OutFatalError("Failed to allocate command buffer!", 1);

		// Begin recording the command buffer
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		if(result != VK_SUCCESS)
			console::OutFatalError((string)"Failed to begin recording command buffer! Error code: " + VkResultToString(result), 1);

		// Begin the render pass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = GetRenderPass();
		renderPassInfo.framebuffer = GetFrameBuffer((sint32_t)imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = GetSwapChainExtent();

		VkClearValue clearValues[2]{};

		clearValues[0].color = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Set the viewport and scissor
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float32_t)GetSwapChainExtent().width;
		viewport.height = (float32_t)GetSwapChainExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{ { 0, 0 }, GetSwapChainExtent() };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		// Find every single renderer in the scene
		vector<Renderer*> renderers;
		AddRenderersToVector(renderers, Node::scene);

		// Run the bind and draw command for every single one
		for(auto* renderer : renderers) {
			renderer->Bind(commandBuffer, this);
			renderer->Draw(commandBuffer, this);
		}

        // End the render pass
		vkCmdEndRenderPass(commandBuffer);

        // Stop recording the command buffer
		result = vkEndCommandBuffer(commandBuffer);
		if(result != VK_SUCCESS)
			console::OutFatalError((string)"Failed to end recording command buffer! Error code: " + VkResultToString(result), 1);
		
        // Submit the command buffer
		result = SubmitCommandBuffers(&commandBuffer, &imageIndex);
		if(result != VK_SUCCESS)
			console::OutFatalError((string)"Failed to submit command buffers! Error code: " + VkResultToString(result), 1);
		
        // Free the command buffer
		vkDeviceWaitIdle(GetDevice());
		vkFreeCommandBuffers(GetDevice(), GetCommandPool(), 1, &commandBuffer);
    }

	void Camera::AddRenderersToVector(vector<Renderer*>& renderers, Node* parent) const {
		Renderer* renderer = dynamic_cast<Renderer*>(parent);
		if(renderer)
			renderers.push_back(renderer);
		for(auto* child : parent->GetChildren())
			AddRenderersToVector(renderers, child);
	}

    WFE_NODE(Camera)
    WFE_ADD_NODE_PROPERTY(position, PUBLIC)
    WFE_ADD_NODE_PROPERTY(rotation, PUBLIC)
    WFE_ADD_NODE_PROPERTY(fov, PUBLIC)
    WFE_ADD_NODE_PROPERTY(clearColor, PUBLIC)
	WFE_ADD_NODE_PROPERTY(nearPlane, PUBLIC)
	WFE_ADD_NODE_PROPERTY(farPlane, PUBLIC)
    WFE_END_NODE(Camera)
}