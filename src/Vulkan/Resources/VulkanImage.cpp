#include "VulkanImage.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
	VulkanImage::VulkanImage(VulkanDevice* device, VkImageCreateFlags flags, VkImageType imageType, VkFormat format, VkExtent3D extent, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageAspectFlags aspectFlags, VkImageUsageFlags usage, VkSharingMode sharingMode, VulkanDevice::QueueTypeMask queueTypes, VkMemoryPropertyFlags memoryProperties) : device(device), imageType(imageType), format(format), extent(extent), mipLevels(mipLevels), arrayLayers(arrayLayers), tiling(tiling), aspectFlags(aspectFlags) {
		// Get the queue family indices
		uint32_t indices[4], indexCount = 0;
		if(sharingMode == VK_SHARING_MODE_CONCURRENT)
			device->GetQueueFamilyIndices(queueTypes, indices, indexCount);

		// Set the image ecreate info
		VkImageCreateInfo imageInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = flags,
			.imageType = imageType,
			.format = format,
			.extent = extent,
			.mipLevels = mipLevels,
			.arrayLayers = arrayLayers,
			.samples = samples,
			.tiling = tiling,
			.usage = usage,
			.sharingMode = sharingMode,
			.queueFamilyIndexCount = indexCount,
			.pQueueFamilyIndices = indices
		};

		// Create the image
		VkResult result = device->GetLoader()->vkCreateImage(device->GetDevice(), &imageInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &image);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image! Error code: " + string_VkResult(result));

		// Allocate and bind the image's memory
		try {
			imageMemory = device->GetAllocator()->AllocImageMemory(image, tiling, memoryProperties);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan image!");
		}

		result = device->GetLoader()->vkBindImageMemory(device->GetDevice(), image, imageMemory.memory, imageMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan image to its memory! Error code: " + string_VkResult(result));
	}

	VulkanImage::~VulkanImage() {
		// Destroy the image and free its memory
		device->GetLoader()->vkDestroyImage(device->GetDevice(), image, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(imageMemory);
	}
}