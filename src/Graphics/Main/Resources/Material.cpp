#include "Material.hpp"
#include "Main/Program.hpp"
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
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
				.descriptorCount = (uint32_t)maxMaterialCount
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
			}
		};

		// Set the material descriptor set layout create info
		VkDescriptorSetLayoutCreateInfo setLayoutInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.bindingCount = 3,
			.pBindings = setLayoutBindings
		};

		// Create the material descriptor set layout
		result = renderer->GetLoader()->vkCreateDescriptorSetLayout(renderer->GetDevice()->GetDevice(), &setLayoutInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &materialSetLayout);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan descriptor set layout for material descriptor sets! Error code: " + string_VkResult(result));
		
		// Create the default image texture
		uint8_t textureData[] { 0xff, 0xff, 0xff, 0xff };
		defaultImageTexture = new ImageTexture(program, 1, 1, &textureData);
	}

	MaterialManager::~MaterialManager() {
		// Destroy the manager's components
		delete defaultImageTexture;
		renderer->GetLoader()->vkDestroyDescriptorSetLayout(renderer->GetDevice()->GetDevice(), materialSetLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroyDescriptorPool(renderer->GetDevice()->GetDevice(), descriptorPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
		renderer->GetLoader()->vkDestroySampler(renderer->GetDevice()->GetDevice(), textureSampler, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}

	// Material functions
	Material::Material(MaterialManager* manager, const MaterialData& data, const MaterialTextures& textures) : manager(manager), data(data), textures(textures) {
		// Set the queue family indices
		VulkanDevice* device = manager->GetRenderer()->GetDevice();

		uint32_t familyIndices[] = { device->GetDeviceQueues().graphicsIndex, device->GetDeviceQueues().transferIndex };
		uint32_t familyCount = 1 + (uint32_t)(familyIndices[0] != familyIndices[1]);
		VkSharingMode sharingMode = (familyCount == 1) ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
		
		// Set the data buffer create info
		VkBufferCreateInfo dataBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = sizeof(MaterialData),
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			.sharingMode = sharingMode,
			.queueFamilyIndexCount = familyCount,
			.pQueueFamilyIndices = familyIndices
		};
		
		// Create the data buffer 
		VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &dataBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &dataBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan material data buffer! Error code: " + string_VkResult(result));
		
		// Allocate and bind the data buffer's memory
		try {
			dataBufferMemory = device->GetAllocator()->AllocBufferMemory(dataBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan material data buffer!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), dataBuffer, dataBufferMemory.memory, dataBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan memory to material data buffer! Error code: " + string_VkResult(result));
		
		// Set the staging buffer create info
		uint32_t transferIndex = device->GetDeviceQueues().transferIndex;

		VkBufferCreateInfo stagingBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = sizeof(MaterialData),
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &transferIndex
		};

		// Create the staging buffer
		VkBuffer stagingBuffer;
		result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &stagingBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &stagingBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan staging buffer for material data! Error code: " + string_VkResult(result));
		
		// Allocate and bind the staging buffer's memory
		VulkanAllocator::Memory stagingBufferMemory;
		try {
			stagingBufferMemory = device->GetAllocator()->AllocBufferMemory(stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		} catch(const std::bad_alloc&) {
			throw std::runtime_error("Failed to allocate memory for Vulkan material staging buffer!");
		}

		result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), stagingBuffer, stagingBufferMemory.memory, stagingBufferMemory.offset);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to bind Vulkan memory to material staging buffer! Error code: " + string_VkResult(result));
		
		// Write the material data to the staging buffer
		*(MaterialData*)(device->GetAllocator()->GetMappedMemory(stagingBufferMemory)) = data;

		// Set the fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the copy fence
		VkFence copyFence;
		result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for image copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferAllocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = manager->GetRenderer()->GetTransferCommandPool()->GetCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		// Allocate the command buffer
		VkCommandBuffer commandBuffer;
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferAllocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Transfer the staging buffer's contents to the data buffer
		VkBufferCopy bufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = sizeof(MaterialData)
		};

		device->GetLoader()->vkCmdCopyBuffer(commandBuffer, stagingBuffer, dataBuffer, 1, &bufferCopy);

		// End the command buffer recording
		device->GetLoader()->vkEndCommandBuffer(commandBuffer);

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = commandBuffer,
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
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, copyFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for image copy! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &copyFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for image copy! Error code: " + string_VkResult(result));
		
		// Destroy the command objects
		device->GetLoader()->vkDestroyFence(device->GetDevice(), copyFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), manager->GetRenderer()->GetTransferCommandPool()->GetCommandPool(), 1, &commandBuffer);

		// Destroy the staging buffer and free its memory
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), stagingBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(stagingBufferMemory);

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
		
		// Set the buffer info and image infos
		VkDescriptorBufferInfo bufferInfo {
			.buffer = dataBuffer,
			.offset = 0,
			.range = VK_WHOLE_SIZE
		};
		VkDescriptorImageInfo ambientTextureInfo {
			.sampler = VK_NULL_HANDLE,
			.imageView = textures.ambientTexture->GetSRGBImageView(),
			.imageLayout = textures.ambientTexture->GetImageLayout()
		};
		VkDescriptorImageInfo diffuseTextureInfo {
			.sampler = VK_NULL_HANDLE,
			.imageView = textures.diffuseTexture->GetSRGBImageView(),
			.imageLayout = textures.diffuseTexture->GetImageLayout()
		};

		// Set the descriptor writes
		VkWriteDescriptorSet descriptorWrites[] {
			{
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
			},
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = descriptorSet,
				.dstBinding = 1,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo = &ambientTextureInfo,
				.pBufferInfo = nullptr,
				.pTexelBufferView = nullptr
			},
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = descriptorSet,
				.dstBinding = 2,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo = &diffuseTextureInfo,
				.pBufferInfo = nullptr,
				.pTexelBufferView = nullptr
			}
		};

		// Update the material's descriptor set
		device->GetLoader()->vkUpdateDescriptorSets(device->GetDevice(), 3, descriptorWrites, 0, nullptr);
	}

	Material::~Material() {
		// Free the descriptor set
		VulkanDevice* device = manager->GetRenderer()->GetDevice();

		device->GetLoader()->vkFreeDescriptorSets(device->GetDevice(), manager->GetDescriptorPool(), 1, &descriptorSet);

		// Destroy the data buffer and free its memory
		device->GetLoader()->vkDestroyBuffer(device->GetDevice(), dataBuffer, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetAllocator()->FreeMemory(dataBufferMemory);
	}
}