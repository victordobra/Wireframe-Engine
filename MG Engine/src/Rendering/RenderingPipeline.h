#pragma once
#include "VulkanInclude.h"
#include "Descriptors.h"
#include "Buffer.h"
#include "Defines.h"

namespace mge {
	void InitiatePipeline();
	void ClearPipeline();
	void PipelineBind(VkCommandBuffer commandBuffer);

	VkPipelineLayout GetPipelineLayout();
	DescriptorPool* GetGlobalPool();
	DescriptorSetLayout* GetGlobalSetLayout();
	std::vector<VkDescriptorSet> GetDescriptorSets();
	std::vector<Buffer*> GetUniformBuffers();
}