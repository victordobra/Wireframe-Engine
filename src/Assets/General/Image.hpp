#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanAllocator.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class representing an image asset.
	class Image : public Asset {
	public:
		/// @brief Creates a new image.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		Image(Program* program, uint64_t id = UINT64_T_MAX) : Asset(program, id) { }
		Image(const Image&) = delete;
		Image(Image&&) = delete;

		Image& operator=(const Image&) = delete;
		Image& operator=(Image&&) = delete;

		/// @brief Loads the image from a binary stream, using the final encoding.
		/// @param stream The stream to load the image from.
		void Load(std::istream& stream) override;
		/// @brief Saves the image to a binary stream, using the final encoding.
		/// @param stream The stream to save the image to.
		void Save(std::ostream& stream) const override;
		/// @brief Imports the image from a file.
		/// @param path The path to the file to import the image from.
		void Import(const std::string& path) override;
		/// @brief Exports the image to a file.
		/// @param path The path to the file to export the image to.
		void Export(const std::string& path) const override;
		/// @brief Gets the image's dependencies.
		/// @return The image's dependencies.
		std::vector<Asset*> GetDependencies() const override {
			return {};
		}
	
		/// @brief Gets the image's width.
		/// @return The image's width.
		uint32_t GetWidth() const {
			return width;
		}
		/// @brief Gets the image's height.
		/// @return The image's height.
		uint32_t GetHeight() const {
			return height;
		}
		/// @brief Gets the internal Vulkan image's handle.
		/// @return The internal Vulkan image's handle.
		VkImage GetImage() const {
			return image;
		}
		/// @brief Gets the Vulkan image's memory block.
		/// @return A struct containing the image's memory handle, offset and size.
		VulkanAllocator::Memory GetImageMemory() const {
			return imageMemory;
		}
		/// @brief Gets the handle of the Vulkan image view that uses the sRGB format.
		/// @return The handle of the Vulkan sRGB image view.
		VkImageView GetSRGBImageView() const {
			return srgbImageView;
		}
		/// @brief Gets the handle of the Vulkan image view that uses the linear format.
		/// @return The handle of the Vulkan linear image view.
		VkImageView GetLinearImageView() const {
			return linearImageView;
		}
		/// @brief Gets the Vulkan image's format.
		/// @return The Vulkan image's format.
		VkFormat GetImageFormat() const {
			return imageFormat;
		}
		/// @brief Gets the Vulkan format of the sRGB image view.
		/// @return The Vulkan format of the sRGB image view.
		VkFormat GetSRGBViewFormat() const {
			return srgbViewFormat;
		}
		/// @brief Gets the Vulkan format of the linear image view.
		/// @return The Vulkan format of the linear image view.
		VkFormat GetLinearViewFormat() const {
			return linearViewFormat;
		}
		/// @brief Gets the Vulkan image layout.
		/// @return The Vulkan image layout.
		VkImageLayout GetImageLayout() const {
			return imageLayout;
		}

		/// @brief Gets the image's data.
		/// @param data A pointer to a buffer where the image's data will be stored. Must be able to fit all pixel data.
		void GetImageData(void* data) const;
		/// @brief Sets the image's data.
		/// @param data A pointer to a buffer storing the image's new data. Must contain the data of all pixels.
		void SetImageData(const void* data);

		/// @brief Sets the image layout. Can be used if the layout was transitioned externally.
		/// @param newLayout The image's new layout.
		void SetImageLayout(VkImageLayout newLayout) {
			imageLayout = newLayout;
		}
		/// @brief Transitions the image's layout.
		/// @param newLayout The new layout to transition the image to.
		/// @param srcStageMask The source pipeline stage mask for the layout transition barrier.
		/// @param dstStageMask The destination pipeline stage mask for the layout transition barrier.
		void TransitionImageLayout(VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR, VkPipelineStageFlags2KHR dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR) const;
		/// @brief Transitions the image's layout.
		/// @param commandBuffer The command buffer to record the layout transition commands to.
		/// @param newLayout The new layout to transition the image to.
		/// @param srcStageMask The source pipeline stage mask for the layout transition barrier.
		/// @param dstStageMask The destination pipeline stage mask for the layout transition barrier.
		void TransitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR, VkPipelineStageFlags2KHR dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR) const;

		/// @brief Destroys the image.
		~Image() {
			DestroyVulkanComponents();
		}
	private:
		WFE_ASSET_TYPE(Image, { "bmp", "jpg", "jpeg", "png" })

		void CreateVulkanComponents();
		void DestroyVulkanComponents();
		void InternalGetImageData(void* data, VkImageLayout dstLayout) const;
		void InternalSetImageData(const void* data, VkImageLayout dstLayout);

		uint32_t width = 0, height = 0;

		VkImage image = VK_NULL_HANDLE;
		VulkanAllocator::Memory imageMemory;
		VkImageView srgbImageView = VK_NULL_HANDLE;
		VkImageView linearImageView = VK_NULL_HANDLE;

		VkFormat imageFormat = VK_FORMAT_UNDEFINED;
		VkFormat srgbViewFormat = VK_FORMAT_UNDEFINED;
		VkFormat linearViewFormat = VK_FORMAT_UNDEFINED;

		mutable VkImageLayout imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	};
}