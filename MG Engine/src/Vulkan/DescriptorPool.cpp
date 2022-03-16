#include "DescriptorPool.h"
#include "VulkanDevice.h"
#include "SwapChain.h"
#include "RenderingPipeline.h"
#include "ShaderData.h"
#include "Debugger.h"

namespace mge {
	//Structs
	struct DescriptorSetLayout {
		VkDescriptorSetLayout descriptorSetLayout{};
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
	};

	//Constants
#pragma warning( push )
#pragma warning( disable : 26812 )
	const std::vector<VkDescriptorPoolSize> poolSizes = { { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 * MAX_FRAMES_IN_FLIGHT }, { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, MAX_FRAMES_IN_FLIGHT }, { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT } }; 
	const uint32_t maxSets = 2 * MAX_FRAMES_IN_FLIGHT;
	const VkDescriptorPoolCreateFlags poolFlags = 0;
#pragma warning( pop )

	//Variables
	VkDescriptorPool descriptorPool;

	Buffer* cameraBuffers[MAX_FRAMES_IN_FLIGHT]{};
	Buffer* lightingBuffers[MAX_FRAMES_IN_FLIGHT]{};
	Buffer* materialBuffers[MAX_FRAMES_IN_FLIGHT]{};
	Image* diffuseImages[MAX_FRAMES_IN_FLIGHT]{};

	DescriptorSetLayout globalLayout;
	DescriptorSetLayout localLayout;

	VkDescriptorSet descriptorSets[maxSets];

	//Main functions
	void InitiateDescriptorPool() {
		//Creating the descriptor pool
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		auto result = vkCreateDescriptorPool(GetDevice(), &descriptorPoolInfo, nullptr, &descriptorPool);
		if(result != VK_SUCCESS)
			OutFatalError("Failed to create descriptor pool!");

		//Creating the buffers
		size_t materialBufferSize = PadUniformBufferSize(sizeof(MaterialUbo));

		VkCommandBuffer imageCommandBuffer = BeginSingleTimeCommands();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			cameraBuffers[i] = new Buffer(sizeof(CameraUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			lightingBuffers[i] = new Buffer(sizeof(LightingUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			materialBuffers[i] = new Buffer(materialBufferSize, maxMaterialCount, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

			cameraBuffers[i]->Map();
			lightingBuffers[i]->Map();
			materialBuffers[i]->Map();

			diffuseImages[i] = new Image(targetImageSize * maxMaterialCount, targetImageSize);
			diffuseImages[i]->TransitionImageLayout(imageCommandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

		EndSingleTimeCommands(imageCommandBuffer);

		//Creating the descriptor set layouts
		globalLayout.setLayoutBindings = { { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr }, { 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } };
		localLayout.setLayoutBindings = { { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } };

		//Global layout
		VkDescriptorSetLayoutCreateInfo globalSetLayoutInfo{};
		globalSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		globalSetLayoutInfo.bindingCount = static_cast<uint32_t>(globalLayout.setLayoutBindings.size());
		globalSetLayoutInfo.pBindings = globalLayout.setLayoutBindings.data();

		result = vkCreateDescriptorSetLayout(GetDevice(), &globalSetLayoutInfo, nullptr, &globalLayout.descriptorSetLayout);
		if (result != VK_SUCCESS)
			OutFatalError("Failed to create global set layout!");

		//Local layout
		VkDescriptorSetLayoutCreateInfo localSetLayoutInfo{};
		localSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		localSetLayoutInfo.bindingCount = static_cast<uint32_t>(localLayout.setLayoutBindings.size());
		localSetLayoutInfo.pBindings = localLayout.setLayoutBindings.data();

		result = vkCreateDescriptorSetLayout(GetDevice(), &localSetLayoutInfo, nullptr, &localLayout.descriptorSetLayout);
		if (result != VK_SUCCESS)
			OutFatalError("Failed to create local set layout!");

		//Allocate the descriptor sets
		VkDescriptorSetLayout layouts[maxSets] = { globalLayout.descriptorSetLayout, globalLayout.descriptorSetLayout, localLayout.descriptorSetLayout, localLayout.descriptorSetLayout };

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.pSetLayouts = layouts;
		allocInfo.descriptorSetCount = maxSets;

		if (vkAllocateDescriptorSets(GetDevice(), &allocInfo, descriptorSets) != VK_SUCCESS)
			OutFatalError("Failed to allocate descriptor sets!");

		//Write the buffers
		const uint32_t writesCount = 4 * (uint32_t)MAX_FRAMES_IN_FLIGHT;

		VkWriteDescriptorSet writes[writesCount]{};
		VkDescriptorBufferInfo bufferInfos[3 * (uint32_t)MAX_FRAMES_IN_FLIGHT]{};
		VkDescriptorImageInfo  imageInfos[MAX_FRAMES_IN_FLIGHT]{};

		size_t index = 0;
		//Camera buffers
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			bufferInfos[index] = cameraBuffers[i]->DescriptorInfo();

			writes[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[index].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writes[index].dstBinding = 0;
			writes[index].pBufferInfo = bufferInfos + index;
			writes[index].descriptorCount = 1;
			writes[index].dstSet = descriptorSets[i];
			index++;
		}

		//Lighting buffers
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			bufferInfos[index] = lightingBuffers[i]->DescriptorInfo();

			writes[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[index].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writes[index].dstBinding = 1;
			writes[index].pBufferInfo = bufferInfos + index;
			writes[index].descriptorCount = 1;
			writes[index].dstSet = descriptorSets[i];
			index++;
		}

		//Material buffers
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			bufferInfos[index] = materialBuffers[i]->DescriptorInfo(materialBufferSize);

			writes[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[index].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			writes[index].dstBinding = 0;
			writes[index].pBufferInfo = bufferInfos + index;
			writes[index].descriptorCount = 1;
			writes[index].dstSet = descriptorSets[i + MAX_FRAMES_IN_FLIGHT];
			index++;
		}
		
		//Diffuse images
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfos[i].imageView = diffuseImages[i]->GetImageView();
			imageInfos[i].sampler = GetSampler();

			writes[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[index].dstSet = descriptorSets[i + MAX_FRAMES_IN_FLIGHT];
			writes[index].dstBinding = 1;
			writes[index].dstArrayElement = 0;
			writes[index].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writes[index].descriptorCount = 1;
			writes[index].pImageInfo = imageInfos + i;
			index++;
		}

		//Apply everything
		vkUpdateDescriptorSets(GetDevice(), (uint32_t)writesCount, writes, 0, nullptr);
	}
	void ClearDescriptorPool() {
		vkDestroyDescriptorSetLayout(GetDevice(), globalLayout.descriptorSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(GetDevice(), localLayout.descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(GetDevice(), descriptorPool, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			delete cameraBuffers[i];
			delete lightingBuffers[i];
			delete materialBuffers[i];
			delete diffuseImages[i];
		}
	}

	Buffer** GetCameraBuffers() {
		return cameraBuffers;
	}
	Buffer** GetLightingBuffers() {
		return lightingBuffers;
	}
	Buffer** GetMaterialBuffers() {
		return materialBuffers;
	}
	Image** GetDiffuseImages() {
		return diffuseImages;
	}

	std::vector<VkDescriptorSetLayout> GetDescriptorLayouts() {
		return { globalLayout.descriptorSetLayout, localLayout.descriptorSetLayout };
	}
	std::vector<VkDescriptorSet> GetDescriptorSets() {
		return std::vector<VkDescriptorSet>(descriptorSets, descriptorSets + maxSets);
	}
}