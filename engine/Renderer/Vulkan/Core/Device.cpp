#include "Device.hpp"
#include "Allocator.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "Renderer/Vulkan/Loader/Loader.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	static const uint32_t MIN_DEVICE_API_VERSION = VK_API_VERSION_1_0;

	static const uint32_t DISCRETE_GPU_SCORE = 128;
	static const uint32_t OPTIONAL_EXTENSION_SCORE_INCREASE = 8;
	static const uint32_t TRANSFER_QUEUE_FAMILY_SCORE_INCREASE = 8;
	static const uint32_t COMPUTE_QUEUE_FAMILY_SCORE_INCREASE = 8;
	static const uint32_t GROUPED_GRAPHICS_PRESENT_QUEUE_FAMILIES_SCORE_INCREASE = 16;

	static const unordered_set<const char_t*> MANDATORY_DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	static const unordered_set<const char_t*> OPTIONAL_DEVICE_EXTENSIONS = {
		VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
		VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
		VK_KHR_BIND_MEMORY_2_EXTENSION_NAME
	};

	// Internal variables
	static VkPhysicalDeviceProperties deviceProperties;
	static VkPhysicalDeviceFeatures deviceFeatures;
	static VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
	static VulkanQueueFamilyIndices queueFamilies;
	static set<const char_t*> deviceExtensions;

	static VkPhysicalDevice physicalDevice;
	static VkDevice device;
	static VkQueue graphicsQueue;
	static VkQueue presentQueue;
	static VkQueue transferQueue;
	static VkQueue computeQueue;

	// Internal helper functions
	static VulkanQueueFamilyIndices GetQueueFamilies(VkPhysicalDevice physicalDevice) {
		// Retrieve the number of queue families
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		// Allocate the array of queue families and fill it
		VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties), MEMORY_USAGE_ARRAY);
		if(!queueFamilies)
			WFE_LOG_FATAL("Failed to allocate Vulkan queue family property array!");
		
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

		// Loop through the queue families; picking the most favorable for all queue types
		VulkanQueueFamilyIndices families;
		
		// First try to find the best graphics queue family, preferably one that also supports presenting
		for(uint32_t i = 0; i != queueFamilyCount; ++i) {
			// Load the current queue family properties
			VkQueueFamilyProperties queueFamily = queueFamilies[i];

			// Check if the current family is a graphics queue family
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				// Check if the current queue family also supports presenting
				VkBool32 presentSupported;
				VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, GetVulkanSurface(), &presentSupported);
				if(result != VK_SUCCESS)
					WFE_LOG_FATAL("Failed to query Vulkan queue family present support! Error code: %s", string_VkResult(result));
				
				// If the current queue family also supports presenting, set the new queue families and exit the loop
				if(presentSupported) {
					families.graphicsQueueIndex = i;
					families.presentQueueIndex = i;

					break;
				}

				// Presenting is not supported; store the queue family index but keep looking
				families.graphicsQueueIndex = i;
			}
		}

		// Look for a present queue with no criteria if no graphics and present family was found
		if(families.presentQueueIndex == -1)
			for(uint32_t i = 0; i != queueFamilyCount; ++i) {
				// Load the current queue family properties
				VkQueueFamilyProperties queueFamily = queueFamilies[i];

				// Skip the current family if it is a graphics queue family, since we know that no graphics queue family supports presenting in this scenario
				if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					continue;
				
				// Check if the current queue family supports presenting
				VkBool32 presentSupported;
				VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, GetVulkanSurface(), &presentSupported);
				if(result != VK_SUCCESS)
					WFE_LOG_FATAL("Failed to query Vulkan queue family present support! Error code: %s", string_VkResult(result));
				
				// If the current queue supports presenting, set the transfer queue family index and exit the loop
				if(presentSupported) {
					families.presentQueueIndex = i;
					break;
				}
			}
		
		// Try to find the transfer and compute queue families that aren't being used up for other roles
		for(uint32_t i = 0; i != queueFamilyCount && (families.transferQueueIndex == -1 || families.computeQueueIndex == -1); ++i) {
			// Skip the current queue if it is the current graphics or present family
			if(i == families.graphicsQueueIndex || i == families.presentQueueIndex)
				continue;

			// Load the current queue family properties
			VkQueueFamilyProperties queueFamily = queueFamilies[i];

			// Set the compute queue family index and move on to the next family if the current family is a compute queue family
			if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
				families.computeQueueIndex = i;
				continue;
			}

			// Set the transfer queue family index if the current queue family 
			if(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
				families.transferQueueIndex = i;
		}

		// Check if no optimal compute queue family was found
		if(families.computeQueueIndex == -1) {
			// Check if the graphics or present queue families also support compute queues
			if(families.graphicsQueueIndex != -1 && queueFamilies[families.graphicsQueueIndex].queueFlags & VK_QUEUE_COMPUTE_BIT)
				families.computeQueueIndex = families.graphicsQueueIndex;
			else if(families.presentQueueIndex != -1 && queueFamilies[families.presentQueueIndex].queueFlags & VK_QUEUE_COMPUTE_BIT)
				families.computeQueueIndex = families.presentQueueIndex;
		}
			
		// Check if no optimal transfer queue family was found
		if(families.transferQueueIndex == -1) {
			if(families.computeQueueIndex != -1 && queueFamilies[families.computeQueueIndex].queueFlags & VK_QUEUE_TRANSFER_BIT)
				families.transferQueueIndex = families.computeQueueIndex;
			else if(families.graphicsQueueIndex != -1 && queueFamilies[families.graphicsQueueIndex].queueFlags & VK_QUEUE_TRANSFER_BIT)
				families.transferQueueIndex = families.graphicsQueueIndex;
			else if(families.presentQueueIndex != -1 && queueFamilies[families.presentQueueIndex].queueFlags & VK_QUEUE_TRANSFER_BIT)
				families.transferQueueIndex = families.presentQueueIndex;
		}

		// Free the queue family array
		free(queueFamilies, MEMORY_USAGE_ARRAY);

		return families;
	}
	static VkFormat FindSupportedFormat(VkPhysicalDevice physicalDevice, const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		// Loop through every given candidate
		for(VkFormat candidate : candidates) {
			// Get the candidate format's properties
			VkFormatProperties properties;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, candidate, &properties);

			// Return the current format if it supports all the requested formats for the requested tiling
			if(tiling == VK_IMAGE_TILING_LINEAR && properties.linearTilingFeatures & features == features)
				return candidate;
			else if(tiling == VK_IMAGE_TILING_OPTIMAL && properties.optimalTilingFeatures & features == features)
				return candidate;
		}

		// No supported format was found; return VK_FORMAT_UNDEFINED
		return VK_FORMAT_UNDEFINED;
	}
	static uint32_t CalculateDeviceScore(VkPhysicalDevice physicalDevice) {
		// Start with the initial score of 1, as 0 means that the current device does not meet all requirements
		uint32_t score = 1;

		// Load the physical device's properties
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

		// Decompose the version and increase the score for higher versions
		uint32_t versionMajor = VK_VERSION_MAJOR(deviceProperties.apiVersion);
		uint32_t versionMinor = VK_VERSION_MINOR(deviceProperties.apiVersion);

		score += (versionMajor << 3) + versionMinor;

		// Increase the score if the device is discrete
		score += (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) * DISCRETE_GPU_SCORE;

		// Load the physical device's extensions count
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		// Allocate the array of extesnions and fill it
		VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties), MEMORY_USAGE_ARRAY);
		if(!extensions)
			WFE_LOG_FATAL("Failed to allocate Vulkan physical device extension array!");
		
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions);

		// Loop through the extensions, checking if any of them are mandatory or optional
		size_t mandatoryExtensionCount = 0;
		VkExtensionProperties* extensionsEnd = extensions + extensionCount;
		for(VkExtensionProperties* extension = extensions; extension != extensionsEnd; ++extension) {
			// Check if the current extension is mandatory
			if(MANDATORY_DEVICE_EXTENSIONS.count(extension->extensionName)) {
				// Increment the mandatory extension count
				++mandatoryExtensionCount;

				continue;
			}

			// Check if the current extension if optional
			if(OPTIONAL_DEVICE_EXTENSIONS.count(extension->extensionName)) {
				// Increase the device score
				score += OPTIONAL_EXTENSION_SCORE_INCREASE;

				continue;
			}
		}

		// Free the extension array
		free(extensions, MEMORY_USAGE_ARRAY);

		// Exit the function if not all mandatory extensions are supported
		if(mandatoryExtensionCount != MANDATORY_DEVICE_EXTENSIONS.size())
			return 0;
		
		// Get the physical device's supported surface present mode and format count
		uint32_t surfacePresentModeCount, surfaceFormatCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, GetVulkanSurface(), &surfacePresentModeCount, nullptr);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, GetVulkanSurface(), &surfaceFormatCount, nullptr);

		// Exit the function if no present modes or formats are supported
		if(!surfacePresentModeCount || !surfaceFormatCount)
			return 0;
		
		// Exit the function if the current physical device doesn't have any supported depth formats
		if(FindSupportedFormat(physicalDevice, { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_UNDEFINED)
			return 0;
		
		// Get the current physical device's queue family indices
		VulkanQueueFamilyIndices families = GetQueueFamilies(physicalDevice);

		// Exit the function if no suitable graphics or present queue families were found
		if(families.graphicsQueueIndex == -1 || families.presentQueueIndex == -1)
			return 0;
		
		// Award extra score if the graphics and present queue families are grouped
		score += (families.graphicsQueueIndex == families.presentQueueIndex) * GROUPED_GRAPHICS_PRESENT_QUEUE_FAMILIES_SCORE_INCREASE;

		return score;
	}

	static bool8_t FindPhysicalDevice() {
		// Get the number of physical devices
		uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(GetVulkanInstance(), &physicalDeviceCount, nullptr);

		// Exit the function if no physical devices were found
		if(!physicalDeviceCount)
			return false;
		
		// Allocate the array of physical devices and fill it
		VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*)malloc(physicalDeviceCount * sizeof(VkPhysicalDevice), MEMORY_USAGE_ARRAY);
		if(!physicalDevices)
			WFE_LOG_FATAL("Failed to allocate Vulkan physical device array!");
		
		vkEnumeratePhysicalDevices(GetVulkanInstance(), &physicalDeviceCount, physicalDevices);

		// Loop through every physical device and pick the one with the best score
		uint32_t bestScore = 0;
		
		VkPhysicalDevice* physicalDevicesEnd = physicalDevices + physicalDeviceCount;
		for(VkPhysicalDevice* listPhysicalDevice = physicalDevices; listPhysicalDevice != physicalDevicesEnd; ++listPhysicalDevice) {
			// Calculate the current physical device's score
			uint32_t score = CalculateDeviceScore(*listPhysicalDevice);

			// Check if the current score if the highest score
			if(score > bestScore) {
				// Set the new best physical device and score
				physicalDevice = *listPhysicalDevice;
				bestScore = score;
			}
		}

		// Free the physical device array
		free(physicalDevices, MEMORY_USAGE_ARRAY);

		// 0 is an invalid score, therefore the function should return false unly when no device met the given requirements
		return bestScore;
	}
	static void SetupPhysicalDevice() {
		// Get the phyiscal device's properties
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
		GenerateVulkanSurfaceSupportDetails();
		queueFamilies = GetQueueFamilies(physicalDevice);

		// Load the physical device's extensions count
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		// Allocate the array of extesnions and fill it
		VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties), MEMORY_USAGE_ARRAY);
		if(!extensions)
			WFE_LOG_INFO("Failed to allocate Vulkan physical device extension array!");
		
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions);

		// Loop through the extensions, adding them to the set if they are mandatory or optional
		VkExtensionProperties* extensionsEnd = extensions + extensionCount;
		for(VkExtensionProperties* extension = extensions; extension != extensionsEnd; ++extension) {
			// Check if the current extension is mandatory
			auto mandatoryExtension = MANDATORY_DEVICE_EXTENSIONS.find(extension->extensionName);
			if(mandatoryExtension != MANDATORY_DEVICE_EXTENSIONS.end()) {
				// Add the current extension to the set
				deviceExtensions.insert(*mandatoryExtension);

				continue;
			}

			// Check if the current extension if optional
			auto optionalExtension = OPTIONAL_DEVICE_EXTENSIONS.find(extension->extensionName);
			if(optionalExtension != OPTIONAL_DEVICE_EXTENSIONS.end()) {
				// Add the current extension to the set
				deviceExtensions.insert(*optionalExtension);

				continue;
			}
		}

		// Free the extension array
		free(extensions, MEMORY_USAGE_ARRAY);

		WFE_LOG_INFO("Picked Vulkan physical device: %s", deviceProperties.deviceName);
		WFE_LOG_INFO("Vulkan physical device queue families: graphics: %u; present: %u; transfer: %u; compute: %u", queueFamilies.graphicsQueueIndex, queueFamilies.presentQueueIndex, queueFamilies.transferQueueIndex, queueFamilies.computeQueueIndex);
	}
	static void CreateLogicalDevice() {
		// Retrieve the number of queue families
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		// Allocate the array of queue families and queue family counts
		VkQueueFamilyProperties* queueFamilyProperties = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties) + queueFamilyCount * sizeof(uint32_t), MEMORY_USAGE_ARRAY);
		if(!queueFamilyProperties)
			WFE_LOG_FATAL("Failed to allocate Vulkan queue family property array!");
		uint32_t* queueFamilyCounts = (uint32_t*)(queueFamilyProperties + queueFamilyCount);
		
		// Fill the queue family array
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties);
		
		// Fill the queue family count array
		uint32_t* queueFamilyCountsEnd = queueFamilyCounts + queueFamilyCount;
		for(uint32_t* familyCount = queueFamilyCounts; familyCount != queueFamilyCountsEnd; ++familyCount)
			*familyCount = 0;
		
		++queueFamilyCounts[queueFamilies.graphicsQueueIndex];
		++queueFamilyCounts[queueFamilies.presentQueueIndex];
		++queueFamilyCounts[queueFamilies.transferQueueIndex];
		++queueFamilyCounts[queueFamilies.computeQueueIndex];
		
		// Check if any queue family count surpasses its limit
		for(uint32_t* familyCount = queueFamilyCounts; familyCount != queueFamilyCountsEnd; ++familyCount)
			if(*familyCount > queueFamilyProperties->queueCount)
				*familyCount = queueFamilyProperties->queueCount;

		// Set all queue create infos
		uint32_t queueCreateInfoCount = 0;
		VkDeviceQueueCreateInfo queueCreateInfos[4];

		float32_t queuePriotities[4]{ 1.f, 1.f, 1.f, 1.f };

		for(uint32_t i = 0; i != queueFamilyCount; ++i) {
			// Move on to the next family count if there are no queues for the current family
			if(!queueFamilyCounts[i])
				continue;

			// Set the current queue create info
			queueCreateInfos[queueCreateInfoCount].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfos[queueCreateInfoCount].pNext = nullptr;
			queueCreateInfos[queueCreateInfoCount].flags = 0;
			queueCreateInfos[queueCreateInfoCount].queueFamilyIndex = i;
			queueCreateInfos[queueCreateInfoCount].queueCount = queueFamilyCounts[i];
			queueCreateInfos[queueCreateInfoCount].pQueuePriorities = queuePriotities;

			// Increment the queue create info count
			++queueCreateInfoCount;
		}

		// Set the device create info
		VkDeviceCreateInfo createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.queueCreateInfoCount = queueCreateInfoCount;
		createInfo.pQueueCreateInfos = queueCreateInfos;
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledExtensionCount = deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.begin();
		createInfo.pEnabledFeatures = &deviceFeatures;

		// Create the device
		VkResult result = vkCreateDevice(physicalDevice, &createInfo, GetVulkanAllocCallbacks(), &device);
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Vulkan logical device! Error code: ", string_VkResult(result));
		
		// Load the Vulkan device functions
		LoadVulkanDeviceFunctions(device);
		
		// Retrieve the handles of every supported queue
		uint32_t graphicsQueueIndex = queueFamilyCounts[queueFamilies.graphicsQueueIndex];
		graphicsQueueIndex -= graphicsQueueIndex != 0;
		queueFamilyCounts[queueFamilies.graphicsQueueIndex] = graphicsQueueIndex;
		vkGetDeviceQueue(device, queueFamilies.graphicsQueueIndex, graphicsQueueIndex, &graphicsQueue);

		uint32_t presentQueueIndex = queueFamilyCounts[queueFamilies.presentQueueIndex];
		presentQueueIndex -= presentQueueIndex != 0;
		queueFamilyCounts[queueFamilies.presentQueueIndex] = presentQueueIndex;
		vkGetDeviceQueue(device, queueFamilies.presentQueueIndex, presentQueueIndex, &presentQueue);

		uint32_t transferQueueIndex = queueFamilyCounts[queueFamilies.transferQueueIndex];
		transferQueueIndex -= transferQueueIndex != 0;
		queueFamilyCounts[queueFamilies.transferQueueIndex] = transferQueueIndex;
		vkGetDeviceQueue(device, queueFamilies.transferQueueIndex, transferQueueIndex, &transferQueue);

		uint32_t computeQueueIndex = queueFamilyCounts[queueFamilies.computeQueueIndex];
		computeQueueIndex -= computeQueueIndex != 0;
		queueFamilyCounts[queueFamilies.computeQueueIndex] = computeQueueIndex;
		vkGetDeviceQueue(device, queueFamilies.computeQueueIndex, computeQueueIndex, &computeQueue);

		// Free the queue family property array
		free(queueFamilyProperties, MEMORY_USAGE_ARRAY);

		// Count the number of unique queues
		uint32_t uniqueQueueCount = 1 + (presentQueue != graphicsQueue) + (transferQueue != graphicsQueue && transferQueue != presentQueue) + (computeQueue != graphicsQueue && computeQueue != presentQueue && computeQueue != transferQueue);

		WFE_LOG_INFO("Create Vulkan logical device with %u unique queues.", uniqueQueueCount);
	}

	// Public functions
	bool8_t CreateVulkanDevice() {
		// Try to find a physical device
		if(!FindPhysicalDevice())
			return false;
		
		// A physical device was found; set it up and create the logical device
		SetupPhysicalDevice();
		CreateLogicalDevice();

		return true;
	}
	void DestroyVulkanDevice() {
		// Destroy the logical device
		vkDestroyDevice(device, GetVulkanAllocCallbacks());
	}

	const VkPhysicalDeviceProperties& GetVulkanPhysicalDeviceProperties() {
		return deviceProperties;
	}
	const VkPhysicalDeviceFeatures& GetVulkanPhysicalDeviceFeatures() {
		return deviceFeatures;
	}
	const VkPhysicalDeviceMemoryProperties& GetVulkanPhysicalDeviceMemoryProperties() {
		return deviceMemoryProperties;
	}
	const VulkanQueueFamilyIndices& GetVulkanDeviceQueueFamilyIndices() {
		return queueFamilies;
	}
	const set<const char_t*>& GetVulkanDeviceExtensions() {
		return deviceExtensions;
	}

	uint32_t FindVulkanMemoryType(uint32_t startIndex, uint32_t typeBitmask, VkMemoryPropertyFlags properties) {
		// Loop through every available memory type, starting at the given index
		for(uint32_t i = startIndex; i != deviceMemoryProperties.memoryTypeCount; ++i) {
			// Check if the current memory type supports all the given properties
			if(typeBitmask & (1 << i) && deviceMemoryProperties.memoryTypes[i].propertyFlags & properties == properties)
				return i;
		}

		// No supported memory format exists; return -1
		return -1;
	}
	VkFormat FindVulkanSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		// Call the internal format function with the current physical device
		return FindSupportedFormat(physicalDevice, candidates, tiling, features);
	}

	VkPhysicalDevice GetVulkanPhysicalDevice() {
		return physicalDevice;
	}
	VkDevice GetVulkanDevice() {
		return device;
	}
	VkQueue GetVulkanGraphicsQueue() {
		return graphicsQueue;
	}
	VkQueue GetVulkanPresentQueue() {
		return presentQueue;
	}
	VkQueue GetVulkanTransferQueue() {
		return transferQueue;
	}
	VkQueue GetVulkanComputeQueue() {
		return computeQueue;
	}
}