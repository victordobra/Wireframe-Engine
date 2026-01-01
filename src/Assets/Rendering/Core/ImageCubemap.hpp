#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanAllocator.hpp"
#include "Vulkan/Resources/VulkanImage.hpp"
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
		ImageCubemap(Program* program, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "");
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
	
		/// @brief Gets the Vulkan image associated with this texture.
		/// @return The Vulkan image associated with this texture.
		VulkanImage* GetImage() {
			return image;
		}
		/// @brief Gets the Vulkan image associated with this texture.
		/// @return The Vulkan image associated with this texture.
		const VulkanImage* GetImage() const {
			return image;
		}

		/// @brief Gets the cubemap's image data.
		/// @param data A pointer to a buffer where the cubemap image list's data will be stored. Must be able to fit all pixel data.
		void GetImageData(void* data) const;
		/// @brief Sets the cubemap's image data.
		/// @param data A pointer to a buffer storing the cubemap image list's new data. Must contain the data of all pixels.
		void SetImageData(const void* data);


		/// @brief Destroys the image.
		~ImageCubemap();
	private:
		WFE_ASSET_TYPE(ImageCubemap)

		VulkanImage* image;
	};
}