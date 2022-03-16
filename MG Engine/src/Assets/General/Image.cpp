#include "BuildInfo.h"

//Platform specific includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gdiplus.h>

#pragma comment (lib, "Gdiplus.lib")
#endif

#include "Image.h"
#include "Debugger.h"
#include "VulkanDevice.h"
#include "Buffer.h"

#include <filesystem>

namespace mge {
	Image::Image() : width(0), height(0) { 
		CreateImage();
	}
	Image::Image(size_t width, size_t height) : width(width), height(height) {
		CreateImage();
	}

	void Image::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		TransitionImageLayout(commandBuffer, oldLayout, newLayout);

		EndSingleTimeCommands(commandBuffer);
	}
	void Image::TransitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage = 0;
		VkPipelineStageFlags destinationStage = 0;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else
			OutFatalError("Unsupported layout transition!");

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	Image::~Image() {
		vkDestroyImageView(GetDevice(), imageView, nullptr);
		vkDestroyImage(GetDevice(), image, nullptr);
		vkFreeMemory(GetDevice(), imageMemory, nullptr);
	}

	void Image::CreateImageBuffer(Color4* data) {
		uint32_t pixelCount = (uint32_t)(width * height);
		VkDeviceSize deviceSize = pixelCount * sizeof(Color4);
		uint32_t pixelSize = sizeof(Color4);

		Buffer stagingBuffer{ pixelSize, pixelCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT };

		if (data != nullptr) {
			stagingBuffer.Map();
			stagingBuffer.WriteToBuffer((void*)data);
			stagingBuffer.Flush();
		}

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		imageInfo.extent.width = (uint32_t)width;
		imageInfo.extent.height = (uint32_t)height;
		imageInfo.extent.depth = 1;

		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;

		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory);
		TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer.GetBuffer(), image, (uint32_t)width, (uint32_t)height, 1);
		TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	}
	void Image::CreateImageView() {
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(GetDevice(), &viewInfo, nullptr, &imageView))
			OutFatalError("Failed to create image view!");
	}

	//Platform specific functions
#ifdef PLATFORM_WINDOWS
	//Windows

	Image::Image(const char_t* fileLocation) {
		//Convert the string to a wchar string
		const size_t fSize = strlen(fileLocation) + 1;
		charw_t* wFileLocation = new mge::charw_t[fSize];
		mge::size_t fConvCount;

		mbstowcs_s(&fConvCount, wFileLocation, fSize, fileLocation, fSize);
		
		if (fConvCount != fSize)
			OutFatalError("Not enough characters converted!");

		//Load the image with GDI+

		Gdiplus::Bitmap bitmap(wFileLocation);

		//Set the width and the height
		width = bitmap.GetWidth();
		height = bitmap.GetHeight();

		//Allocate the data array
		Color4* data = (Color4*)malloc(width * height * sizeof(Color4));
		if (!data) {
			OutFatalError("Failed to allocate memory!");
			return;
		}

		//Lock the bitmap
		Gdiplus::BitmapData* bitmapData = new Gdiplus::BitmapData();

		Gdiplus::Rect rect(0, 0, (INT)width, (INT)height);
		bitmap.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, bitmapData);

		uint32_t* pixels = (uint32_t*)bitmapData->Scan0;
		const size_t stride = (size_t)abs(bitmapData->Stride) / sizeof(uint32_t);

		//Copy the data from the bitmap
		for (size_t x = 0; x < width; x++)
			for (size_t y = 0; y < height; y++) {
				uint32_t pixelColor = pixels[y * stride + x];

				uint32_t alpha = (pixelColor & 0xff000000) >> 24;
				uint32_t red   = (pixelColor & 0x00ff0000) >> 16;
				uint32_t green = (pixelColor & 0x0000ff00) >> 8;
				uint32_t blue  =  pixelColor & 0x000000ff;

				data[(height - y - 1) * width + x] = { (float32_t)red / 255.f, (float32_t)green / 255.f, (float32_t)blue / 255.f, (float32_t)alpha / 255.f };
			}

		CreateImage(data);
		return;
	}
	void Image::SaveToFile(const char_t* fileLocation) const {
		
	}
#endif //Windows
}
