#pragma once
#include <vulkan/vulkan.hpp>

namespace Vulkan {
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	struct QueueFamilyIndices {
		uint32_t GraphicsFamily;
		uint32_t PresentFamily;
		bool GraphicsFamilyHasValue = false;
		bool PresentFamilyHasValue = false;
		bool IsComplete() { return GraphicsFamilyHasValue && PresentFamilyHasValue; }
	};

#ifdef NDEBUG
	const bool EnableValidationLayers = false;
#else
	const bool EnableValidationLayers = false;
#endif

	VkCommandPool GetCommandPool();
	VkDevice GetDevice();
	VkSurfaceKHR GetSurface();
	VkQueue GetGraphicsQueue();
	VkQueue GetPresentQueue();
	VkPhysicalDeviceProperties& GetPhysicalDeviceProperties();

	SwapChainSupportDetails GetSwapChainSupport();
	unsigned int FindMemoryType(unsigned int TypeFilter, VkMemoryPropertyFlags Properties);
	QueueFamilyIndices FindPhysicalQueueFamilies();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& Candidates, VkImageTiling Tiling, VkFormatFeatureFlags Features);

	void CreateBuffer(VkDeviceSize Size, VkBufferUsageFlags Usage, VkMemoryPropertyFlags Properties, VkBuffer& Buffer, VkDeviceMemory& BufferMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer CommandBuffer);
	void CopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize Size);
	void CopyBufferToImage(VkBuffer Buffer, VkImage Image, size_t Width, size_t Height, size_t LayerCount);

	void CreateImageWithInfo(const VkImageCreateInfo &ImageInfo, VkMemoryPropertyFlags Properties, VkImage& Image, VkDeviceMemory& ImageMemory);
}