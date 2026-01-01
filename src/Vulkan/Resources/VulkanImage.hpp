#pragma once

#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanDevice.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class implementing a Vulkan image.
	class VulkanImage {
	public:
		/// @brief Creates a new Vulkan image.
		/// @param device The Vulkan device to own the image.
		/// @param flags The flags to create the image with.
		/// @param imageType The image's type.
		/// @param format The image's format.
		/// @param extent The image's extent.
		/// @param mipLevels The number of mip levels in the image.
		/// @param arrayLayers The number of layers in the image.
		/// @param samples An enum value specifying the number of samples per texel.
		/// @param tiling The tiling arrangement of the texel blocks.
		/// @param aspectFlags The aspect flags of the image.
		/// @param usage The usage flags of the image.
		/// @param sharingMode The sharing mode of the image resource.
		/// @param queueTypes A mask containing all queue types that can access the image. Ignored if sharingMode is VK_SHARING_MODE_EXCLUSIVE.
		/// @param memoryProperties The memory property flags for the image's memory.
		VulkanImage(VulkanDevice* device, VkImageCreateFlags flags, VkImageType imageType, VkFormat format, VkExtent3D extent, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageAspectFlags aspectFlags, VkImageUsageFlags usage, VkSharingMode sharingMode, VulkanDevice::QueueTypeMask queueTypes, VkMemoryPropertyFlags memoryProperties);
		VulkanImage(const VulkanImage&) = delete;
		VulkanImage(VulkanImage&&) noexcept = delete;

		VulkanImage& operator=(const VulkanImage&) = delete;
		VulkanImage& operator=(VulkanImage&&) = delete;

		/// @brief Gets the Vulkan device that owns this image.
		/// @return The Vulkan device that owns this image.
		VulkanDevice* GetDevice() const {
			return device;
		}

		/// @brief Gets the Vulkan image's handle.
		/// @return The Vulkan image's handle.
		VkImage GetImage() const {
			return image;
		}
		/// @brief Gets the Vulkan image's corresponding memory region.
		/// @return A struct describing the Vulkan image's corresponding memory region.
		const VulkanAllocator::Memory& GetImageMemory() const {
			return imageMemory;
		}
		/// @brief Gets the Vulkan image's type.
		/// @return The Vulkan image's type.
		VkImageType GetImageType() const {
			return imageType;
		}
		/// @brief Gets the Vulkan image's format.
		/// @return The Vulkan image's format.
		VkFormat GetImageFormat() const {
			return format;
		}
		/// @brief Gets the Vulkan image's extent.
		/// @return The Vulkan image's extent.
		VkExtent3D GetImageExtent() const {
			return extent;
		}
		/// @brief Gets the number of mip levels in the Vulkan image.
		/// @return The number of mip levels in the Vulkan image.
		uint32_t GetImageMipLevelCount() const {
			return mipLevels;
		}
		/// @brief Gets the number of layers in the Vulkan image.
		/// @return The number of layers in the Vulkan image.
		uint32_t GetImageLayerCount() const {
			return arrayLayers;
		}
		/// @brief Gets the Vulkan image's tiling.
		/// @return The Vulkan image's tiling.
		VkImageTiling GetImageTiling() const {
			return tiling;
		}
		/// @brief Gets the Vulkan image's aspect flags.
		/// @return The Vulkan image's aspect flags.
		VkImageAspectFlags GetImageAspectFlags() const {
			return aspectFlags;
		}

		/// @brief Gets the Vulkan image's layout.
		/// @return The Vulkan image's layout.
		VkImageLayout GetImageLayout() const {
			return layout;
		}
		/// @brief Sets the Vulkan image's layout.
		/// @param newLayout The new layout of the Vulkan image.
		void SetImageLayout(VkImageLayout newLayout) const {
			layout = newLayout;
		}

		/// @brief Destroys the Vulkan image.
		~VulkanImage();
	private:
		VulkanDevice* device;

		VkImage image;
		VulkanAllocator::Memory imageMemory;
		VkImageType imageType;
		VkFormat format;
		VkExtent3D extent;
		uint32_t mipLevels;
		uint32_t arrayLayers;
		VkImageTiling tiling;
		VkImageAspectFlags aspectFlags;
		mutable VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
	};
}