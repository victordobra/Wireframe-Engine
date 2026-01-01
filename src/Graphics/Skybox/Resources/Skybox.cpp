#include "Skybox.hpp"
#include "Main/Program.hpp"
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Skybox manager functions
	SkyboxManager::SkyboxManager(Program* program, size_t maxSkyboxCount) : renderer(program->GetRenderer()) {
		// Set the cubemap sampler create info
		VkSamplerCreateInfo cubemapSamplerInfo {
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.magFilter = VK_FILTER_LINEAR,
			.minFilter = VK_FILTER_LINEAR,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.mipLodBias = 0.0f,
			.anisotropyEnable = renderer->GetDevice()->GetDeviceFeatures().features.samplerAnisotropy,
			.maxAnisotropy = 1.0f,
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_NEVER,
			.minLod = 0.0f,
			.maxLod = 0.0f,
			.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
			.unnormalizedCoordinates = VK_FALSE
		};

		// Create the cubemap sampler
		VkResult result = renderer->GetLoader()->vkCreateSampler(renderer->GetDevice()->GetDevice(), &cubemapSamplerInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &cubemapSampler);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan sampler for skybox cubemaps! Error code: " + string_VkResult(result));
		
		// Set the descriptor pool create info
		VkDescriptorPoolSize descriptorPoolSize {
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = (uint32_t)maxSkyboxCount
		};

		VkDescriptorPoolCreateInfo descriptorPoolInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = (uint32_t)maxSkyboxCount,
			.poolSizeCount = 1,
			.pPoolSizes = &descriptorPoolSize
		};

		// Create the descriptor pool
		result = renderer->GetLoader()->vkCreateDescriptorPool(renderer->GetDevice()->GetDevice(), &descriptorPoolInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &descriptorPool);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan descriptor pool for skybox descriptor sets! Error code: " + string_VkResult(result));

		// Set the skybox set layout create info
		VkDescriptorSetLayoutBinding setLayoutBinding {
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = &cubemapSampler
		};

		VkDescriptorSetLayoutCreateInfo setLayoutInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.bindingCount = 1,
			.pBindings = &setLayoutBinding
		};

		// Create the skybox descriptor set layout
		result = renderer->GetLoader()->vkCreateDescriptorSetLayout(renderer->GetDevice()->GetDevice(), &setLayoutInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &skyboxSetLayout);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan descriptor set layout for skybox descriptor sets! Error code: " + string_VkResult(result));
	}

	SkyboxManager::~SkyboxManager() {
		// Destroy the manager's components
		renderer->GetLoader()->vkDestroyDescriptorSetLayout(renderer->GetDevice()->GetDevice(), skyboxSetLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroyDescriptorPool(renderer->GetDevice()->GetDevice(), descriptorPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroySampler(renderer->GetDevice()->GetDevice(), cubemapSampler, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}

	// Skybox functions
	Skybox::Skybox(SkyboxManager* manager, ImageCubemap* cubemap) : manager(manager), cubemap(cubemap) {
		// Set the image view create info
		VulkanDevice* device = manager->GetRenderer()->GetDevice();

		VkImageViewCreateInfo imageViewInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = cubemap->GetImage()->GetImage(),
			.viewType = VK_IMAGE_VIEW_TYPE_CUBE,
			.format = VK_FORMAT_R8G8B8A8_SRGB,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY
			},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 6
			}
		};

		// Create the image view
		VkResult result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &cubemapView);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan skybox cubemap image view! Error code: " + string_VkResult(result));

		// Set the descriptor set alloc info
		VkDescriptorSetLayout descriptorSetLayout = manager->GetSkyboxSetLayout();

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = manager->GetDescriptorPool(),
			.descriptorSetCount = 1,
			.pSetLayouts = &descriptorSetLayout
		};

		// Allocate the descriptor set
		result = device->GetLoader()->vkAllocateDescriptorSets(device->GetDevice(), &descriptorSetAllocInfo, &descriptorSet);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan descriptor set for skybox data! Error code: " + string_VkResult(result));

		// Set the descriptor set write info
		VkDescriptorImageInfo cubemapInfo {
			.sampler = VK_NULL_HANDLE,
			.imageView = cubemapView,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};

		VkWriteDescriptorSet descriptorSetWrite {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = descriptorSet,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImageInfo = &cubemapInfo,
			.pBufferInfo = nullptr,
			.pTexelBufferView = nullptr
		};

		// Update the descriptor set
		device->GetLoader()->vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorSetWrite, 0, nullptr);
	}

	Skybox::~Skybox() {
		// Free the descriptor set and destroy the image view
		manager->GetRenderer()->GetLoader()->vkFreeDescriptorSets(manager->GetRenderer()->GetDevice()->GetDevice(), manager->GetDescriptorPool(), 1, &descriptorSet);
		manager->GetRenderer()->GetLoader()->vkDestroyImageView(manager->GetRenderer()->GetDevice()->GetDevice(), cubemapView, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}
