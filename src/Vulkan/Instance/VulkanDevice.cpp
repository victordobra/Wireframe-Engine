#include "VulkanDevice.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	const VkPhysicalDeviceFeatures VulkanDevice::DEFAULT_REQUIRED_DEVICE_FEATURES {
		.robustBufferAccess = VK_FALSE,
		.fullDrawIndexUint32 = VK_FALSE,
		.imageCubeArray = VK_FALSE,
		.independentBlend = VK_FALSE,
		.geometryShader = VK_FALSE,
		.tessellationShader = VK_FALSE,
		.sampleRateShading = VK_FALSE,
		.dualSrcBlend = VK_FALSE,
		.logicOp = VK_FALSE,
		.multiDrawIndirect = VK_TRUE,
		.drawIndirectFirstInstance = VK_FALSE,
		.depthClamp = VK_TRUE,
		.depthBiasClamp = VK_FALSE,
		.fillModeNonSolid = VK_TRUE,
		.depthBounds = VK_FALSE,
		.wideLines = VK_FALSE,
		.largePoints = VK_FALSE,
		.alphaToOne = VK_FALSE,
		.multiViewport = VK_FALSE,
		.samplerAnisotropy = VK_FALSE,
		.textureCompressionETC2 = VK_FALSE,
		.textureCompressionASTC_LDR = VK_FALSE,
		.textureCompressionBC = VK_FALSE,
		.occlusionQueryPrecise = VK_FALSE,
		.pipelineStatisticsQuery = VK_FALSE,
		.vertexPipelineStoresAndAtomics = VK_FALSE,
		.fragmentStoresAndAtomics = VK_FALSE,
		.shaderTessellationAndGeometryPointSize = VK_FALSE,
		.shaderImageGatherExtended = VK_FALSE,
		.shaderStorageImageExtendedFormats = VK_FALSE,
		.shaderStorageImageMultisample = VK_FALSE,
		.shaderStorageImageReadWithoutFormat = VK_FALSE,
		.shaderStorageImageWriteWithoutFormat = VK_FALSE,
		.shaderUniformBufferArrayDynamicIndexing = VK_TRUE,
		.shaderSampledImageArrayDynamicIndexing = VK_TRUE,
		.shaderStorageBufferArrayDynamicIndexing = VK_TRUE,
		.shaderStorageImageArrayDynamicIndexing = VK_TRUE,
		.shaderClipDistance = VK_FALSE,
		.shaderCullDistance = VK_FALSE,
		.shaderFloat64 = VK_FALSE,
		.shaderInt64 = VK_FALSE,
		.shaderInt16 = VK_FALSE,
		.shaderResourceResidency = VK_FALSE,
		.shaderResourceMinLod = VK_FALSE,
		.sparseBinding = VK_FALSE,
		.sparseResidencyBuffer = VK_FALSE,
		.sparseResidencyImage2D = VK_FALSE,
		.sparseResidencyImage3D = VK_FALSE,
		.sparseResidency2Samples = VK_FALSE,
		.sparseResidency4Samples = VK_FALSE,
		.sparseResidency8Samples = VK_FALSE,
		.sparseResidency16Samples = VK_FALSE,
		.sparseResidencyAliased = VK_FALSE,
		.variableMultisampleRate = VK_FALSE,
		.inheritedQueries = VK_FALSE
	};
	const VkPhysicalDeviceFeatures VulkanDevice::DEFAULT_OPTIONAL_DEVICE_FEATURES {
		.robustBufferAccess = VK_TRUE,
		.fullDrawIndexUint32 = VK_TRUE,
		.imageCubeArray = VK_TRUE,
		.independentBlend = VK_FALSE,
		.geometryShader = VK_TRUE,
		.tessellationShader = VK_TRUE,
		.sampleRateShading = VK_FALSE,
		.dualSrcBlend = VK_FALSE,
		.logicOp = VK_TRUE,
		.multiDrawIndirect = VK_FALSE,
		.drawIndirectFirstInstance = VK_TRUE,
		.depthClamp = VK_FALSE,
		.depthBiasClamp = VK_TRUE,
		.fillModeNonSolid = VK_FALSE,
		.depthBounds = VK_FALSE,
		.wideLines = VK_TRUE,
		.largePoints = VK_TRUE,
		.alphaToOne = VK_FALSE,
		.multiViewport = VK_FALSE,
		.samplerAnisotropy = VK_TRUE,
		.textureCompressionETC2 = VK_FALSE,
		.textureCompressionASTC_LDR = VK_FALSE,
		.textureCompressionBC = VK_FALSE,
		.occlusionQueryPrecise = VK_TRUE,
		.pipelineStatisticsQuery = VK_TRUE,
		.vertexPipelineStoresAndAtomics = VK_TRUE,
		.fragmentStoresAndAtomics = VK_TRUE,
		.shaderTessellationAndGeometryPointSize = VK_TRUE,
		.shaderImageGatherExtended = VK_TRUE,
		.shaderStorageImageExtendedFormats = VK_TRUE,
		.shaderStorageImageMultisample = VK_TRUE,
		.shaderStorageImageReadWithoutFormat = VK_TRUE,
		.shaderStorageImageWriteWithoutFormat = VK_TRUE,
		.shaderUniformBufferArrayDynamicIndexing = VK_FALSE,
		.shaderSampledImageArrayDynamicIndexing = VK_FALSE,
		.shaderStorageBufferArrayDynamicIndexing = VK_FALSE,
		.shaderStorageImageArrayDynamicIndexing = VK_FALSE,
		.shaderClipDistance = VK_TRUE,
		.shaderCullDistance = VK_TRUE,
		.shaderFloat64 = VK_TRUE,
		.shaderInt64 = VK_TRUE,
		.shaderInt16 = VK_TRUE,
		.shaderResourceResidency = VK_TRUE,
		.shaderResourceMinLod = VK_TRUE,
		.sparseBinding = VK_FALSE,
		.sparseResidencyBuffer = VK_FALSE,
		.sparseResidencyImage2D = VK_FALSE,
		.sparseResidencyImage3D = VK_FALSE,
		.sparseResidency2Samples = VK_FALSE,
		.sparseResidency4Samples = VK_FALSE,
		.sparseResidency8Samples = VK_FALSE,
		.sparseResidency16Samples = VK_FALSE,
		.sparseResidencyAliased = VK_FALSE,
		.variableMultisampleRate = VK_TRUE,
		.inheritedQueries = VK_TRUE
	};
	const std::vector<const char*> VulkanDevice::DEFAULT_REQUIRED_DEVICE_EXTENSIONS {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	const std::vector<const char*> VulkanDevice::DEFAULT_OPTIONAL_DEVICE_EXTENSIONS {
		VK_EXT_MESH_SHADER_EXTENSION_NAME,
		VK_KHR_INDEX_TYPE_UINT8_EXTENSION_NAME,
		VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
	};

	// Internal helper functions
	bool VulkanDevice::GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VulkanSurface* surface, uint32_t& graphicsIndex, uint32_t& presentIndex, uint32_t& transferIndex, uint32_t& computeIndex) {
		// Get all queue families
		uint32_t queueFamilyCount;
		GetLoader()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		GetLoader()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Find the best graphics and present queue family indices
		graphicsIndex = UINT32_T_MAX;
		presentIndex = UINT32_T_MAX;

		for(uint32_t i = 0; i != queueFamilyCount; ++i) {
			// Check if the current queue supports graphics and presenting
			bool graphicsSupport = queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;

			VkBool32 presentSupport;
			GetLoader()->vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface->GetSurface(), &presentSupport);

			// Exit the loop if both graphics and presenting are supported, as this is the best case scenario
			if(graphicsSupport && presentSupport) {
				graphicsIndex = i;
				presentIndex = i;
				break;
			}

			// Set the individual queue family indices
			if(graphicsSupport && graphicsIndex == UINT32_T_MAX) {
				graphicsIndex = i;
			} else if(presentSupport && presentIndex == UINT32_T_MAX) {
				presentIndex = i;
			}
		}

		// Find the best transfer queue family index
		transferIndex = UINT32_T_MAX;
		uint32_t maxScore = 0;

		for(uint32_t i = 0; i != queueFamilyCount; ++i) {
			// Move on to the next queue family if it doesn't support transfer
			if(!(queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT))
				continue;
			
			// Calculate the current device's score
			uint32_t score = 1 + (((i != graphicsIndex) + (i != presentIndex)) << 1);
			score += (i == graphicsIndex && queueFamilies[i].queueCount != 1);
			score += (i == presentIndex && queueFamilies[i].queueCount != 1);

			// Check if the current score is the maximum score
			if(score > maxScore) {
				maxScore = score;
				transferIndex = i;
			}
		}

		// Find the best comput equeue family index
		computeIndex = UINT32_T_MAX;
		maxScore = 0;

		for(uint32_t i = 0; i != queueFamilyCount; ++i) {
			// Move on to the next queue family if it doesn't support compute pipelines
			if(!(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
				continue;

			// Calculate the current device's score
			uint32_t score = 1 + (((i != graphicsIndex) + (i != presentIndex) + (i != transferIndex)) << 1);
			score += (i == graphicsIndex && queueFamilies[i].queueCount != 1);
			score += (i == presentIndex && queueFamilies[i].queueCount != 1);
			score += (i == transferIndex && queueFamilies[i].queueCount != 1);

			// Check if the current score is the maximum score
			if(score > maxScore) {
				maxScore = score;
				computeIndex = i;
			}
		}

		return graphicsIndex != UINT32_T_MAX && presentIndex != UINT32_T_MAX && transferIndex != UINT32_T_MAX && computeIndex != UINT32_T_MAX;
	}
	bool VulkanDevice::GetDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures& supportedFeatures, const VkPhysicalDeviceFeatures& requiredFeatures, const VkPhysicalDeviceFeatures& optionalFeatures) {
		// Get all device features
		VkPhysicalDeviceFeatures devicebFeatures;
		GetLoader()->vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		// Loop through all features
		uint32_t featureCount = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);

		const VkBool32* deviceFeaturesVec = (const VkBool32*)&deviceFeatures;
		const VkBool32* requiredFeaturesVec = (const VkBool32*)&requiredFeatures;
		const VkBool32* optionalFeaturesVec = (const VkBool32*)&optionalFeatures;
		VkBool32* supportedFeaturesVec = (VkBool32*)&supportedFeatures;

		for(uint32_t i = 0; i != featureCount; ++i) {
			// Exit the function if the current feature is required, yet not supported
			if(requiredFeaturesVec[i] && !deviceFeaturesVec[i])
				return false;
			
			// Set the feature in the supported features vector
			supportedFeaturesVec[i] = deviceFeaturesVec[i] && (requiredFeaturesVec[i] || optionalFeaturesVec[i]);
		}

		return true;
	}
	bool VulkanDevice::GetDeviceExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& supportedExtensions, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions) {
		// Get all device extensions
		uint32_t extensionCount;
		GetLoader()->vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		GetLoader()->vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

		// Check if all required extensions are supported
		for(const char* extension : requiredExtensions) {
			// Compare all extension names
			bool supported = false;
			for(size_t i = 0; i != extensions.size() && !supported; ++i)
				supported = !strncmp(extension, extensions[i].extensionName, VK_MAX_EXTENSION_NAME_SIZE);
			
			// Throw an exception if the extension is not supported
			if(!supported)
				return false;
		}

		// Add all required extensions to the vector
		supportedExtensions = requiredExtensions;

		// Add all supported optional extensions
		for(const char* extension : optionalExtensions) {
			// Compare all extension names
			bool supported = false;
			for(size_t i = 0; i != extensions.size() && !supported; ++i)
				supported = !strncmp(extension, extensions[i].extensionName, VK_MAX_EXTENSION_NAME_SIZE);
			
			// Add the exception to the vector, if supported
			if(supported)
				supportedExtensions.push_back(extension);
		}

		return true;
	}
	void VulkanDevice::SetBestDevice(VulkanSurface* surface, const VkPhysicalDeviceFeatures& requiredFeatures, const VkPhysicalDeviceFeatures& optionalFeatures, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions) {
		// Get all devices
		uint32_t deviceCount;
		GetLoader()->vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		GetLoader()->vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, devices.data());

		// Find the best device 
		physicalDevice = VK_NULL_HANDLE;
		deviceProperties.deviceType = VK_PHYSICAL_DEVICE_TYPE_CPU;
		VkDeviceSize totalMemorySize = 0;

		for(VkPhysicalDevice currentDevice : devices) {
			// Check if the device's version is high enough
			VkPhysicalDeviceProperties currentProperties;
			GetLoader()->vkGetPhysicalDeviceProperties(currentDevice, &currentProperties);
			if(currentProperties.apiVersion < instance->GetAPIVersion())
				continue;

			// Get the device's queue family indices
			uint32_t graphicsIndex, presentIndex, transferIndex, computeIndex;
			if(!GetQueueFamilyIndices(currentDevice, surface, graphicsIndex, presentIndex, transferIndex, computeIndex))
				continue;
			
			// Get the device's supported features
			VkPhysicalDeviceFeatures currentFeatures;
			if(!GetDeviceFeatures(currentDevice, currentFeatures, requiredFeatures, optionalFeatures))
				continue;
			
			// Get the device's supported extensions
			std::vector<const char*> currentExtensions;
			if(!GetDeviceExtensions(currentDevice, currentExtensions, requiredExtensions, optionalExtensions))
				continue;
			
			// Calculate the size of the device's local memory
			VkPhysicalDeviceMemoryProperties currentMemoryProperties;
			GetLoader()->vkGetPhysicalDeviceMemoryProperties(currentDevice, &currentMemoryProperties);

			VkDeviceSize currentMemorySize = 0;
			for(uint32_t i = 0; i != currentMemoryProperties.memoryHeapCount; ++i) {
				if(currentMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
					currentMemorySize += currentMemoryProperties.memoryHeaps[i].size;
			}
			
			// If this is the only discrete device or it has the most memory, replace the old device with it
			if((currentProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) || currentMemorySize > totalMemorySize) {
				// Set the physical device and all info structs
				physicalDevice = currentDevice;
				deviceProperties = currentProperties;
				deviceMemoryProperties = currentMemoryProperties;
				deviceFeatures = currentFeatures;

				// Set the queue family indices and extensions
				queues.graphicsIndex = graphicsIndex;
				queues.presentIndex = presentIndex;
				queues.transferIndex = transferIndex;
				queues.computeIndex = computeIndex;
				enabledExtensions = currentExtensions;
			}
		}
	}
	void VulkanDevice::CreateLogicalDevice() {
		// Get all queue families
		uint32_t queueFamilyCount;
		GetLoader()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		GetLoader()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		// Reset all timestamp valid bits; they will be used to keep track of the amount of created queues
		for(uint32_t i = 0; i != queueFamilyCount; ++i)
			queueFamilies[i].timestampValidBits = 0;

		// Set all queue family-local indices
		uint32_t queueFamilyIndices[4] { queues.graphicsIndex, queues.presentIndex, queues.transferIndex, queues.computeIndex }, queueIndices[4];

		for(uint32_t i = 0; i != 4; ++i) {
			// Add a new queue, if there is still room
			if(queueFamilies[queueFamilyIndices[i]].timestampValidBits != queueFamilies[queueFamilyIndices[i]].queueCount)
				++queueFamilies[queueFamilyIndices[i]].timestampValidBits;
			queueIndices[i] = queueFamilies[queueFamilyIndices[i]].timestampValidBits - 1;
		}

		// Set all queue create infos
		VkDeviceQueueCreateInfo queueInfos[4];
		uint32_t queueInfoCount = 0;
		const float32_t queuePriorities[] { 1.0f, 1.0f, 1.0f, 1.0f };

		for(uint32_t i = 0; i != queueFamilyCount; ++i) {
			// Skip the family if no queues are present in it
			if(!queueFamilies[i].timestampValidBits)
				continue;
			
			// Set the new queue create info
			queueInfos[queueInfoCount++] = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.queueFamilyIndex = i,
				.queueCount = queueFamilies[i].timestampValidBits,
				.pQueuePriorities = queuePriorities
			};
		}

		// Set the device create info
		VkDeviceCreateInfo deviceInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.queueCreateInfoCount = queueInfoCount,
			.pQueueCreateInfos = queueInfos,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = (uint32_t)enabledExtensions.size(),
			.ppEnabledExtensionNames = enabledExtensions.data(),
			.pEnabledFeatures = &deviceFeatures
		};

		// Create the device
		VkResult result = GetLoader()->vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan logical device! Error code: " + string_VkResult(result));
		
		// Add the logical device functions to the loader
		GetLoader()->LoadDeviceFunctions(device);
		
		// Get all queues
		GetLoader()->vkGetDeviceQueue(device, queues.graphicsIndex, queueIndices[0], &queues.graphicsQueue);
		GetLoader()->vkGetDeviceQueue(device, queues.presentIndex, queueIndices[1], &queues.presentQueue);
		GetLoader()->vkGetDeviceQueue(device, queues.transferIndex, queueIndices[2], &queues.transferQueue);
		GetLoader()->vkGetDeviceQueue(device, queues.computeIndex, queueIndices[3], &queues.computeQueue);
	}

	// Public functions
	VulkanDevice::VulkanDevice(VulkanInstance* instance, VulkanSurface* surface, const VkPhysicalDeviceFeatures& requiredFeatures, const VkPhysicalDeviceFeatures& optionalFeatures, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions) : instance(instance) {
		// Get the best physical device and create the logical device
		SetBestDevice(surface, requiredFeatures, optionalFeatures, requiredExtensions, optionalExtensions);
		CreateLogicalDevice();
	}

	VulkanDevice::~VulkanDevice() {
		// Destroy the device
		vkDestroyDevice(device, nullptr);
	}
}