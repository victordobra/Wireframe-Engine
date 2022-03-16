#pragma once
#include "VulkanInclude.h"
#include "Defines.h"
#include "Buffer.h"
#include "Image.h"

#include <vector>

namespace mge {
	const size_t maxMaterialCount = 10;
	const size_t targetImageSize = 1024;

	void InitiateDescriptorPool();
	void ClearDescriptorPool();

	Buffer** GetCameraBuffers();
	Buffer** GetLightingBuffers();
	Buffer** GetMaterialBuffers();
	Image** GetDiffuseImages();

	std::vector<VkDescriptorSetLayout> GetDescriptorLayouts();
	std::vector<VkDescriptorSet> GetDescriptorSets();
}