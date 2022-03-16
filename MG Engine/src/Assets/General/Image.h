#pragma once
#include "Asset.h"
#include "ColorTypes.h"
#include "VulkanInclude.h"

namespace mge {
	class Image : public Asset {
	public:
		Image();
		Image(const Image&) = delete;
		Image(Image&&) noexcept = delete;
		Image(size_t width, size_t height);
		Image(const char_t* fileLocation);

		Image& operator=(const Image&) = delete;
		Image& operator=(Image&&) noexcept = delete;

		size_t GetWidth() const { return width; }
		size_t GetHeight() const { return height; }

		VkImage GetImage() const { return image; }
		VkDeviceMemory GetImageMemory() const { return imageMemory; }
		VkImageView GetImageView() const { return imageView; }

		void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
		void TransitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);

		void SaveToFile(const char_t* fileLocation) const override;

		~Image();
	private:
		size_t width{0}, height{0};

		VkImage image{};
		VkDeviceMemory imageMemory{};
		VkImageView imageView{};

		void CreateImage(Color4* data = nullptr) {
			CreateImageBuffer(data);
			CreateImageView();

			delete[] data;
		}

		void CreateImageBuffer(Color4* data);
		void CreateImageView();
	};
}