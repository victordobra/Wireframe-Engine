#include "ImageTexture.hpp"
#include "Core/Parsers/Image/ImageParser.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include "Main/Program.hpp"
#include "Vulkan/Resources/VulkanBuffer.hpp"
#include "Vulkan/Resources/VulkanCommand.hpp"
#include <cstring>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Static record calalbacks
	struct TransferUserData {
		VulkanBuffer* stagingBuffer;
		VulkanImage* image;
	};

	static void RecordGetImageDataCallback(void* userData, VkCommandBuffer commandBuffer) {
		// Get the buffer and image
		TransferUserData* transferData = (TransferUserData*)userData;
		VulkanBuffer* stagingBuffer = transferData->stagingBuffer;
		VulkanImage* image = transferData->image;

		// Set the buffer-image copy info
		VkBufferImageCopy bufferImageCopy {
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
			.imageExtent = image->GetImageExtent()
		};

		// Record the transfer
		image->GetDevice()->GetLoader()->vkCmdCopyImageToBuffer(commandBuffer, image->GetImage(), image->GetImageLayout(), stagingBuffer->GetBuffer(), 1, &bufferImageCopy);
	}
	static void RecordSetImageDataCallback(void* userData, VkCommandBuffer commandBuffer) {
		// Get the buffer and image
		TransferUserData* transferData = (TransferUserData*)userData;
		VulkanBuffer* stagingBuffer = transferData->stagingBuffer;
		VulkanImage* image = transferData->image;

		// Set the buffer-image copy info
		VkBufferImageCopy bufferImageCopy {
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
			.imageExtent = image->GetImageExtent()
		};

		// Record the transfer
		image->GetDevice()->GetLoader()->vkCmdCopyBufferToImage(commandBuffer, stagingBuffer->GetBuffer(), image->GetImage(), image->GetImageLayout(), 1, &bufferImageCopy);
	}

	// Virtual function definitions
	void ImageTexture::Load() {
		// Destroy the previous image
		delete image;

		// Open the file stream for reading
		std::ifstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for reading!");

		// Load the image's width and height
		uint32_t width = BinaryReadUint32BE(stream);
		uint32_t height = BinaryReadUint32BE(stream);

		// Allocate the image's data buffer
		size_t dataSize = width * height * 4;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Read the image data from the stream
		stream.read((char*)imageData, dataSize);
		
		// Create the Vulkan components for the image
		image = new VulkanImage(
			GetProgram()->GetRenderer()->GetDevice(),
			VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
			VK_IMAGE_TYPE_2D,
			VK_FORMAT_R8G8B8A8_UINT,
			{ width, height, 1 }, 1, 1,
			VK_SAMPLE_COUNT_1_BIT,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_SHARING_MODE_CONCURRENT,
			VulkanDevice::QUEUE_TYPE_GRAPHICS | VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		// Set the image's data
		SetImageData(imageData);
		
		// Close the file stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}
	void ImageTexture::Save() const {
		// Open the file stream for writing
		std::ofstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for writing!");
		
		// Allocate the image's data buffer
		size_t dataSize = image->GetImageExtent().width * image->GetImageExtent().height * 4;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Get the image's data
		GetImageData(imageData);

		// Write the image's width and height to the stream
		BinaryWriteUint32BE(stream, image->GetImageExtent().width);
		BinaryWriteUint32BE(stream, image->GetImageExtent().height);
		
		// Write the image's data to the stream
		stream.write((const char*)imageData, dataSize);

		// Close the file stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}
	void ImageTexture::Import() {
		// Destroy the previous image
		delete image;

		// Open the file stream for reading
		std::ifstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for reading!");

		// Load the image data from the stream
		uint32_t width, height;

		uint8_t* imageData = ReadImageFile(stream, width, height);
		if(!imageData)
			throw std::runtime_error("Failed to read image data from stream!");	
		
		// Close the stream
		stream.close();
		
		// Create the Vulkan components for the image
		image = new VulkanImage(
			GetProgram()->GetRenderer()->GetDevice(),
			VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
			VK_IMAGE_TYPE_2D,
			VK_FORMAT_R8G8B8A8_UINT,
			{ width, height, 1 }, 1, 1,
			VK_SAMPLE_COUNT_1_BIT,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_SHARING_MODE_CONCURRENT,
			VulkanDevice::QUEUE_TYPE_GRAPHICS | VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		// Set the image's data
		SetImageData(imageData);
		
		// Free the image data buffer
		FreeMemory(imageData);
	}
	void ImageTexture::Export() const {
		// Allocate the image's data buffer
		size_t dataSize = image->GetImageExtent().width * image->GetImageExtent().height * 4;
		uint8_t* imageData = (uint8_t*)AllocMemory(dataSize);
		if(!imageData)
			throw std::bad_alloc();
		
		// Get the image's data
		GetImageData(imageData);

		// Get the file's extension
		std::string extension = GetPath().extension().string();

		// Open the file stream for writing
		std::ofstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open image file \"" + GetPath().string() + "\" for writing!");
		
		// Write the image to the stream
		if(extension == ".png") {
			if(!WritePNGFile(stream, image->GetImageExtent().width, image->GetImageExtent().height, imageData))
				throw std::runtime_error("Failed to write PNG image file \"" + GetPath().string() + "\"!");
		} else if(extension == ".tga") {
			if(!WriteTGAFile(stream, image->GetImageExtent().width, image->GetImageExtent().height, imageData))
				throw std::runtime_error("Failed to write TGA image file \"" + GetPath().string() + "\"!");
		} else {
			throw std::invalid_argument("Unsupported image file format \"" + extension + "\" for image export!");
		}

		// Close the stream and free the image data buffer
		stream.close();
		FreeMemory(imageData);
	}

	// Public functions
	ImageTexture::ImageTexture(Program* program, uint64_t id, const std::filesystem::path& path) : Asset(program, id, path), image(nullptr) { }
	ImageTexture::ImageTexture(Program* program, uint32_t width, uint32_t height, const void* data, uint64_t id, const std::filesystem::path& path) : Asset(program, id, path) {
		// Create the Vulkan components for the image
		image = new VulkanImage(
			GetProgram()->GetRenderer()->GetDevice(),
			VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
			VK_IMAGE_TYPE_2D,
			VK_FORMAT_R8G8B8A8_UINT,
			{ width, height, 1 }, 1, 1,
			VK_SAMPLE_COUNT_1_BIT,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_SHARING_MODE_CONCURRENT,
			VulkanDevice::QUEUE_TYPE_GRAPHICS | VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		// Set the image's data, if given
		if(data)
			SetImageData(data);
	}

	void ImageTexture::GetImageData(void* data) const {
		// Create the staging buffer
		VkDeviceSize dataSize = (VkDeviceSize)image->GetImageExtent().width * image->GetImageExtent().height * 4;
		VulkanBuffer stagingBuffer {
			GetProgram()->GetRenderer()->GetDevice(),
			dataSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Set the command stage info
		TransferUserData transferData {
			.stagingBuffer = &stagingBuffer,
			.image = image
		};

		VulkanCommand::CommandStageInfo transferStageInfo {
			.name = "GetImageDataTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
					.bufferAccessInfo = {
						.buffer = &stagingBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_IMAGE,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
					.imageAccessInfo = {
						.image = image,
						.layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
					}
				}
			},
			.recordCallback = RecordGetImageDataCallback,
			.userData = &transferData
		};

		// Create and record the command
		VulkanCommand transferCommand { GetProgram()->GetRenderer()->GetDevice(), VulkanCommand::COMMAND_TYPE_TRANSFER };

		transferCommand.AddCommandStage(std::move(transferStageInfo));
		transferCommand.RecordCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		// Set the transfer fence create info
		VkFenceCreateInfo transferFenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the transfer fence 
		VkFence transferFence;
		VkResult result = GetProgram()->GetRenderer()->GetLoader()->vkCreateFence(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), &transferFenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &transferFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image transfer fence! Error code: " + string_VkResult(result));

		// Set the submit info
		VkCommandBufferSubmitInfoKHR commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.commandBuffer = transferCommand.GetCommandBuffer(),
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

		// Submit the command
		GetProgram()->GetRenderer()->GetDevice()->GetDeviceQueues().transferQueueMutex.lock();
		result = GetProgram()->GetRenderer()->GetLoader()->vkQueueSubmit2KHR(GetProgram()->GetRenderer()->GetDevice()->GetDeviceQueues().transferQueue, 1, &submitInfo, transferFence);
		GetProgram()->GetRenderer()->GetDevice()->GetDeviceQueues().transferQueueMutex.unlock();

		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan image transfer command! Error code: " + string_VkResult(result));

		// Wait for the transfer fence
		result = GetProgram()->GetRenderer()->GetLoader()->vkWaitForFences(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), 1, &transferFence, VK_TRUE, UINT64_T_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan image transfer fence! Error code: " + string_VkResult(result));
		
		// Copy the data to the given pointer
		void* mappedData = GetProgram()->GetRenderer()->GetDevice()->GetAllocator()->GetMappedMemory(stagingBuffer.GetBufferMemory());
		std::memcpy(data, mappedData, dataSize);

		// Destroy the fence
		GetProgram()->GetRenderer()->GetLoader()->vkDestroyFence(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), transferFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
	void ImageTexture::SetImageData(const void* data) {
		// Create the staging buffer
		VkDeviceSize dataSize = (VkDeviceSize)image->GetImageExtent().width * image->GetImageExtent().height * 4;
		VulkanBuffer stagingBuffer {
			GetProgram()->GetRenderer()->GetDevice(),
			dataSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Copy the data to the staging buffer
		void* mappedData = GetProgram()->GetRenderer()->GetDevice()->GetAllocator()->GetMappedMemory(stagingBuffer.GetBufferMemory());
		std::memcpy(mappedData, data, dataSize);

		// Set the command stage info
		TransferUserData transferData {
			.stagingBuffer = &stagingBuffer,
			.image = image
		};

		VulkanCommand::CommandStageInfo transferStageInfo {
			.name = "GetImageDataTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
					.bufferAccessInfo = {
						.buffer = &stagingBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_IMAGE,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR,
					.imageAccessInfo = {
						.image = image,
						.layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
					}
				}
			},
			.recordCallback = RecordSetImageDataCallback,
			.userData = &transferData
		};

		// Create and record the command
		VulkanCommand transferCommand { GetProgram()->GetRenderer()->GetDevice(), VulkanCommand::COMMAND_TYPE_TRANSFER };

		transferCommand.AddCommandStage(std::move(transferStageInfo));
		transferCommand.RecordCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		// Set the transfer fence create info
		VkFenceCreateInfo transferFenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the transfer fence 
		VkFence transferFence;
		VkResult result = GetProgram()->GetRenderer()->GetLoader()->vkCreateFence(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), &transferFenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &transferFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan image transfer fence! Error code: " + string_VkResult(result));

		// Set the submit info
		VkCommandBufferSubmitInfoKHR commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR,
			.pNext = nullptr,
			.commandBuffer = transferCommand.GetCommandBuffer(),
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

		// Submit the command
		GetProgram()->GetRenderer()->GetDevice()->GetDeviceQueues().transferQueueMutex.lock();
		result = GetProgram()->GetRenderer()->GetLoader()->vkQueueSubmit2KHR(GetProgram()->GetRenderer()->GetDevice()->GetDeviceQueues().transferQueue, 1, &submitInfo, transferFence);
		GetProgram()->GetRenderer()->GetDevice()->GetDeviceQueues().transferQueueMutex.unlock();
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan image transfer command! Error code: " + string_VkResult(result));

		// Wait for the transfer fence
		result = GetProgram()->GetRenderer()->GetLoader()->vkWaitForFences(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), 1, &transferFence, VK_TRUE, UINT64_T_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan image transfer fence! Error code: " + string_VkResult(result));
		
		// Destroy the fence
		GetProgram()->GetRenderer()->GetLoader()->vkDestroyFence(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), transferFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}

	ImageTexture::~ImageTexture() {
		// Destroy the image
		delete image;
	}
}