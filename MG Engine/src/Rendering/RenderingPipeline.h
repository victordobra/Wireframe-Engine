#pragma once
#include "VulkanInclude.h"
#include "Buffer.h"
#include "Defines.h"

namespace mge {
	void InitiatePipeline();
	void ClearPipeline();
	void PipelineBind(VkCommandBuffer commandBuffer);

	VkPipelineLayout GetPipelineLayout();
	VkSampler GetSampler();
}