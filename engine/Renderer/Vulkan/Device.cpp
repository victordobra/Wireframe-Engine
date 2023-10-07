#include "Device.hpp"
#include "Allocator.hpp"
#include "Instance.hpp"
#include "SwapChain.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Structs
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
		
	};

	// Internal variables
	static VkPhysicalDeviceProperties deviceProperties;
	static VkPhysicalDeviceFeatures deviceFeatures;
	static VulkanQueueFamilyIndices queueFamilies;
	static set<const char_t*> deviceExtensions;

	static VkPhysicalDevice physicalDevice;
	static VkDevice device;
	static VkQueue graphicsQueue;
	static VkQueue presentQueue;
	static VkQueue transferQueue = VK_NULL_HANDLE;
	static VkQueue computeQueue = VK_NULL_HANDLE;

	// Internal helper functions
	static VulkanQueueFamilyIndices GetQueueFamilies(VkPhysicalDevice physicalDevice) {
		// Retrieve the number of queue families
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		// Allocate the array of queue families and fill it
		VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties), MEMORY_USAGE_ARRAY);
		if(!queueFamilies)
			throw BadAllocException("Failed to allocate array!");
		
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
			throw BadAllocException("Failed to allocate array!");
		
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
		
		// Get the current physical device's queue family indices
		VulkanQueueFamilyIndices families = GetQueueFamilies(physicalDevice);

		// Exit the function if no suitable graphics or present queue families were found
		if(families.graphicsQueueIndex == -1 || families.presentQueueIndex == -1)
			return 0;
		
		// Award extra score if transfer and compute queue families were found
		score += (families.transferQueueIndex != -1) * TRANSFER_QUEUE_FAMILY_SCORE_INCREASE;
		score += (families.computeQueueIndex != -1) * COMPUTE_QUEUE_FAMILY_SCORE_INCREASE;

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
			throw BadAllocException("Failed to allocate array!");
		
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
		// Get the phyiscal device's properties, features and queue families
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		queueFamilies = GetQueueFamilies(physicalDevice);

		// Load the physical device's extensions count
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		// Allocate the array of extesnions and fill it
		VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties), MEMORY_USAGE_ARRAY);
		if(!extensions)
			throw BadAllocException("Failed to allocate array!");
		
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
	}
	static void CreateLogicalDevice() {
		// Retrieve the number of queue families
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		// Allocate the array of queue families and queue family counts
		VkQueueFamilyProperties* queueFamilyProperties = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties) + queueFamilyCount * sizeof(uint32_t), MEMORY_USAGE_ARRAY);
		if(!queueFamilyProperties)
			throw BadAllocException("Failed to allocate array!");
		uint32_t* queueFamilyCounts = (uint32_t*)(queueFamilyProperties + queueFamilyCount);
		
		// Fill the queue family array
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties);
		
		// Fill the queue family count array
		uint32_t* queueFamilyCountsEnd = queueFamilyCounts + queueFamilyCount;
		for(uint32_t* familyCount = queueFamilyCounts; familyCount != queueFamilyCountsEnd; ++familyCount)
			*familyCount = 0;
		
		++queueFamilyCounts[queueFamilies.graphicsQueueIndex];
		++queueFamilyCounts[queueFamilies.presentQueueIndex];
		if(queueFamilies.transferQueueIndex != -1)
			++queueFamilyCounts[queueFamilies.transferQueueIndex];
		if(queueFamilies.computeQueueIndex != -1)
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
		
		// Retrieve the handles of every supported queue
		uint32_t graphicsQueueIndex = queueFamilyCounts[queueFamilies.graphicsQueueIndex];
		graphicsQueueIndex -= graphicsQueueIndex != 0;
		queueFamilyCounts[queueFamilies.graphicsQueueIndex] = graphicsQueueIndex;
		vkGetDeviceQueue(device, queueFamilies.graphicsQueueIndex, graphicsQueueIndex, &graphicsQueue);

		uint32_t presentQueueIndex = queueFamilyCounts[queueFamilies.presentQueueIndex];
		presentQueueIndex -= presentQueueIndex != 0;
		queueFamilyCounts[queueFamilies.presentQueueIndex] = presentQueueIndex;
		vkGetDeviceQueue(device, queueFamilies.presentQueueIndex, presentQueueIndex, &presentQueue);

		if(queueFamilies.transferQueueIndex) {
			uint32_t transferQueueIndex = queueFamilyCounts[queueFamilies.transferQueueIndex];
			transferQueueIndex -= transferQueueIndex != 0;
			queueFamilyCounts[queueFamilies.transferQueueIndex] = transferQueueIndex;
			vkGetDeviceQueue(device, queueFamilies.transferQueueIndex, transferQueueIndex, &transferQueue);
		}

		if(queueFamilies.computeQueueIndex) {
			uint32_t computeQueueIndex = queueFamilyCounts[queueFamilies.computeQueueIndex];
			computeQueueIndex -= computeQueueIndex != 0;
			queueFamilyCounts[queueFamilies.computeQueueIndex] = computeQueueIndex;
			vkGetDeviceQueue(device, queueFamilies.computeQueueIndex, computeQueueIndex, &computeQueue);
		}

		// Free the queue family property array
		free(queueFamilyProperties, MEMORY_USAGE_ARRAY);
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
	const VulkanQueueFamilyIndices& GetVulkanDeviceQueueFamilyIndices() {
		return queueFamilies;
	}
	const set<const char_t*>& GetVulkanDeviceExtensions() {
		return deviceExtensions;
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