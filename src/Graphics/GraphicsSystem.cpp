#include "GraphicsSystem.hpp"
#include <future>
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Static record callbacks
	static void RecordLayoutTransitionCallback(void* userData, VkCommandBuffer commandBuffer) { }

	// Public functions
	GraphicsSystem::GraphicsSystem(VulkanRenderer* renderer, size_t maxFramesInFlight) : renderer(renderer) {
		// Create the rendering commands
		renderingCommands.resize(maxFramesInFlight);
		for(size_t i = 0; i != maxFramesInFlight; ++i)
			renderingCommands[i] = new VulkanCommand(renderer->GetDevice(), VulkanCommand::COMMAND_TYPE_GRAPHICS);

		// Set the rendering fence create info
		VkFenceCreateInfo renderingFenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		// Create the rendering fences
		renderingFences.resize(maxFramesInFlight);
		for(size_t i = 0; i != maxFramesInFlight; ++i) {
			VkResult result = renderer->GetLoader()->vkCreateFence(renderer->GetDevice()->GetDevice(), &renderingFenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &renderingFences[i]);
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
			VkResult result = renderer->GetLoader()->vkCreateSemaphore(renderer->GetDevice()->GetDevice(), &semaphoreInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &imageAvailableSemaphores[i]);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan image available semaphore! Error code: " + string_VkResult(result));
		}

		// Create the rendering semaphores
		renderingSemaphores.resize(renderer->GetSwapChain()->GetSwapChainImages().size());
		for(size_t i = 0; i != renderingSemaphores.size(); ++i) {
			VkResult result = renderer->GetLoader()->vkCreateSemaphore(renderer->GetDevice()->GetDevice(), &semaphoreInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &renderingSemaphores[i]);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan rendering semaphore! Error code: " + string_VkResult(result));
		}
	}

	VulkanCommand::CommandStageInfo GraphicsSystem::GetPresentLayoutTransitionStageInfo() {
		return VulkanCommand::CommandStageInfo {
			.name = "PresentLayoutTransition",
			.dependencies = { },
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_SWAP_CHAIN_IMAGE,
					.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR,
					.accessMask = VK_ACCESS_2_MEMORY_READ_BIT_KHR | VK_ACCESS_2_MEMORY_WRITE_BIT_KHR,
					.swapChainImageAccessInfo = {
						.swapChainImage = &renderer->GetSwapChain()->GetSwapChainImages()[imageIndex],
						.layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
					}
				}
			},
			.recordCallback = RecordLayoutTransitionCallback,
			.userData = nullptr
		};
	}

	bool GraphicsSystem::InitCommand() {
		// Skip the current frame if the swap chain does not exist
		if(!renderer->GetSwapChain()->GetSwapChain())
			return false;

		// Wait for the last frame in flight to finish rendering
		VkResult result = renderer->GetLoader()->vkWaitForFences(renderer->GetDevice()->GetDevice(), 1, &renderingFences[frameIndex], VK_TRUE, UINT64_T_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan rendering fence! Error code: " + string_VkResult(result));
		
		// Reset the rendering fence
		result = renderer->GetLoader()->vkResetFences(renderer->GetDevice()->GetDevice(), 1, &renderingFences[frameIndex]);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to reset Vulkan rendering fence! Error code: " + string_VkResult(result));

		// Acquire the next swap chain image
		result = renderer->GetLoader()->vkAcquireNextImageKHR(renderer->GetDevice()->GetDevice(), renderer->GetSwapChain()->GetSwapChain(), UINT64_T_MAX, imageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, &imageIndex);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to acquire next Vulkan swap chain image! Error code: " + string_VkResult(result));
		
		// Reset the current command 
		renderingCommands[frameIndex]->Reset();
	
		return true;
	}
	void GraphicsSystem::SubmitCommand() {
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
			.commandBuffer = renderingCommands[frameIndex]->GetCommandBuffer(),
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
		renderer->GetDevice()->GetDeviceQueues().graphicsQueueMutex.lock();
		VkResult result = renderer->GetLoader()->vkQueueSubmit2KHR(renderer->GetDevice()->GetDeviceQueues().graphicsQueue, 1, &submitInfo, renderingFences[frameIndex]);
		renderer->GetDevice()->GetDeviceQueues().graphicsQueueMutex.unlock();
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan rendering command buffer! Error code: " + string_VkResult(result));
		
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
		frameIndex = (frameIndex + 1) % renderingCommands.size();
	}

	GraphicsSystem::~GraphicsSystem() {
		// Wait for all fences and for all presenting to finish
		renderer->GetLoader()->vkWaitForFences(renderer->GetDevice()->GetDevice(), (uint32_t)renderingFences.size(), renderingFences.data(), VK_TRUE, UINT64_T_MAX);
		renderer->GetLoader()->vkQueueWaitIdle(renderer->GetDevice()->GetDeviceQueues().presentQueue);

		// Destroy all objects
		for(VulkanCommand* renderingCommand : renderingCommands)
			delete renderingCommand;
		for(VkFence renderingFence : renderingFences)
			renderer->GetLoader()->vkDestroyFence(renderer->GetDevice()->GetDevice(), renderingFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		for(VkSemaphore imageAvailableSemaphore : imageAvailableSemaphores)
			renderer->GetLoader()->vkDestroySemaphore(renderer->GetDevice()->GetDevice(), imageAvailableSemaphore, &VulkanRenderer::ALLOCATION_CALLBACKS);
		for(VkSemaphore renderingSemaphore : renderingSemaphores)
			renderer->GetLoader()->vkDestroySemaphore(renderer->GetDevice()->GetDevice(), renderingSemaphore, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}