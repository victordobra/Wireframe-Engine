#include "ImageTexture.hpp"
#include "Main/Program.hpp"
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Internal helper functions
	void ImageTexture::CreateVulkanComponents() {
		// Exit the function if the image already has Vulkan components created
		if(image)
			return;
		
		// Set the queue family indices
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();

		uint32_t familyIndices[] = { device->GetDeviceQueues().graphicsIndex, device->GetDeviceQueues().transferIndex };
		uint32_t familyCount = 1 + (uint32_t)(familyIndices[0] != familyIndices[1]);
		VkSharingMode sharingMode = (familyCount == 1) ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
		
		// Set the image create info
		VkImageCreateInfo imageInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = imageFormat,
			.extent = { width, height, 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = sharingMode,
			.queueFamilyIndexCount = familyCount,
			.pQueueFamilyIndices = familyIndices,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};

		imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		// Create the Vulkan image
		VkResult result = device->GetLoader()->vkCreateImage(device->GetDevice(), &imageInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &image);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image! Error code: " + string_VkResult(result));
		
		// Allocate memory for the image and bind the memory
		try {
			imageMemory = device->GetAllocator()->AllocImageMemory(image, VK_IMAGE_TILING_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan image!");
		}

		result = device->GetLoader()->vkBindImageMemory(device->GetDevice(), image, imageMemory.memory, imageMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan image to its memory! Error code: " + string_VkResult(result));

		// Set the component mapping
		VkComponentMapping componentMapping;
		switch(imageFormat) {
		case VK_FORMAT_R8_UINT:
			componentMapping = {
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_R,
				.b = VK_COMPONENT_SWIZZLE_R,
				.a = VK_COMPONENT_SWIZZLE_ONE
			};
			break;
		case VK_FORMAT_R8G8_UINT:
			componentMapping = {
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_R,
				.b = VK_COMPONENT_SWIZZLE_R,
				.a = VK_COMPONENT_SWIZZLE_G
			};
			break;
		case VK_FORMAT_R8G8B8_UINT:
			componentMapping = {
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_G,
				.b = VK_COMPONENT_SWIZZLE_B,
				.a = VK_COMPONENT_SWIZZLE_ONE
			};
			break;
		case VK_FORMAT_R8G8B8A8_UINT:
			componentMapping = {
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_G,
				.b = VK_COMPONENT_SWIZZLE_B,
				.a = VK_COMPONENT_SWIZZLE_A
			};
			break;
		default:
			throw std::runtime_error("Unsupported Vulkan image format!");
		}

		// Set the image view create info
		VkImageViewCreateInfo imageViewInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags =  0,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.components = componentMapping,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		// Create the image views
		imageViewInfo.format = srgbViewFormat;
		result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &srgbImageView);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image view! Error code: " + string_VkResult(result));
		
		imageViewInfo.format = linearViewFormat;
		result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &linearImageView);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image view! Error code: " + string_VkResult(result));
	}
	void ImageTexture::DestroyVulkanComponents() {
		// Exit the function if the image doesn't have Vulkan components created
		if(!image)
			return;
		
		// Free the image's memory
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();

		device->GetAllocator()->FreeMemory(imageMemory);

		// Destroy all Vulkan components
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), srgbImageView, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), linearImageView, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyImage(device->GetDevice(), image, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Reset all variables
		width = 0; height = 0;
		image = VK_NULL_HANDLE;
		imageMemory = {};
		srgbImageView = VK_NULL_HANDLE;
		linearImageView = VK_NULL_HANDLE;
		imageFormat = VK_FORMAT_UNDEFINED;
		srgbViewFormat = VK_FORMAT_UNDEFINED;
		linearViewFormat = VK_FORMAT_UNDEFINED;
		imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}
	void ImageTexture::InternalGetImageData(void* data, VkImageLayout dstLayout) const {
		// Get the image's composition from its format
		ImageComposition composition;
		switch(imageFormat) {
		case VK_FORMAT_R8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE;
			break;
		case VK_FORMAT_R8G8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE_ALPHA;
			break;
		case VK_FORMAT_R8G8B8_UINT:
			composition = IMAGE_COMPOSITION_RGB;
			break;
		case VK_FORMAT_R8G8B8A8_UINT:
			composition = IMAGE_COMPOSITION_RGBA;
			break;
		}

		// Set the staging buffer create info
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();
		uint32_t transferFamily = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo stagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)width * height * (VkDeviceSize)composition,
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferFamily
		};

		// Create the staging buffer
		VkBuffer stagingBuffer;
		VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &stagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &stagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan staging buffer for image! Error code: " + string_VkResult(result));
		
		// Allocate memory for the staging buffer and bind the memory
		VulkanAllocator::Memory stagingBufferMemory;
		try {
			stagingBufferMemory = device->GetAllocator()->AllocBufferMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan staging buffer for image!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), stagingBuffer, stagingBufferMemory.memory, stagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan staging buffer to its memory! Error code: " + string_VkResult(result));
		
		// Write the image data to the staging buffer
		memcpy(device->GetAllocator()->GetMappedMemory(stagingBufferMemory), data, (size_t)stagingBufferInfo.size);

		// Set the fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the copy fence
		VkFence copyFence;
		result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for image copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Set the image layout transition barrier
		TransitionImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		// Transfer the buffer's contents to the image
		VkBufferImageCopy copyRegion {
			.bufferOffset = 0,
			.bufferRowLength = 0,
			.bufferImageHeight = 0,
			.imageSubresource = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
			.imageOffset = { 0, 0, 0 },
			.imageExtent = { width, height, 1 }
		};

		device->GetLoader()->vkCmdCopyImageToBuffer(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingBuffer, 1, &copyRegion);

		// Set the image layout to the given target layout
		TransitionImageLayout(commandBuffer, dstLayout);

		// End the command buffer recording
		device->GetLoader()->vkEndCommandBuffer(commandBuffer);

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &copyFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for image copy! Error code: " + string_VkResult(result));
		
		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), copyFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);

		// Destroy the staging buffer and free its memory
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), stagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(stagingBufferMemory);
	}
	void ImageTexture::InternalSetImageData(const void* data, VkImageLayout dstLayout) {
		// Get the image's composition from its format
		ImageComposition composition;
		switch(imageFormat) {
		case VK_FORMAT_R8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE;
			break;
		case VK_FORMAT_R8G8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE_ALPHA;
			break;
		case VK_FORMAT_R8G8B8_UINT:
			composition = IMAGE_COMPOSITION_RGB;
			break;
		case VK_FORMAT_R8G8B8A8_UINT:
			composition = IMAGE_COMPOSITION_RGBA;
			break;
		}

		// Set the staging buffer create info
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();
		uint32_t transferFamily = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo stagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)width * height * (VkDeviceSize)composition,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferFamily
		};

		// Create the staging buffer
		VkBuffer stagingBuffer;
		VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &stagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &stagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan staging buffer for image! Error code: " + string_VkResult(result));
		
		// Allocate memory for the staging buffer and bind the memory
		VulkanAllocator::Memory stagingBufferMemory;
		try {
			stagingBufferMemory = device->GetAllocator()->AllocBufferMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan staging buffer for image!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), stagingBuffer, stagingBufferMemory.memory, stagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan staging buffer to its memory! Error code: " + string_VkResult(result));
		
		// Write the image data to the staging buffer
		memcpy(device->GetAllocator()->GetMappedMemory(stagingBufferMemory), data, (size_t)stagingBufferInfo.size);

		// Set the fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the copy fence
		VkFence copyFence;
		result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for image copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Set the image layout transition barrier
		TransitionImageLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Transfer the buffer's contents to the image
		VkBufferImageCopy copyRegion {
			.bufferOffset = 0,
			.bufferRowLength = 0,
			.bufferImageHeight = 0,
			.imageSubresource = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
			.imageOffset = { 0, 0, 0 },
			.imageExtent = { width, height, 1 }
		};

		device->GetLoader()->vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

		// Set the image layout to be sample-optimal
		TransitionImageLayout(commandBuffer, dstLayout);

		// End the command buffer recording
		device->GetLoader()->vkEndCommandBuffer(commandBuffer);

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &copyFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for image copy! Error code: " + string_VkResult(result));
		
		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), copyFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);

		// Destroy the staging buffer and free its memory
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), stagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(stagingBufferMemory);
	}

	// Virtual function definitions
	void ImageTexture::Load(std::istream& stream) {
		// Load the image's width, height and composition
		uint8_t imageParams[9];
		if(stream.read((char*)imageParams, 9).gcount() != 9)
			throw std::runtime_error("Failed to read image parameters from stream!");
		
		width = ((uint32_t)imageParams[0] << 24) | ((uint32_t)imageParams[1] << 16) | ((uint32_t)imageParams[2] << 8) | (uint32_t)imageParams[3];
		height = ((uint32_t)imageParams[4] << 24) | ((uint32_t)imageParams[5] << 16) | ((uint32_t)imageParams[6] << 8) | (uint32_t)imageParams[7];
		ImageComposition composition = (ImageComposition)imageParams[8];

		// Check if the image's composition is valid
		if(!width || !height)
			throw std::runtime_error("Invalid image dimensions read from stream!");
		if(composition < IMAGE_COMPOSITION_GRAYSCALE || composition > IMAGE_COMPOSITION_RGBA)
			throw std::runtime_error("Invalid image composition read from stream!");

		// Allocate the image's data buffer
		size_t dataSize = width * height * (uint32_t)composition;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Read the image data from the stream
		if(stream.read((char*)imageData, dataSize).gcount() != dataSize)
			throw std::runtime_error("Failed to read image data from stream!");
		
		// Set the image's formats
		switch(composition) {
		case IMAGE_COMPOSITION_GRAYSCALE:
			imageFormat = VK_FORMAT_R8_UINT;
			srgbViewFormat = VK_FORMAT_R8_SRGB;
			linearViewFormat = VK_FORMAT_R8_UNORM;
			break;
		case IMAGE_COMPOSITION_GRAYSCALE_ALPHA:
			imageFormat = VK_FORMAT_R8G8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8_UNORM;
			break;
		case IMAGE_COMPOSITION_RGB:
			imageFormat = VK_FORMAT_R8G8B8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8B8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8B8_UNORM;
			break;
		case IMAGE_COMPOSITION_RGBA:
			imageFormat = VK_FORMAT_R8G8B8A8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8B8A8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8B8A8_UNORM;
			break;
		}

		// Create the Vulkan components for the image
		CreateVulkanComponents();

		// Set the image's data
		InternalSetImageData(imageData, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		
		// Free the image data buffer
		FreeMemory(imageData);
	}
	void ImageTexture::Save(std::ostream& stream) const {
		// Get the image's composition from its format
		ImageComposition composition;
		switch(imageFormat) {
		case VK_FORMAT_R8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE;
			break;
		case VK_FORMAT_R8G8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE_ALPHA;
			break;
		case VK_FORMAT_R8G8B8_UINT:
			composition = IMAGE_COMPOSITION_RGB;
			break;
		case VK_FORMAT_R8G8B8A8_UINT:
			composition = IMAGE_COMPOSITION_RGBA;
			break;
		}

		// Allocate the image's data buffer
		size_t dataSize = width * height * (uint32_t)composition;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Get the image's data
		InternalGetImageData(imageData, imageLayout);

		// Write the image's width, height and composition to the stream
		uint8_t imageParams[9];

		imageParams[0] = (uint8_t)(width >> 24);
		imageParams[1] = (uint8_t)(width >> 16);
		imageParams[2] = (uint8_t)(width >> 8);
		imageParams[3] = (uint8_t)width;

		imageParams[4] = (uint8_t)(height >> 24);
		imageParams[5] = (uint8_t)(height >> 16);
		imageParams[6] = (uint8_t)(height >> 8);
		imageParams[7] = (uint8_t)height;

		imageParams[8] = (uint8_t)composition;

		if(!stream.write((char*)imageParams, 9))
			throw std::runtime_error("Failed to write image parameters to stream!");
		
		// Write the image's data to the stream
		if(!stream.write((char*)imageData, dataSize))
			throw std::runtime_error("Failed to write image data to stream!");
		
		// Free the image data buffer
		FreeMemory(imageData);
	}
	void ImageTexture::Import(const std::string& path) {
		// Open the file stream for reading
		std::ifstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + path + "\" for reading!");

		// Load the image data from the stream
		uint32_t newWidth, newHeight;
		ImageComposition composition;

		uint8_t* imageData = ReadImageFile(stream, newWidth, newHeight, composition);
		if(!imageData)
			throw std::runtime_error("Failed to read image data from stream!");	
		
		// Close the stream
		stream.close();
		
		// Destroy any existing Vulkan components
		DestroyVulkanComponents();

		// Set the image's properties
		width = newWidth;
		height = newHeight;

		switch(composition) {
		case IMAGE_COMPOSITION_GRAYSCALE:
			imageFormat = VK_FORMAT_R8_UINT;
			srgbViewFormat = VK_FORMAT_R8_SRGB;
			linearViewFormat = VK_FORMAT_R8_UNORM;
			break;
		case IMAGE_COMPOSITION_GRAYSCALE_ALPHA:
			imageFormat = VK_FORMAT_R8G8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8_UNORM;
			break;
		case IMAGE_COMPOSITION_RGB:
			imageFormat = VK_FORMAT_R8G8B8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8B8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8B8_UNORM;
			break;
		case IMAGE_COMPOSITION_RGBA:
			imageFormat = VK_FORMAT_R8G8B8A8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8B8A8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8B8A8_UNORM;
			break;
		}

		// Create the Vulkan components for the image
		CreateVulkanComponents();

		// Set the image's data
		InternalSetImageData(imageData, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		
		// Free the image data buffer
		FreeMemory(imageData);
	}
	void ImageTexture::Export(const std::string& path) const {
		// Get the image's composition from its format
		ImageComposition composition;
		switch(imageFormat) {
		case VK_FORMAT_R8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE;
			break;
		case VK_FORMAT_R8G8_UINT:
			composition = IMAGE_COMPOSITION_GRAYSCALE_ALPHA;
			break;
		case VK_FORMAT_R8G8B8_UINT:
			composition = IMAGE_COMPOSITION_RGB;
			break;
		case VK_FORMAT_R8G8B8A8_UINT:
			composition = IMAGE_COMPOSITION_RGBA;
			break;
		}

		// Allocate the image's data buffer
		size_t dataSize = width * height * (uint32_t)composition;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Get the image's data
		InternalGetImageData(imageData, imageLayout);

		// Get the file's extension
		size_t dotPos = path.find_last_of('.');
		if(dotPos == std::string::npos)
			throw std::runtime_error("Image export path \"" + path + "\" does not have an extension!");

		std::string extension = path.substr(dotPos + 1);

		// Open the file stream for writing
		std::ofstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + path + "\" for writing!");
		
		// Write the image to the stream
		if(extension == "bmp") {
			if(!WriteBMPFile(stream, width, height, composition, imageData))
				throw std::runtime_error("Failed to write BMP image file \"" + path + "\"!");
		} else if(extension == "jpg" || extension == "jpeg") {
			if(!WriteJPEGFile(stream, width, height, composition, imageData))
				throw std::runtime_error("Failed to write JPEG image file \"" + path + "\"!");
		} else if(extension == "png") {
			if(!WritePNGFile(stream, width, height, composition, imageData))
				throw std::runtime_error("Failed to write PNG image file \"" + path + "\"!");
		} else {
			throw std::invalid_argument("Unsupported image file format \"" + extension + "\" for image export!");
		}

		// Close the stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}

	// Public functions
	ImageTexture::ImageTexture(Program* program, uint32_t width, uint32_t height, ImageComposition composition, const void* data, uint64_t id) : Asset(program, id), width(width), height(height) {
		// Set the image's formats
		switch(composition) {
		case IMAGE_COMPOSITION_GRAYSCALE:
			imageFormat = VK_FORMAT_R8_UINT;
			srgbViewFormat = VK_FORMAT_R8_SRGB;
			linearViewFormat = VK_FORMAT_R8_UNORM;
			break;
		case IMAGE_COMPOSITION_GRAYSCALE_ALPHA:
			imageFormat = VK_FORMAT_R8G8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8_UNORM;
			break;
		case IMAGE_COMPOSITION_RGB:
			imageFormat = VK_FORMAT_R8G8B8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8B8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8B8_UNORM;
			break;
		case IMAGE_COMPOSITION_RGBA:
			imageFormat = VK_FORMAT_R8G8B8A8_UINT;
			srgbViewFormat = VK_FORMAT_R8G8B8A8_SRGB;
			linearViewFormat = VK_FORMAT_R8G8B8A8_UNORM;
			break;
		}

		// Create the Vulkan components for the image
		CreateVulkanComponents();

		if(data) {
			// Set the image's data
			InternalSetImageData(data, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		} else {
			// Transition the image's layout
			TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
	}

	void ImageTexture::GetImageData(void* data) const {
		// Get the image's data and transition back to the original layout
		InternalGetImageData(data, imageLayout);
	}
	void ImageTexture::SetImageData(const void* data) {
		// Set the image's data and transition back to the original layout
		InternalSetImageData(data, imageLayout);
	}

	void ImageTexture::TransitionImageLayout(VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask, VkPipelineStageFlags2KHR dstStageMask) const {
		// Exit the function if the layout wouldn't be changed
		if(imageLayout == newLayout)
			return;

		// Set the fence create info
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();

		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the transition fence
		VkFence transitionFence;
		VkResult result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &transitionFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create fence for image layout transition! Error code: " + string_VkResult(result));

		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
		};

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate command buffer for image layout transition! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin command buffer for image layout transition! Error code: " + string_VkResult(result));
		
		// Transition the image layout in the command buffer
		TransitionImageLayout(commandBuffer, newLayout, srcStageMask, dstStageMask);

		// End the command buffer recording
		result = device->GetLoader()->vkEndCommandBuffer(commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to end command buffer for image layout transition! Error code: " + string_VkResult(result));
		
		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.flags = 0,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, transitionFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for image layout transition! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &transitionFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for image layout transition! Error code: " + string_VkResult(result));
		
		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), transitionFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);
	}
	void ImageTexture::TransitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask, VkPipelineStageFlags2KHR dstStageMask) const {
		// Set the image memory barrier
		VkImageMemoryBarrier2KHR imageMemoryBarrier {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR,
			.pNext = nullptr,
			.srcStageMask = srcStageMask,
			.srcAccessMask = VK_ACCESS_2_MEMORY_READ_BIT_KHR | VK_ACCESS_2_MEMORY_WRITE_BIT_KHR,
			.dstStageMask = dstStageMask,
			.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT_KHR | VK_ACCESS_2_MEMORY_WRITE_BIT_KHR,
			.oldLayout = imageLayout,
			.newLayout = newLayout,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = image,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		// Set the dependency info
		VkDependencyInfoKHR dependencyInfo {
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR,
			.pNext = nullptr,
			.dependencyFlags = 0,
			.memoryBarrierCount = 0,
			.pMemoryBarriers = nullptr,
			.bufferMemoryBarrierCount = 0,
			.pBufferMemoryBarriers = nullptr,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &imageMemoryBarrier
		};

		// Record the pipeline barrier to the given command buffer
		GetProgram()->GetRenderer()->GetLoader()->vkCmdPipelineBarrier2KHR(commandBuffer, &dependencyInfo);

		// Set the layout's new value
		imageLayout = newLayout;
	}
}