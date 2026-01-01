#pragma once

#include "Assets/Rendering/Core/ImageCubemap.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	class Skybox;
	class Program;

	/// @brief A class that manages skyboxes and their descriptor sets.
	class SkyboxManager {
	public:
		/// @brief Creates a new skybox manager.
		/// @param program The program that owns the skybox manager.
		/// @param maxSkyboxCount The maximum number of skyboxes.
		SkyboxManager(Program* program, size_t maxSkyboxCount);
		SkyboxManager(const SkyboxManager&) = delete;
		SkyboxManager(SkyboxManager&&) = delete;

		SkyboxManager& operator=(const SkyboxManager&) = delete;
		SkyboxManager& operator=(SkyboxManager&&) = delete;

		/// @brief Gets the Vulkan renderer the skybox manager uses.
		/// @return The Vulkan renderer the skybox manager uses.
		VulkanRenderer* GetRenderer() const {
			return renderer;
		}
		/// @brief Gets the Vulkan cubemap sampler all skyboxes use.
		/// @return The Vulkan cubemap sampler all skyboxes use.
		VkSampler GetCubemapSampler() const {
			return cubemapSampler;
		}
		/// @brief Gets the Vulkan descriptor pool in which the skybox descriptor sets are allocated.
		/// @return The Vulkan descriptor pool in which the skybox descriptor sets are allocated.
		VkDescriptorPool GetDescriptorPool() const {
			return descriptorPool;
		}
		/// @brief Gets the Vulkan descriptor set layout of the skybox descriptor sets.
		/// @return The Vulkan descriptor set layout of the skybox descriptor sets.
		VkDescriptorSetLayout GetSkyboxSetLayout() const {
			return skyboxSetLayout;
		}

		/// @brief Destroys the skybox manager.
		~SkyboxManager();
	private:
		VulkanRenderer* renderer;
		VkSampler cubemapSampler;
		VkDescriptorPool descriptorPool;
		VkDescriptorSetLayout skyboxSetLayout;
	};

	/// @brief A class that contains all data for a 3D skybox.
	class Skybox {
	public:
		/// @brief Creates a new skybox.
		/// @param manager The skybox manager to use.
		/// @param cubemap The cubemap to sample.
		Skybox(SkyboxManager* manager, ImageCubemap* cubemap);
		Skybox(const Skybox&) = delete;
		Skybox(Skybox&&) = delete;

		Skybox& operator=(const Skybox&) = delete;
		Skybox& operator=(Skybox&&) = delete;

		/// @brief Gets the skybox manager used by the skybox.
		/// @return The skybox manager used by the skybox.
		SkyboxManager* GetManager() const {
			return manager;
		}
		/// @brief Gets the cubemap that the skybox samples.
		/// @return The cubemap that the skybox samples.
		ImageCubemap* GetCubemap() const {
			return cubemap;
		}
		/// @brief Gets the image view corresponding to the skybox cubemap.
		/// @return The image view corresponding to the skybox cubemap.
		VkImageView GetCubemapView() const {
			return cubemapView;
		}
		/// @brief Gets the Vulkan descriptor set containing the skybox's data.
		/// @return The Vulkan descriptor set containing the skybox's data.
		VkDescriptorSet GetDescriptorSet() const {
			return descriptorSet;
		}

		/// @brief Destroys the skybox.
		~Skybox();
	private:
		SkyboxManager* manager;
		ImageCubemap* cubemap;
		VkImageView cubemapView;
		VkDescriptorSet descriptorSet;
	};
}