#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanAllocator.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class representing a cubemap using the data of an image.
	class ImageCubemap : public Asset {
	public:
		/// @brief Creates a new blank image cubemap. The cubemap must be subsequentally loaded or imported.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		/// @param path The asset file's path. Left blank if the asset has no path.
		ImageCubemap(Program* program, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "") : Asset(program, id, path) { }
		/// @brief Creates a new cubemap with the given info.
		/// @param program The program that owns the asset.
		/// @param width The member image width, in pixels.
		/// @param height The member image height, in pixels.
		/// @param data A pointer to the cubemap data, storing the data for all member images in the Vulkan cubemap image order, or nullptr if the image data is undefined.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		/// @param path The asset file's path. Left blank if the asset has no path.
		ImageCubemap(Program* program, uint32_t width, uint32_t height, const void* data = nullptr, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "");
		ImageCubemap(const ImageCubemap&) = delete;
		ImageCubemap(ImageCubemap&&) = delete;

		ImageCubemap& operator=(const ImageCubemap&) = delete;
		ImageCubemap& operator=(ImageCubemap&&) = delete;

		/// @brief Loads the image texture from its path, using the final encoding.
		void Load() override;
		/// @brief Saves the image texture to its path, using the final encoding.
		void Save() const override;
		/// @brief Imports the image texture from its path.
		void Import() override;
		/// @brief Exports the image texture to its path.
		void Export() const override;
		/// @brief Gets the image texture's dependencies.
		/// @return The image's dependencies.
		std::vector<Asset*> GetDependencies() const override {
			return {};
		}
	
		/// @brief Gets the cubemap image's width.
		/// @return The cubemap image's width.
		uint32_t GetWidth() const {
			return width;
		}
		/// @brief Gets the cubemap image's height.
		/// @return The cubemap image's height.
		uint32_t GetHeight() const {
			return height;
		}
		/// @brief Gets the internal Vulkan cubemap image's handle.
		/// @return The internal Vulkan cubemap image's handle.
		VkImage GetImage() const {
			return image;
		}
		/// @brief Gets the Vulkan cubemap image's memory block.
		/// @return A struct containing the cubemap image's memory handle, offset and size.
		VulkanAllocator::Memory GetImageMemory() const {
			return imageMemory;
		}
		/// @brief Gets the handle of the Vulkan cubemap image view that uses the sRGB format.
		/// @return The handle of the Vulkan sRGB cubemap image view.
		VkImageView GetSRGBImageView() const {
			return srgbImageView;
		}
		/// @brief Gets the handle of the Vulkan cubemap image view that uses the linear format.
		/// @return The handle of the Vulkan linear cubemap image view.
		VkImageView GetLinearImageView() const {
			return linearImageView;
		}
		/// @brief Gets the Vulkan cubemap image's format.
		/// @return The Vulkan cubemap image's format.
		VkFormat GetImageFormat() const {
			return VK_FORMAT_R8G8B8A8_UINT;
		}
		/// @brief Gets the Vulkan cubemap format of the sRGB image view.
		/// @return The Vulkan cubemap format of the sRGB image view.
		VkFormat GetSRGBViewFormat() const {
			return VK_FORMAT_R8G8B8A8_SRGB;
		}
		/// @brief Gets the Vulkan cubemap format of the linear image view.
		/// @return The Vulkan cubemap format of the linear image view.
		VkFormat GetLinearViewFormat() const {
			return VK_FORMAT_R8G8B8A8_UNORM;
		}
		/// @brief Gets the Vulkan cubemap image layout.
		/// @return The Vulkan cubemap image layout.
		VkImageLayout GetImageLayout() const {
			return imageLayout;
		}

		/// @brief Gets the cubemap's image data.
		/// @param data A pointer to a buffer where the cubemap image list's data will be stored. Must be able to fit all pixel data.
		void GetImageData(void* data) const;
		/// @brief Sets the cubemap's image data.
		/// @param data A pointer to a buffer storing the cubemap image list's new data. Must contain the data of all pixels.
		void SetImageData(const void* data);

		/// @brief Sets the Vulkan cubemap image layout. Can be used if the layout was transitioned externally.
		/// @param newLayout The Vulkan cubemap image's new layout.
		void SetImageLayout(VkImageLayout newLayout) const {
			imageLayout = newLayout;
		}
		/// @brief Transitions the Vulkan cubemap image's layout.
		/// @param newLayout The new layout to transition the Vulkan cubemap image to.
		/// @param srcStageMask The source pipeline stage mask for the layout transition barrier.
		/// @param dstStageMask The destination pipeline stage mask for the layout transition barrier.
		void TransitionImageLayout(VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR, VkPipelineStageFlags2KHR dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR) const;
		/// @brief Transitions the Vulkan cubemap image's layout.
		/// @param commandBuffer The command buffer to record the layout transition commands to.
		/// @param newLayout The new layout to transition the Vulkan cubemap image to.
		/// @param srcStageMask The source pipeline stage mask for the layout transition barrier.
		/// @param dstStageMask The destination pipeline stage mask for the layout transition barrier.
		void TransitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout, VkPipelineStageFlags2KHR srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR, VkPipelineStageFlags2KHR dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR) const;

		/// @brief Destroys the image.
		~ImageCubemap() {
			DestroyVulkanComponents();
		}
	private:
		WFE_ASSET_TYPE(ImageCubemap)

		void CreateVulkanComponents();
		void DestroyVulkanComponents();
		void InternalGetImageData(void* data, VkImageLayout dstLayout) const;
		void InternalSetImageData(const void* data, VkImageLayout dstLayout);

		uint32_t width = 0, height = 0;

		VkImage image = VK_NULL_HANDLE;
		VulkanAllocator::Memory imageMemory;
		VkImageView srgbImageView = VK_NULL_HANDLE;
		VkImageView linearImageView = VK_NULL_HANDLE;

		mutable VkImageLayout imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	};
}