#pragma once
#include "VulkanInclude.h"

namespace mge {
	void InitiatePipeline();
	void ClearPipeline();
	void PipelineBind(VkCommandBuffer commandBuffer);

	VkPipelineLayout GetPipelineLayout();
}