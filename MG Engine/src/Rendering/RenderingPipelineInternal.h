#pragma once
#include <vulkan/vulkan.hpp>

namespace RPipeline {
	struct PipelineConfigInfo {
		VkViewport Viewport;
		VkRect2D Scissor;
		VkPipelineViewportStateCreateInfo ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		unsigned int Subpass = 0;
	};
	void DefaultPipelineConfigInfo(size_t Width, size_t Height, PipelineConfigInfo& ConfigInfo);

	void InitPipeline(const PipelineConfigInfo& ConfInfo);
	void ClearPipeline();
}