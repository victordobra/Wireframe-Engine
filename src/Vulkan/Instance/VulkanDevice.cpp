#include "VulkanDevice.hpp"
#include "Core/Memory/Allocator.hpp"
#include "Vulkan/API/VulkanStructSize.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <unordered_map>
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	static const VkPhysicalDeviceDynamicRenderingFeaturesKHR REQUIRED_DYNAMIC_RENDERING_FEATURES {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
		.pNext = nullptr,
		.dynamicRendering = VK_TRUE
	};
	static const VkPhysicalDeviceSynchronization2FeaturesKHR REQUIRED_SYNCHRONIZATION_2_FEATURES {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR,
		.pNext = (void*)&REQUIRED_DYNAMIC_RENDERING_FEATURES,
		.synchronization2 = VK_TRUE
	};

	static const VkPhysicalDeviceIndexTypeUint8FeaturesEXT OPTIONAL_INDEX_TYPE_UINT8_FEATURES {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT,
		.pNext = nullptr,
		.indexTypeUint8 = VK_TRUE
	};
	static const VkPhysicalDeviceMeshShaderFeaturesEXT OPTIONAL_MESH_SHADER_FEATURES {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT,
		.pNext = (void*)&OPTIONAL_INDEX_TYPE_UINT8_FEATURES,
		.taskShader = VK_TRUE,
		.meshShader = VK_TRUE,
		.multiviewMeshShader = VK_FALSE,
		.primitiveFragmentShadingRateMeshShader = VK_FALSE,
		.meshShaderQueries = VK_FALSE
	};

	const VkPhysicalDeviceFeatures2 VulkanDevice::DEFAULT_REQUIRED_DEVICE_FEATURES {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = (void*)&REQUIRED_SYNCHRONIZATION_2_FEATURES,
		.features = {
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
		}
	};
	const VkPhysicalDeviceFeatures2 VulkanDevice::DEFAULT_OPTIONAL_DEVICE_FEATURES {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = (void*)&OPTIONAL_MESH_SHADER_FEATURES,
		.features = {
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
		}
	};
	const std::vector<const char*> VulkanDevice::DEFAULT_REQUIRED_DEVICE_EXTENSIONS {
		VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
	};
	const std::vector<const char*> VulkanDevice::DEFAULT_OPTIONAL_DEVICE_EXTENSIONS {
		VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME,
		VK_EXT_MESH_SHADER_EXTENSION_NAME,
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
	bool VulkanDevice::GetDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2& supportedFeatures, const VkPhysicalDeviceFeatures2& requiredFeatures, const VkPhysicalDeviceFeatures2& optionalFeatures) {
		struct FeatureRequest {
			const VkBool32* requiredFeatures = nullptr;
			const VkBool32* optionalFeatures = nullptr;
			const VkBool32* deviceFeatures = nullptr;
			VkBool32* supportedFeatures = nullptr;
		};

		std::unordered_map<VkStructureType, FeatureRequest> featureRequests;

		// Get all required feature types
		for(const VkBaseInStructure* featureStruct = (const VkBaseInStructure*)requiredFeatures.pNext; featureStruct; featureStruct = featureStruct->pNext)
			featureRequests[featureStruct->sType].requiredFeatures = (const VkBool32*)(featureStruct + 1);

		// Get all optional feature types
		for(const VkBaseInStructure* featureStruct = (const VkBaseInStructure*)optionalFeatures.pNext; featureStruct; featureStruct = featureStruct->pNext)
			featureRequests[featureStruct->sType].optionalFeatures = (const VkBool32*)(featureStruct + 1);
		
		// Build the device features struct
		VkPhysicalDeviceFeatures2 deviceFeatures {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
			.pNext = nullptr
		};
		void** prevNext = &deviceFeatures.pNext;

		for(std::pair<const VkStructureType, FeatureRequest>& pair : featureRequests) {
			// Allocate the new structure
			VkBaseOutStructure* newStruct = (VkBaseOutStructure*)AllocMemory(VulkanGetStructSize(pair.first));
			if(!newStruct)
				throw std::bad_alloc();
			
			// Set its info
			newStruct->sType = pair.first;
			newStruct->pNext = nullptr;

			// Add the struct to the pNext chain
			*prevNext = newStruct;
			prevNext = (void**)&newStruct->pNext;

			pair.second.deviceFeatures = (const VkBool32*)(newStruct + 1);
		}

		// Build the supported features struct
		supportedFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		supportedFeatures.pNext = nullptr;
		prevNext = &supportedFeatures.pNext;

		for(std::pair<const VkStructureType, FeatureRequest>& pair : featureRequests) {
			// Allocate the new structure
			VkBaseOutStructure* newStruct = (VkBaseOutStructure*)AllocMemory(VulkanGetStructSize(pair.first));
			if(!newStruct)
				throw std::bad_alloc();
			
			// Set its info
			newStruct->sType = pair.first;
			newStruct->pNext = nullptr;

			// Add the struct to the pNext chain
			*prevNext = newStruct;
			prevNext = (void**)&newStruct->pNext;

			pair.second.supportedFeatures = (VkBool32*)(newStruct + 1);
		}

		// Add the default request
		featureRequests[VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2] = FeatureRequest {
			.requiredFeatures = (const VkBool32*)&requiredFeatures.features,
			.optionalFeatures = (const VkBool32*)&optionalFeatures.features,
			.deviceFeatures = (const VkBool32*)&deviceFeatures.features,
			.supportedFeatures = (VkBool32*)&supportedFeatures.features
		};

		// Get all device features
		GetLoader()->vkGetPhysicalDeviceFeatures2(physicalDevice, &deviceFeatures);

		// Set all enabled features
		bool allSupported = true;

		for(std::pair<const VkStructureType, FeatureRequest>& pair : featureRequests) {
			// Get the feature count
			size_t featureCount = (VulkanGetStructSize(pair.first) - sizeof(VkBaseOutStructure)) / sizeof(VkBool32);

			// Check all features
			for(size_t i = 0; i != featureCount; ++i) {
				VkBool32 featureRequired = pair.second.requiredFeatures ? pair.second.requiredFeatures[i] : VK_FALSE;
				VkBool32 featureOptional = pair.second.optionalFeatures ? pair.second.optionalFeatures[i] : VK_FALSE;
				VkBool32 featureSupported = pair.second.deviceFeatures[i];
				
				// Exit the loop if the current feature is required, yet not supported
				if(featureRequired && !featureSupported) {
					allSupported = false;
					break;
				}

				// Set the current feature's enabled status
				pair.second.supportedFeatures[i] = featureSupported && (featureRequired || featureOptional);
			}

			// Exit the loop if not all required features are supported
			if(!allSupported)
				break;
		}

		// Free the device feature chain
		for(VkBaseOutStructure* featureStruct = (VkBaseOutStructure*)deviceFeatures.pNext; featureStruct;) {
			// Get the next structure
			VkBaseOutStructure* nextStruct = featureStruct->pNext;

			// Free the current struct
			FreeMemory(featureStruct);
			featureStruct = nextStruct;
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
	void VulkanDevice::SetBestDevice(VulkanSurface* surface, const VkPhysicalDeviceFeatures2& requiredFeatures, const VkPhysicalDeviceFeatures2& optionalFeatures, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions) {
		// Get all devices
		uint32_t deviceCount;
		GetLoader()->vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		GetLoader()->vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, devices.data());

		// Find the best device 
		physicalDevice = VK_NULL_HANDLE;

		deviceFeatures = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
			.pNext = nullptr
		};

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
			VkPhysicalDeviceFeatures2 currentFeatures;
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
				// Delete the old device features
				for(VkBaseOutStructure* featureStruct = (VkBaseOutStructure*)deviceFeatures.pNext; featureStruct;) {
					// Get the next structure
					VkBaseOutStructure* nextStruct = featureStruct->pNext;

					// Free the current struct
					free(featureStruct);
					featureStruct = nextStruct;
				}

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
			} else {
				// Delete the current device features
				for(VkBaseOutStructure* featureStruct = (VkBaseOutStructure*)currentFeatures.pNext; featureStruct;) {
					// Get the next structure
					VkBaseOutStructure* nextStruct = featureStruct->pNext;

					// Free the current struct
					free(featureStruct);
					featureStruct = nextStruct;
				}
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
			.pNext = &deviceFeatures,
			.flags = 0,
			.queueCreateInfoCount = queueInfoCount,
			.pQueueCreateInfos = queueInfos,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = (uint32_t)enabledExtensions.size(),
			.ppEnabledExtensionNames = enabledExtensions.data(),
			.pEnabledFeatures = nullptr
		};

		// Create the device
		VkResult result = GetLoader()->vkCreateDevice(physicalDevice, &deviceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &device);
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
	VulkanDevice::VulkanDevice(VulkanInstance* instance, VulkanSurface* surface, const VkPhysicalDeviceFeatures2& requiredFeatures, const VkPhysicalDeviceFeatures2& optionalFeatures, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions) : instance(instance) {
		// Get the best physical device and create the logical device
		SetBestDevice(surface, requiredFeatures, optionalFeatures, requiredExtensions, optionalExtensions);
		CreateLogicalDevice();
	}

	void VulkanDevice::LogInfo(Logger* logger) const {
		// Log the device's name and type
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)"Using Vulkan device " + deviceProperties.deviceName + " (" + string_VkPhysicalDeviceType(deviceProperties.deviceType) + ").");
		
		// Log the device's queue family indices
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)"Vulkan device queue family indices:\n - Graphics: " + std::to_string(queues.graphicsIndex) + "\n - Present: " + std::to_string(queues.presentIndex) + "\n - Transfer: " + std::to_string(queues.transferIndex) + "\n - Compute: " + std::to_string(queues.computeIndex));

		// Log the device's extensions
		std::string extensionsString = "Enabled Vulkan device extensions:\n";
		for(const char* extension : enabledExtensions)
			extensionsString += (std::string)" - " + extension + "\n";
		extensionsString.pop_back();
		
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, extensionsString);

		// Log the device's total memory size
		VkDeviceSize totalMemorySize = 0;
		for(uint32_t i = 0; i != deviceMemoryProperties.memoryHeapCount; ++i) {
			if(deviceMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				totalMemorySize += deviceMemoryProperties.memoryHeaps[i].size;
		}

		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)"Total Vulkan device memory size: " + std::to_string(totalMemorySize >> 20) + " MiB.");
	}

	uint32_t VulkanDevice::GetMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties) const {
		// Loop through all memory types
		for(uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; ++i) {
			// Check if the current memory type is in the bitmask and has the required properties
			if((memoryTypeBits & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		// No memory type was found; return UINT32_T_MAX
		return UINT32_T_MAX;
	}

	VulkanDevice::~VulkanDevice() {
		// Delete the device features
		for(VkBaseOutStructure* featureStruct = (VkBaseOutStructure*)deviceFeatures.pNext; featureStruct;) {
			// Get the next structure
			VkBaseOutStructure* nextStruct = featureStruct->pNext;

			// Free the current struct
			FreeMemory(featureStruct);
			featureStruct = nextStruct;
		}

		// Destroy the device
		GetLoader()->vkDestroyDevice(device, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}