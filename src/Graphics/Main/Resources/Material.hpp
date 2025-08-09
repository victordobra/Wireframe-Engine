#pragma once

#include "Assets/Rendering/ImageTexture.hpp"
#include "Core/Math/Templates/Vec3.hpp"
#include "Core/Math/Templates/Vec4.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	class Material;
	class Program;

	/// @brief A class that manages materials and their descriptor sets.
	class MaterialManager {
	public:
		/// @brief Creates a new material manager.
		/// @param program The program that owns the material manager.
		/// @param maxMaterialCount The maximum number of materials.
		MaterialManager(Program* program, size_t maxMaterialCount);
		MaterialManager(const MaterialManager&) = delete;
		MaterialManager(MaterialManager&&) = delete;

		MaterialManager& operator=(const MaterialManager&) = delete;
		MaterialManager& operator=(MaterialManager&&) = delete;

		/// @brief Gets the Vulkan renderer the material manager uses.
		/// @return The Vulkan renderer the material manager uses.
		VulkanRenderer* GetRenderer() const {
			return renderer;
		}
		/// @brief Gets the Vulkan texture sampler all material textures use.
		/// @return The Vulkan texture sampler all material textures use.
		VkSampler GetTextureSampler() const {
			return textureSampler;
		}
		/// @brief Gets the Vulkan descriptor pool in which the material descriptor sets are allocated.
		/// @return The Vulkan descriptor pool in which the material descriptor sets are allocated.
		VkDescriptorPool GetDescriptorPool() const {
			return descriptorPool;
		}
		/// @brief Gets the Vulkan descriptor set layout of the material descriptor sets.
		/// @return The Vulkan descriptor set layout of the material descriptor sets.
		VkDescriptorSetLayout GetMaterialSetLayout() const {
			return materialSetLayout;
		}
		/// @brief Gets the default white image texture, used when no texture is specified.
		/// @return The default white image texture, used when no texture is specified.
		ImageTexture* GetDefaultImageTexture() const {
			return defaultImageTexture;
		}
		/// @brief Gets the default normal map texture, used when no normal map is specified.
		/// @return The default normal map texture, used when no normal map is specified.
		ImageTexture* GetDefaultNormalMap() const {
			return defaultNormalMap;
		}

		/// @brief Destroys the material manager.
		~MaterialManager();
	private:
		friend Material;

		VulkanRenderer* renderer;
		VkSampler textureSampler;
		VkDescriptorPool descriptorPool;
		VkDescriptorSetLayout materialSetLayout;
		ImageTexture* defaultImageTexture;
		ImageTexture* defaultNormalMap;
	};

	/// @brief A class that contains all data and textures for a 3D material.
	class Material {
	public:
		/// @brief A struct that contains all material general data.
		struct MaterialData {
			/// @brief The color affected by ambient lighting.
			alignas(16) Vec3f ambientColor;
			/// @brief The color affected by diffuse lighting.
			alignas(16) Vec3f diffuseColor;
			/// @brief The color affected by specular lighting.
			alignas(16) Vec3f specularColor;
			/// @brief The exponent for the specular component.
			float specularExponent;
		};
		/// @brief A struct that contains all image textures used by the material.
		struct MaterialTextures {
			/// @brief The texture used for the ambient color.
			ImageTexture* ambientTexture;
			/// @brief The texture used for the diffuse color.
			ImageTexture* diffuseTexture;
			/// @brief The texture used for the specular color.
			ImageTexture* specularTexture;
			/// @brief The texture map used for the specular exponent.
			ImageTexture* specularExponentMap;
			/// @brief The texture map used to change the surface normals.
			ImageTexture* normalMap;
		};

		/// @brief Creates a new material.
		/// @param manager The material manager to use.
		/// @param data The material's data.
		/// @param textures The material's textures.
		Material(MaterialManager* manager, const MaterialData& data, const MaterialTextures& textures);
		Material(const Material&) = delete;
		Material(Material&&) = delete;

		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;

		/// @brief Gets the material manager used by the material.
		/// @return The material manager used by the material.
		MaterialManager* GetManager() const {
			return manager;
		}
		/// @brief Gets the material's data.
		/// @return A struct containing the material's data.
		const MaterialData& GetData() const {
			return data;
		}
		/// @brief Gets the material's textures.
		/// @return A struct containing the material's textures.
		const MaterialTextures& GetTextures() const {
			return textures;
		}
		/// @brief Gets the Vulkan buffer storing the material's data
		/// @return The Vulkan buffer storing the material's data
		VkBuffer GetDataBuffer() const {
			return dataBuffer;
		}
		/// @brief Gets the Vulkan data buffer's memory block.
		/// @return A struct containing the data buffer's memory handle, offset and size.
		VulkanAllocator::Memory GetDataBufferMemory() const {
			return dataBufferMemory;
		}
		/// @brief Gets the Vulkan descriptor set referencing the material's data and textures.
		/// @return The Vulkan descriptor set referencing the material's data and textures.
		VkDescriptorSet GetDescriptorSet() const {
			return descriptorSet;
		}

		/// @brief Destroys the material.
		~Material();
	private:
		MaterialManager* manager;
		MaterialData data;
		MaterialTextures textures;
		VkBuffer dataBuffer;
		VulkanAllocator::Memory dataBufferMemory;
		VkDescriptorSet descriptorSet;
	};
}