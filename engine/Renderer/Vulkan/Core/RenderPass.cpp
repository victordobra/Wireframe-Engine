#include "RenderPass.hpp"
#include "Allocator.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Variables
	VkRenderPass renderPass;

	// Public functions
	void CreateVulkanRenderPass() {
		// Set the attachment descriptions
		VkAttachmentDescription attachemnts[2];

		// Set the color attachment description
		attachemnts[0].flags = 0;
		attachemnts[0].format = GetVulkanSwapChainSettings().imageFormat;
		attachemnts[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachemnts[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachemnts[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachemnts[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachemnts[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachemnts[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachemnts[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Set the depth attachemnt description
		attachemnts[1].flags = 0;
		attachemnts[1].format = GetVulkanSwapChainDepthFormat();
		attachemnts[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachemnts[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachemnts[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachemnts[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachemnts[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachemnts[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachemnts[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Set the color attachment reference
		VkAttachmentReference colorAttachmentRef;

		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Set the depth attachment reference
		VkAttachmentReference depthAttachemntRef;

		depthAttachemntRef.attachment = 1;
		depthAttachemntRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Set the subpass description
		VkSubpassDescription subpass;

		subpass.flags = 0;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pResolveAttachments = nullptr;
		subpass.pDepthStencilAttachment = &depthAttachemntRef;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		// Set the render pass create info
		VkRenderPassCreateInfo createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.attachmentCount = 2;
		createInfo.pAttachments = attachemnts;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 0;
		createInfo.pDependencies = nullptr;

		// Create the render pass
		VkResult result = vkCreateRenderPass(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &renderPass);
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Vulkan render pass! Error code: %s", string_VkResult(result));
	}
	void DestroyVulkanRenderPass() {
		// Destroy the render pass
		vkDestroyRenderPass(GetVulkanDevice(), renderPass, GetVulkanAllocCallbacks());
	}
	void RecreateVulkanRenderPass() {
		// Destroy and create the render pass
		DestroyVulkanRenderPass();
		CreateVulkanRenderPass();
	}

	VkRenderPass GetVulkanRenderPass() {
		return renderPass;
	}
}