#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanAllocator.hpp"
#include "Vulkan/Resources/VulkanImage.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class representing a texture using the data of an image.
	class ImageTexture : public Asset {
	public:
		/// @brief Creates a new blank image texture. The image must be subsequentally loaded or imported.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		/// @param path The asset file's path. Left blank if the asset has no path.
		ImageTexture(Program* program, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "");
		/// @brief Creates a new image texture with the given info.
		/// @param program The program that owns the asset.
		/// @param width The image width, in pixels.
		/// @param height The image height, in pixels.
		/// @param data A pointer to the image data, or nullptr if the image data is undefined.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		/// @param path The asset file's path. Left blank if the asset has no path.
		ImageTexture(Program* program, uint32_t width, uint32_t height, const void* data = nullptr, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "");
		ImageTexture(const ImageTexture&) = delete;
		ImageTexture(ImageTexture&&) = delete;

		ImageTexture& operator=(const ImageTexture&) = delete;
		ImageTexture& operator=(ImageTexture&&) = delete;

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

		/// @brief Gets the texture's image data.
		/// @param data A pointer to a buffer where the image's data will be stored. Must be able to fit all pixel data.
		void GetImageData(void* data) const;
		/// @brief Sets the texture's image data.
		/// @param data A pointer to a buffer storing the image's new data. Must contain the data of all pixels.
		void SetImageData(const void* data);

		/// @brief Destroys the image.
		~ImageTexture();
	private:
		WFE_ASSET_TYPE(ImageTexture, ".png", ".tga")

		VulkanImage* image;
	};
}