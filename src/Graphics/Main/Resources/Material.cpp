#include "Material.hpp"
#include "Main/Program.hpp"
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Static record callbacks
	struct TransferUserData {
		VulkanBuffer* srcBuffer;
		VulkanBuffer* dstBuffer;
	};

	static void RecordMaterialDataCopyCallback(void* userData, VkCommandBuffer commandBuffer) {
		// Get the buffers
		TransferUserData* transferData = (TransferUserData*)userData;
		VulkanBuffer* srcBuffer = transferData->srcBuffer;
		VulkanBuffer* dstBuffer = transferData->dstBuffer;

		// Set the buffer copy info
		VkBufferCopy bufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = srcBuffer->GetBufferSize()
		};

		// Record the transfer
		srcBuffer->GetDevice()->GetLoader()->vkCmdCopyBuffer(commandBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &bufferCopy);
	}

	// Material manager functions
	MaterialManager::MaterialManager(Program* program, size_t maxMaterialCount) : renderer(program->GetRenderer()) {
		// Set the texture sampler create info
		VkSamplerCreateInfo textureSamplerInfo {
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

		// Create the texture sampler
		VkResult result = renderer->GetLoader()->vkCreateSampler(renderer->GetDevice()->GetDevice(), &textureSamplerInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &textureSampler);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan sampler for material textures! Error code: " + string_VkResult(result));
		
		// Set the descriptor pool sizes
		VkDescriptorPoolSize descriptorPoolSizes[] {
			{
				.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = (uint32_t)maxMaterialCount
			},
			{
				.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = (uint32_t)(maxMaterialCount * 4)
			}
		};

		// Set the descriptor pool create info
		VkDescriptorPoolCreateInfo descriptorPoolInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = (uint32_t)maxMaterialCount,
			.poolSizeCount = 2,
			.pPoolSizes = descriptorPoolSizes
		};

		// Create the descriptor pool
		result = renderer->GetLoader()->vkCreateDescriptorPool(renderer->GetDevice()->GetDevice(), &descriptorPoolInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &descriptorPool);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan descriptor pool for material descriptor sets! Error code: " + string_VkResult(result));
		
		// Set the material descriptor set layout bindings
		VkDescriptorSetLayoutBinding setLayoutBindings[] {
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
				.pImmutableSamplers = nullptr
			},
			{
				.binding = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
				.pImmutableSamplers = &textureSampler
			},
			{
				.binding = 2,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
				.pImmutableSamplers = &textureSampler
			},
			{
				.binding = 3,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
				.pImmutableSamplers = &textureSampler
			},
			{
				.binding = 4,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
				.pImmutableSamplers = &textureSampler
			},
			{
				.binding = 5,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
				.pImmutableSamplers = &textureSampler
			}
		};

		// Set the material descriptor set layout create info
		VkDescriptorSetLayoutCreateInfo setLayoutInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.bindingCount = 6,
			.pBindings = setLayoutBindings
		};

		// Create the material descriptor set layout
		result = renderer->GetLoader()->vkCreateDescriptorSetLayout(renderer->GetDevice()->GetDevice(), &setLayoutInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &materialSetLayout);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan descriptor set layout for material descriptor sets! Error code: " + string_VkResult(result));
		
		// Create the default image texture
		uint8_t textureData[] { 0xff, 0xff, 0xff, 0xff };
		defaultImageTexture = new ImageTexture(program, 1, 1, &textureData);

		// Create the default normal map
		uint8_t normalMapData[] { 0x7f, 0x7f, 0xff, 0xff };
		defaultNormalMap = new ImageTexture(program, 1, 1, &normalMapData);
	}

	MaterialManager::~MaterialManager() {
		// Destroy the manager's components
		delete defaultImageTexture;
		delete defaultNormalMap;

		renderer->GetLoader()->vkDestroyDescriptorSetLayout(renderer->GetDevice()->GetDevice(), materialSetLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroyDescriptorPool(renderer->GetDevice()->GetDevice(), descriptorPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroySampler(renderer->GetDevice()->GetDevice(), textureSampler, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}

	// Internal material functions
	VkImageView Material::CreateImageView(VkImage image, VkFormat format) {
		// Set the image view create info
		VkImageViewCreateInfo imageViewInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
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
				.layerCount = 1
			}
		};

		// Create the image view
		VkImageView imageView;
		VkResult result = manager->GetRenderer()->GetLoader()->vkCreateImageView(manager->GetRenderer()->GetDevice()->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &imageView);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan material image view! Error code: " + string_VkResult(result));
		
		return imageView;
	}

	// Material functions
	Material::Material(MaterialManager* manager, const MaterialData& data, const MaterialTextures& textures) : manager(manager), data(data), textures(textures) {
		// Create the data buffer
		VulkanDevice* device = manager->GetRenderer()->GetDevice();

		dataBuffer = new VulkanBuffer(
			device,
			sizeof(MaterialData),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_SHARING_MODE_CONCURRENT,
			VulkanDevice::QUEUE_TYPE_GRAPHICS | VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);
		
		// Create the staging buffer
		VulkanBuffer stagingBuffer {
			device,
			sizeof(MaterialData),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};
		
		// Write the material data to the staging buffer
		*(MaterialData*)(device->GetAllocator()->GetMappedMemory(stagingBuffer.GetBufferMemory())) = data;

		// Set the command stage info
		TransferUserData transferData {
			.srcBuffer = &stagingBuffer,
			.dstBuffer = dataBuffer
		};

		VulkanCommand::CommandStageInfo transferStageInfo {
			.name = "MaterialDataCopyTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = &stagingBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = dataBuffer
					}
				}
			},
			.recordCallback = RecordMaterialDataCopyCallback,
			.userData = &transferData
		};

		// Create and record the command
		VulkanCommand transferCommand { device, VulkanCommand::COMMAND_TYPE_TRANSFER };

		transferCommand.AddCommandStage(std::move(transferStageInfo));
		transferCommand.RecordCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		// Set the transfer fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the transfer fence
		VkFence transferFence;
		VkResult result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &transferFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for material data transfer! Error code: " + string_VkResult(result));
		
		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = transferCommand.GetCommandBuffer(),
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		device->GetDeviceQueues().transferQueueMutex.lock();
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, transferFence);
		device->GetDeviceQueues().transferQueueMutex.unlock();
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for material data transfer! Error code: " + string_VkResult(result));
		
		// Wait for the transfer to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &transferFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for material data transfer! Error code: " + string_VkResult(result));
		
		// Destroy the transfer fence
		device->GetLoader()->vkDestroyFence(device->GetDevice(), transferFence, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Set the descriptor set alloc info
		VkDescriptorSetLayout descriptorSetLayout = manager->GetMaterialSetLayout();

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
			throw std::runtime_error((std::string)"Failed to allocate Vulkan descriptor set for material data! Error code: " + string_VkResult(result));

		// Create the image views
		this->textures.ambientTextureView = CreateImageView(this->textures.ambientTexture->GetImage()->GetImage(), VK_FORMAT_R8G8B8A8_SRGB);
		this->textures.diffuseTextureView = CreateImageView(this->textures.diffuseTexture->GetImage()->GetImage(), VK_FORMAT_R8G8B8A8_SRGB);
		this->textures.specularTextureView = CreateImageView(this->textures.specularTexture->GetImage()->GetImage(), VK_FORMAT_R8G8B8A8_SRGB);
		this->textures.specularExponentMapView = CreateImageView(this->textures.specularExponentMap->GetImage()->GetImage(), VK_FORMAT_R8G8B8A8_UNORM);
		this->textures.normalMapView = CreateImageView(this->textures.normalMap->GetImage()->GetImage(), VK_FORMAT_R8G8B8A8_UNORM);
		
		// Set the buffer info and image infos
		VkImageView imageViews[] {
			this->textures.ambientTextureView,
			this->textures.diffuseTextureView,
			this->textures.specularTextureView,
			this->textures.specularExponentMapView,
			this->textures.normalMapView
		};

		VkDescriptorBufferInfo bufferInfo {
			.buffer = dataBuffer->GetBuffer(),
			.offset = 0,
			.range = VK_WHOLE_SIZE
		};
		VkDescriptorImageInfo imageInfos[5];

		for(size_t i = 0; i != 5; ++i) {
			imageInfos[i] = {
				.sampler = VK_NULL_HANDLE,
				.imageView = imageViews[i],
				.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			};
		}

		// Set the descriptor writes
		VkWriteDescriptorSet descriptorWrites[6];
	
		descriptorWrites[0] = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = descriptorSet,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImageInfo = nullptr,
			.pBufferInfo = &bufferInfo,
			.pTexelBufferView = nullptr
		};

		for(size_t i = 0; i != 5; ++i) {
			descriptorWrites[i + 1] = {
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = descriptorSet,
				.dstBinding = (uint32_t)i + 1,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo = imageInfos + i,
				.pBufferInfo = nullptr,
				.pTexelBufferView = nullptr
			};
		}

		// Update the material's descriptor set
		device->GetLoader()->vkUpdateDescriptorSets(device->GetDevice(), 6, descriptorWrites, 0, nullptr);
	}

	Material::~Material() {
		// Free the descriptor set
		VulkanDevice* device = manager->GetRenderer()->GetDevice();

		device->GetLoader()->vkFreeDescriptorSets(device->GetDevice(), manager->GetDescriptorPool(), 1, &descriptorSet);

		// Destroy the data buffer
		delete dataBuffer;

		// Destroy the image views
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), textures.ambientTextureView, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), textures.diffuseTextureView, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), textures.specularTextureView, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), textures.specularExponentMapView, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyImageView(device->GetDevice(), textures.normalMapView, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}