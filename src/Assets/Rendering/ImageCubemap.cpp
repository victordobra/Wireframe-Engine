#include "ImageCubemap.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include "Main/Program.hpp"
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	const uint32_t CUBEMAP_UNWRAPPED_STARTS[][2] = {
		{ 2, 1 }, // +X
		{ 0, 1 }, // -X
		{ 1, 2 }, // +Y
		{ 1, 0 }, // -Y
		{ 3, 1 }, // +Z
		{ 1, 1 }  // -Z
	};
	const bool CUBEMAP_UNWRAPPED_FILPPED[] = {
		true,  // +X (flipped horizontally and vertically)
		true,  // -X (flipped horizontally and vertically)
		false, // +Y (not flipped)
		false, // -Y (not flipped)
		true,  // +Z (flipped horizontally and vertically)
		true   // -Z (flipped horizontally and vertically)
	};

	// Internal helper functions
	void ImageCubemap::CreateVulkanComponents() {
		// Exit the function if the cubemap already has Vulkan components created
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
			.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT | VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_UINT,
			.extent = { width, height, 1 },
			.mipLevels = 1,
			.arrayLayers = 6,
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

		// Set the image view create info
		VkImageViewCreateInfo imageViewInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags =  0,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_CUBE,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_G,
				.b = VK_COMPONENT_SWIZZLE_B,
				.a = VK_COMPONENT_SWIZZLE_A
			},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 6
			}
		};

		// Create the image views
		imageViewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &srgbImageView);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image view! Error code: " + string_VkResult(result));
		
		imageViewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &linearImageView);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image view! Error code: " + string_VkResult(result));
	}
	void ImageCubemap::DestroyVulkanComponents() {
		// Exit the function if the cubemap doesn't have Vulkan components created
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
		imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}
	void ImageCubemap::InternalGetImageData(void* data, VkImageLayout dstLayout) const {
		// Set the staging buffer create info
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();
		uint32_t transferFamily = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo stagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)width * height * 24,
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
				.layerCount = 6
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
		
		// Read the image data from the staging buffer
		memcpy(data, device->GetAllocator()->GetMappedMemory(stagingBufferMemory), (size_t)stagingBufferInfo.size);

		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), copyFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), GetProgram()->GetRenderer()->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);

		// Destroy the staging buffer and free its memory
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), stagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(stagingBufferMemory);
	}
	void ImageCubemap::InternalSetImageData(const void* data, VkImageLayout dstLayout) {
		// Set the staging buffer create info
		VulkanDevice* device = GetProgram()->GetRenderer()->GetDevice();
		uint32_t transferFamily = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo stagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = (VkDeviceSize)width * height * 24,
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
				.layerCount = 6
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
	void ImageCubemap::Load() {
		// Destroy any existing Vulkan components
		DestroyVulkanComponents();

		// Open the file stream for reading
		std::ifstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for reading!");

		// Load the image's width and height
		width = BinaryReadUint32(stream);
		height = BinaryReadUint32(stream);

		// Allocate the image's data buffer
		size_t dataSize = width * height * 24;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Read the image data from the stream
		stream.read((char*)imageData, dataSize);
		
		// Create the Vulkan components for the cubemap
		CreateVulkanComponents();

		// Set the image's data
		InternalSetImageData(imageData, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		
		// Close the file stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}
	void ImageCubemap::Save() const {
		// Open the file stream for writing
		std::ofstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for writing!");
		
		// Allocate the image's data buffer
		size_t dataSize = width * height * 24;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Get the image's data
		InternalGetImageData(imageData, imageLayout);

		// Write the image's width and height to the stream
		BinaryWriteUint32(stream, width);
		BinaryWriteUint32(stream, height);
		
		// Write the image's data to the stream
		stream.write((const char*)imageData, dataSize);

		// Close the file stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}
	void ImageCubemap::Import() {
		// Destroy any existing Vulkan components
		DestroyVulkanComponents();

		// Open the file stream for reading
		std::ifstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for reading!");

		// Load the unwrapped image data from the stream
		uint32_t unwrappedWidth, unwrappedHeight;

		uint8_t* unwrappedData = ReadImageFile(stream, unwrappedWidth, unwrappedHeight);
		if(!unwrappedData)
			throw std::runtime_error("Failed to read image data from stream!");	

		// Close the stream
		stream.close();

		// Set the cubemap's properties
		width = unwrappedWidth / 4;
		height = unwrappedHeight / 3;

		// Allocate the cubemap's data buffer
		size_t dataSize = width * height * 24;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Copy the unwrapped image to the cubemap's data buffer
		for(uint32_t l = 0; l != 6; ++l) {
			// Get the range start and end for the current layer
			uint32_t leftX = CUBEMAP_UNWRAPPED_STARTS[l][0] * width;
			uint32_t downY = CUBEMAP_UNWRAPPED_STARTS[l][1] * height;

			if(CUBEMAP_UNWRAPPED_FILPPED[l]) {
				// Copy the unwrapped image data to the cubemap's data buffer, flipping it horizontally and vertically
				for(uint32_t y = 0; y != height; ++y) {
					for(uint32_t x = 0; x != width; ++x) {
						// Calculate the indices in the unwrapped image data and the cubemap's data buffer
						uint32_t srcX = leftX + (width - 1 - x);
						uint32_t srcY = downY + (height - 1 - y);
						uint32_t srcIndex = (srcY * unwrappedWidth + srcX) * 4;
						uint32_t dstIndex = (l * width * height + y * width + x) * 4;

						// Copy the pixel data from the unwrapped image to the cubemap's data buffer
						imageData[dstIndex] = unwrappedData[srcIndex];
						imageData[dstIndex + 1] = unwrappedData[srcIndex + 1];
						imageData[dstIndex + 2] = unwrappedData[srcIndex + 2];
						imageData[dstIndex + 3] = unwrappedData[srcIndex + 3];
					}
				}
			} else {
				// Copy the unwrapped image data to the cubemap's data buffer without flipping
				for(uint32_t y = 0; y != height; ++y) {
					for(uint32_t x = 0; x != width; ++x) {
						// Calculate the indices in the unwrapped image data and the cubemap's data buffer
						uint32_t srcX = leftX + x;
						uint32_t srcY = downY + y;
						uint32_t srcIndex = (srcY * unwrappedWidth + srcX) * 4;
						uint32_t dstIndex = (l * width * height + y * width + x) * 4;

						// Copy the pixel data from the unwrapped image to the cubemap's data buffer
						imageData[dstIndex] = unwrappedData[srcIndex];
						imageData[dstIndex + 1] = unwrappedData[srcIndex + 1];
						imageData[dstIndex + 2] = unwrappedData[srcIndex + 2];
						imageData[dstIndex + 3] = unwrappedData[srcIndex + 3];
					}
				}
			}
		}

		// Free the unwrapped image data buffer
		FreeMemory(unwrappedData);

		// Create the Vulkan components for the cubemap
		CreateVulkanComponents();

		// Set the image's data
		InternalSetImageData(imageData, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		
		// Close the file stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}
	void ImageCubemap::Export() const {
		// Allocate the image's data buffer
		size_t dataSize = width * height * 24;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Get the image's data
		InternalGetImageData(imageData, imageLayout);

		// Set the unwrapped image's properties
		uint32_t unwrappedWidth = width * 4;
		uint32_t unwrappedHeight = height * 3;

		// Allocate the unwrapped image's data buffer
		size_t unwrappedDataSize = unwrappedWidth * unwrappedHeight * 4;
		uint8_t* unwrappedData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Reset the unwrapped image data to zero
		memset(unwrappedData, 0, unwrappedDataSize);

		// Copy the cubemap's data buffer to the unwrapped image
		for(uint32_t l = 0; l != 6; ++l) {
			// Get the range start and end for the current layer
			uint32_t leftX = CUBEMAP_UNWRAPPED_STARTS[l][0] * width;
			uint32_t downY = CUBEMAP_UNWRAPPED_STARTS[l][1] * height;

			if(CUBEMAP_UNWRAPPED_FILPPED[l]) {
				// Copy the cubemap's data buffer to the unwrapped image, flipping it horizontally and vertically
				for(uint32_t y = 0; y != height; ++y) {
					for(uint32_t x = 0; x != width; ++x) {
						// Calculate the indices in the unwrapped image data and the cubemap's data buffer
						uint32_t dstX = leftX + (width - 1 - x);
						uint32_t dstY = downY + (height - 1 - y);
						uint32_t srcIndex = (l * width * height + y * width + x) * 4;
						uint32_t dstIndex = (dstY * unwrappedWidth + dstX) * 4;

						// Copy the pixel data from the unwrapped image to the cubemap's data buffer
						unwrappedData[dstIndex] = imageData[srcIndex];
						unwrappedData[dstIndex + 1] = imageData[srcIndex + 1];
						unwrappedData[dstIndex + 2] = imageData[srcIndex + 2];
						unwrappedData[dstIndex + 3] = imageData[srcIndex + 3];
					}
				}
			} else {
				// Copy the cubemap's data buffer to the unwrapped image without flipping
				for(uint32_t y = 0; y != height; ++y) {
					for(uint32_t x = 0; x != width; ++x) {
						// Calculate the indices in the unwrapped image data and the cubemap's data buffer
						uint32_t dstX = leftX + x;
						uint32_t dstY = downY + y;
						uint32_t srcIndex = (l * width * height + y * width + x) * 4;
						uint32_t dstIndex = (dstY * unwrappedWidth + dstX) * 4;

						// Copy the pixel data from the unwrapped image to the cubemap's data buffer
						unwrappedData[dstIndex] = imageData[srcIndex];
						unwrappedData[dstIndex + 1] = imageData[srcIndex + 1];
						unwrappedData[dstIndex + 2] = imageData[srcIndex + 2];
						unwrappedData[dstIndex + 3] = imageData[srcIndex + 3];
					}
				}
			}
		}

		// Get the file's extension
		std::string extension = GetPath().extension().string();

		// Open the file stream for writing
		std::ofstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for writing!");
		
		// Write the unwrapped image to the stream
		if(extension == ".bmp") {
			if(!WriteBMPFile(stream, unwrappedWidth, unwrappedHeight, unwrappedData))
				throw std::runtime_error("Failed to write BMP image file \"" + GetPath().string() + "\"!");
		} else if(extension == ".jpg" || extension == ".jpeg") {
			if(!WriteJPEGFile(stream, unwrappedWidth, unwrappedHeight, unwrappedData))
				throw std::runtime_error("Failed to write JPEG image file \"" + GetPath().string() + "\"!");
		} else if(extension == ".png") {
			if(!WritePNGFile(stream, unwrappedWidth, unwrappedHeight, unwrappedData))
				throw std::runtime_error("Failed to write PNG image file \"" + GetPath().string() + "\"!");
		} else {
			throw std::invalid_argument("Unsupported image file format \"" + extension + "\" for cubemap export!");
		}

		// Close the stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}

	// Public functions
	ImageCubemap::ImageCubemap(Program* program, uint32_t width, uint32_t height, const void* data, uint64_t id, const std::filesystem::path& path) : Asset(program, id, path), width(width), height(height) {
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

	void ImageCubemap::GetImageData(void* data) const {
		// Get the image's data and transition back to the original layout
		InternalGetImageData(data, imageLayout);
	}
	void ImageCubemap::SetImageData(const void* data) {
		// Set the image's data and transition back to the original layout
		InternalSetImageData(data, imageLayout);
	}

	void ImageCubemap::TransitionImageLayout(VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask, VkPipelineStageFlags2KHR dstStageMask) const {
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
	void ImageCubemap::TransitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask, VkPipelineStageFlags2KHR dstStageMask) const {
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
				.layerCount = 6
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