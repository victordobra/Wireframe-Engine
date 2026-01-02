#include "VulkanPipelineCache.hpp"
#include "Core/Memory/Allocator.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <fstream>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Public functions
    VulkanPipelineCache::VulkanPipelineCache(VulkanDevice* device, const std::filesystem::path& path) : device(device), path(path) {
		// Open the file stream
		std::ifstream stream(path, std::ios::binary);
		size_t dataSize; void* data;

		if(stream) {
			// Get the size of the file
			stream.seekg(0, std::ios::end);
			dataSize = stream.tellg();
			stream.seekg(0, std::ios::beg);

			// Allocate the data
			data = AllocMemory(dataSize);
			if(!data)
				throw std::bad_alloc();
		
			// Read the data
			stream.read((char*)data, dataSize);

			// Close the file stream
			stream.close();
		} else {
			dataSize = 0;
			data = nullptr;
		}

		// Set the pipeline cache create info
		VkPipelineCacheCreateInfo createInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.initialDataSize = dataSize,
			.pInitialData = data
		};

		// Create the pipeline cache
		VkResult result = device->GetLoader()->vkCreatePipelineCache(device->GetDevice(), &createInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &pipelineCache);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan pipeline cache! Error code: " + string_VkResult(result));
		
		// Free the data
		FreeMemory(data);
	}

    void VulkanPipelineCache::SaveData() {
		// Get the size of the data
		size_t dataSize;
		VkResult result = device->GetLoader()->vkGetPipelineCacheData(device->GetDevice(), pipelineCache, &dataSize, nullptr);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to get Vulkan pipeline cache data! Error code: " + string_VkResult(result));
		
		// Allocate the data buffer
		void* data = AllocMemory(dataSize);
		if(!data)
			throw std::bad_alloc();
		
		// Get the data
		result = device->GetLoader()->vkGetPipelineCacheData(device->GetDevice(), pipelineCache, &dataSize, data);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to get Vulkan pipeline cache data! Error code: " + string_VkResult(result));
		
		// Open the file stream
		std::ofstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open pipeline cache file \"" + path.string() + "\" for writing!");
		
		// Write the data
		stream.write((char*)data, dataSize);

		// Close the stream and free the data
		stream.close();
		FreeMemory(data);
	}

    VulkanPipelineCache::~VulkanPipelineCache() {
		// Save the data and destroy the pipeline cache
		SaveData();
		device->GetLoader()->vkDestroyPipelineCache(device->GetDevice(), pipelineCache, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}