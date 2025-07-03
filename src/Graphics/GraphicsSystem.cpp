#include "GraphicsSystem.hpp"
#include <future>
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Pipeline async callback
	static VkCommandBuffer PipelineRender(GraphicsPipeline* pipeline) {
		return pipeline->RecordCommands();
	}

	// Internal functions
	void GraphicsSystem::RegisterPipeline(GraphicsPipeline* pipeline) {
		// Add the pipeline to the vector
		pipelines.push_back(pipeline);
	}
	void GraphicsSystem::UnregisterPipeline(GraphicsPipeline* pipeline) {
		// Find the pipeline in the vector
		for(size_t i = 0; i != pipelines.size(); ++i) {
			if(pipelines[i] == pipeline) {
				pipelines.erase(pipelines.begin() + i);
				break;
			}
		}
	}

	// Public functions
	GraphicsSystem::GraphicsSystem(VulkanRenderer* renderer, size_t maxFramesInFlight) : renderer(renderer) {
		// Set the rendering command buffer alloc info
		VkCommandBufferAllocateInfo renderingCommandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = renderer->GetGraphicsCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = (uint32_t)maxFramesInFlight
		};

		// Allocate the rendering command buffers
		renderingCommandBuffers.resize(maxFramesInFlight);
		VkResult result = renderer->GetLoader()->vkAllocateCommandBuffers(renderer->GetDevice()->GetDevice(), &renderingCommandBufferAllocInfo, renderingCommandBuffers.data());
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan rendering command buffers! Error code: " + string_VkResult(result));

		// Set the rendering fence create info
		VkFenceCreateInfo renderingFenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		// Create the rendering fences
		renderingFences.resize(maxFramesInFlight);
		for(size_t i = 0; i != maxFramesInFlight; ++i) {
			result = renderer->GetLoader()->vkCreateFence(renderer->GetDevice()->GetDevice(), &renderingFenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &renderingFences[i]);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan rendering fence! Error code: " + string_VkResult(result));
		}

		// Set the rendering semaphore create info
		VkSemaphoreCreateInfo semaphoreInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the image available semaphores
		imageAvailableSemaphores.resize(maxFramesInFlight);
		for(size_t i = 0; i != maxFramesInFlight; ++i) {
			result = renderer->GetLoader()->vkCreateSemaphore(renderer->GetDevice()->GetDevice(), &semaphoreInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &imageAvailableSemaphores[i]);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan image available semaphore! Error code: " + string_VkResult(result));
		}

		// Create the rendering semaphores
		renderingSemaphores.resize(renderer->GetSwapChain()->GetSwapChainImages().size());
		for(size_t i = 0; i != renderingSemaphores.size(); ++i) {
			result = renderer->GetLoader()->vkCreateSemaphore(renderer->GetDevice()->GetDevice(), &semaphoreInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &renderingSemaphores[i]);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan rendering semaphore! Error code: " + string_VkResult(result));
		}
	}

	void GraphicsSystem::Render() {
		// Skip the current frame if the swap chain does not exist
		if(!renderer->GetSwapChain()->GetSwapChain())
			return;

		// Wait for the last frame in flight to finish rendering
		VkResult result = renderer->GetLoader()->vkWaitForFences(renderer->GetDevice()->GetDevice(), 1, &renderingFences[frameIndex], VK_TRUE, UINT64_T_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan rendering fence! Error code: " + string_VkResult(result));
		
		// Reset the rendering fence
		result = renderer->GetLoader()->vkResetFences(renderer->GetDevice()->GetDevice(), 1, &renderingFences[frameIndex]);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to reset Vulkan rendering fence! Error code: " + string_VkResult(result));

		// Acquire the next swap chain image
		uint32_t imageIndex;
		result = renderer->GetLoader()->vkAcquireNextImageKHR(renderer->GetDevice()->GetDevice(), renderer->GetSwapChain()->GetSwapChain(), UINT64_T_MAX, imageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, &imageIndex);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to acquire next Vulkan swap chain image! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBuffer commandBuffer = renderingCommandBuffers[frameIndex];

		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = renderer->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan rendering command buffer! Error code: " + string_VkResult(result));
		
		// Set the image barriers for the initial transitions
		VkImageMemoryBarrier2KHR imageBarriers[2] {
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR,
				.pNext = nullptr,
				.srcStageMask = VK_PIPELINE_STAGE_2_NONE_KHR,
				.srcAccessMask = VK_ACCESS_2_NONE_KHR,
				.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
				.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = renderer->GetSwapChain()->GetSwapChainImages()[imageIndex].image,
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			},
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR,
				.pNext = nullptr,
				.srcStageMask = VK_PIPELINE_STAGE_2_NONE_KHR,
				.srcAccessMask = VK_ACCESS_2_NONE_KHR,
				.dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT_KHR,
				.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT_KHR | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT_KHR,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = renderer->GetSwapChain()->GetSwapChainImages()[imageIndex].depthImage,
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			}
		};

		// Set the start dependency info
		VkDependencyInfoKHR startDependencyInfo {
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR,
			.pNext = nullptr,
			.dependencyFlags = 0,
			.memoryBarrierCount = 0,
			.pMemoryBarriers = nullptr,
			.bufferMemoryBarrierCount = 0,
			.pBufferMemoryBarriers = nullptr,
			.imageMemoryBarrierCount = 2,
			.pImageMemoryBarriers = imageBarriers
		};
		
		// Transition the color and depth image layouts
		renderer->GetLoader()->vkCmdPipelineBarrier2KHR(commandBuffer, &startDependencyInfo);
		
		// Set the rendering info
		VkRenderingInfoKHR renderingInfo {
			.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
			.pNext = nullptr,
			.flags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT_KHR,
			.renderArea = {
				.offset = { 0, 0 },
				.extent = renderer->GetSwapChain()->GetExtent()
			},
			.layerCount = 1,
			.viewMask = 0,
			.colorAttachmentCount = 1,
			.pColorAttachments = &renderer->GetSwapChain()->GetSwapChainImages()[imageIndex].attachmentInfo,
			.pDepthAttachment = &renderer->GetSwapChain()->GetSwapChainImages()[imageIndex].depthAttachmentInfo,
			.pStencilAttachment = nullptr,
		};

		// Begin rendering
		renderer->GetLoader()->vkCmdBeginRenderingKHR(commandBuffer, &renderingInfo);

		if(!pipelines.empty()) {
			// Record all pipeline command buffers asynchronously
			std::vector<VkCommandBuffer> commandBuffers;
			std::vector<std::future<VkCommandBuffer>> commandBufferFutures;

			commandBufferFutures.resize(pipelines.size());
			for(size_t i = 0; i != pipelines.size(); ++i)
				commandBufferFutures[i] = std::async(PipelineRender, pipelines[i]);

			commandBuffers.resize(pipelines.size());
			for(size_t i = 0; i != pipelines.size(); ++i)
				commandBuffers[i] = commandBufferFutures[i].get();
			
			// Execute all command buffers
			renderer->GetLoader()->vkCmdExecuteCommands(commandBuffer, (uint32_t)commandBuffers.size(), commandBuffers.data());
		}

		// End rendering
		renderer->GetLoader()->vkCmdEndRenderingKHR(commandBuffer);

		// Set the image barrier
		VkImageMemoryBarrier2KHR imageBarrier {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR,
			.pNext = nullptr,
			.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
			.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT_KHR | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR,
			.dstStageMask = VK_PIPELINE_STAGE_2_NONE_KHR,
			.dstAccessMask = VK_ACCESS_2_NONE_KHR,
			.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = renderer->GetSwapChain()->GetSwapChainImages()[imageIndex].image,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		// Set the end dependency info
		VkDependencyInfoKHR endDependencyInfo {
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR,
			.pNext = nullptr,
			.dependencyFlags = 0,
			.memoryBarrierCount = 0,
			.pMemoryBarriers = nullptr,
			.bufferMemoryBarrierCount = 0,
			.pBufferMemoryBarriers = nullptr,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &imageBarrier
		};

		// Transition the color image layout for presenting
		renderer->GetLoader()->vkCmdPipelineBarrier2KHR(commandBuffer, &endDependencyInfo);

		// End recording the command buffer
		result = renderer->GetLoader()->vkEndCommandBuffer(commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to end recording Vulkan rendering command buffer! Error code: " + string_VkResult(result));

		// Set the submit info
		VkSemaphoreSubmitInfoKHR semaphoreWaitInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.semaphore = imageAvailableSemaphores[frameIndex],
			.value = 0,
			.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
			.deviceIndex = 0
		};
		VkCommandBufferSubmitInfoKHR commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
			.deviceMask = 0
		};
		VkSemaphoreSubmitInfoKHR semaphoreSignalInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.semaphore = renderingSemaphores[imageIndex],
			.value = 0,
			.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR,
			.deviceIndex = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.flags = 0,
			.waitSemaphoreInfoCount = 1,
			.pWaitSemaphoreInfos = &semaphoreWaitInfo,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 1,
			.pSignalSemaphoreInfos = &semaphoreSignalInfo
		};

		// Submit the command buffer
		result = renderer->GetLoader()->vkQueueSubmit2KHR(renderer->GetDevice()->GetDeviceQueues().graphicsQueue, 1, &submitInfo, renderingFences[frameIndex]);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan graphics command buffer! Error code: " + string_VkResult(result));
		
		// Set the present info
		VkSemaphore waitSemaphore = renderingSemaphores[imageIndex];
		VkSwapchainKHR swapChain = renderer->GetSwapChain()->GetSwapChain();

		VkPresentInfoKHR presentInfo {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &waitSemaphore,
			.swapchainCount = 1,
			.pSwapchains = &swapChain,
			.pImageIndices = &imageIndex,
			.pResults = nullptr
		};

		// Present the image once rendering is finished
		result = renderer->GetLoader()->vkQueuePresentKHR(renderer->GetDevice()->GetDeviceQueues().presentQueue, &presentInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan swap chain present to queue! Error code: " + string_VkResult(result));
		
		// Update the frame index
		frameIndex = (frameIndex + 1) % renderingCommandBuffers.size();
	}

	GraphicsSystem::~GraphicsSystem() {
		// Wait for all fences and for all presenting to finish
		renderer->GetLoader()->vkWaitForFences(renderer->GetDevice()->GetDevice(), (uint32_t)renderingFences.size(), renderingFences.data(), VK_TRUE, UINT64_T_MAX);
		renderer->GetLoader()->vkQueueWaitIdle(renderer->GetDevice()->GetDeviceQueues().presentQueue);

		// Free the command buffers
		renderer->GetLoader()->vkFreeCommandBuffers(renderer->GetDevice()->GetDevice(), renderer->GetGraphicsCommandPool()->GetCommandPool(), (uint32_t)renderingCommandBuffers.size(), renderingCommandBuffers.data());

		// Destroy all sync objects
		for(VkFence renderingFence : renderingFences)
			renderer->GetLoader()->vkDestroyFence(renderer->GetDevice()->GetDevice(), renderingFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		for(VkSemaphore renderingSemaphore : renderingSemaphores)
			renderer->GetLoader()->vkDestroySemaphore(renderer->GetDevice()->GetDevice(), renderingSemaphore, &VulkanRenderer::ALLOCATION_CALLBACKS);
		for(VkSemaphore imageAvailableSemaphore : imageAvailableSemaphores)
			renderer->GetLoader()->vkDestroySemaphore(renderer->GetDevice()->GetDevice(), imageAvailableSemaphore, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}