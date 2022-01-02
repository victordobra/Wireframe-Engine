#pragma once
#include "VulkanInclude.h"

namespace RPipeline {
	struct PipelineConfigInfo {
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkViewport Viewport{};
		VkRect2D Scissor{};
		VkPipelineViewportStateCreateInfo ViewportInfo{};
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo RasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo MultisampleInfo{};
		VkPipelineColorBlendAttachmentState ColorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo{};
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		unsigned int Subpass = 0;
	};
	void DefaultPipelineConfigInfo(size_t Width, size_t Height, PipelineConfigInfo& ConfigInfo);

	void InitPipeline();
	void ClearPipeline();
}