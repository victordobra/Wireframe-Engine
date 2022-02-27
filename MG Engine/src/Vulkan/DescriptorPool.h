#pragma once
#include "VulkanInclude.h"
#include "Defines.h"
#include "Buffer.h"

#include <vector>

namespace mge {
	const size_t maxMaterialCount = 20;

	void InitiateDescriptorPool();
	void ClearDescriptorPool();

	Buffer** GetCameraBuffers();
	Buffer** GetLightingBuffers();
	Buffer** GetMaterialBuffers();

	std::vector<VkDescriptorSetLayout> GetDescriptorLayouts();
	std::vector<VkDescriptorSet> GetDescriptorSets();
}