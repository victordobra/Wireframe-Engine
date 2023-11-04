#include "SwapChain.hpp"
#include "Allocator.hpp"
#include "Device.hpp"
#include "Instance.hpp"
#include "RenderPass.hpp"
#include "Surface.hpp"
#include "Platform/Window.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Internal variables
	static VkSurfaceCapabilitiesKHR surfaceCapabilities;

	static VulkanSwapChainSettings swapChainSettings;
	static VkExtent2D swapChainExtent;
	static VkSwapchainKHR swapChain;
	static vector<VulkanSwapChainImage> swapChainImages;
	static VkFormat swapChainDepthFormat;

	// Internal functions
	static void SetSwapChainDefaultSettings() {
		// Set the swap chain's surface format based on their scores
		swapChainSettings.imageFormat = GetVulkanSurfaceSupportedFormats()[0].format;
		swapChainSettings.imageColorSpace = GetVulkanSurfaceSupportedFormats()[0].colorSpace;

		uint32_t maxScore = 0;

		for(const auto& surfaceFormat : GetVulkanSurfaceSupportedFormats()) {
			// Set the current surface format's score
			uint32_t score = 0;

			// Increase the surface format's score if the format is optimal
			score += surfaceFormat.format == VK_FORMAT_R8G8B8A8_SRGB || surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB;

			// Increase the surface format's score if the color space is optimal
			score += surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

			// Check if the current score is the maximum score
			if(score > maxScore) {
				// Set the new max score and surface format
				maxScore = score;
				swapChainSettings.imageFormat = surfaceFormat.format;
				swapChainSettings.imageColorSpace = surfaceFormat.colorSpace;
			}
		}

		// Set the swap chain's present mode to FIFO, as it's guaranteed to be supported
		swapChainSettings.presentMode = VK_PRESENT_MODE_FIFO_KHR;

		// Set the swap chain's other settings
		swapChainSettings.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainSettings.clipped = VK_TRUE;

		// Get the swap chain's depth format
		swapChainDepthFormat = FindVulkanSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}
	static void CreateSwapChain(VkSwapchainKHR oldSwapChain) {
		// Get the surface's capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GetVulkanPhysicalDevice(), GetVulkanSurface(), &surfaceCapabilities);

		// Set the swap chain's extent
		if(surfaceCapabilities.currentExtent.width == UINT32_T_MAX || surfaceCapabilities.currentExtent.height == UINT32_T_MAX) {
			// Set the swap chain extent to the window's size
			WindowInfo windowInfo = GetWindowInfo();

			swapChainExtent.width = windowInfo.width;
			swapChainExtent.height = windowInfo.height;

			// Clamp the extent's width and height between the extent limits
			if(swapChainExtent.width < surfaceCapabilities.minImageExtent.width)
				swapChainExtent.width = surfaceCapabilities.minImageExtent.width;
			else if(swapChainExtent.width > surfaceCapabilities.maxImageExtent.width)
				swapChainExtent.width = surfaceCapabilities.maxImageExtent.width;

			if(swapChainExtent.height < surfaceCapabilities.minImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.minImageExtent.height;
			else if(swapChainExtent.height > surfaceCapabilities.maxImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.maxImageExtent.height;
		} else {
			// Use the surface's extent
			swapChainExtent = surfaceCapabilities.currentExtent;
		}

		// Set the swapchain's handle to VK_NULL_HANDLE and exit the function if the window is minimized
		if(!swapChainExtent.width || !swapChainExtent.height) {
			swapChain = VK_NULL_HANDLE;
			return;
		}

		// Set the swap chain's min image count
		uint32_t swapChainMinImageCount = surfaceCapabilities.minImageCount + 1;
		if(swapChainMinImageCount > surfaceCapabilities.maxImageCount)
			swapChainMinImageCount = surfaceCapabilities.maxImageCount;
		
		// Get the physical device's queue family indices
		VulkanQueueFamilyIndices queueFamilyIndices = GetVulkanDeviceQueueFamilyIndices();

		// Set the swap chain's create info
		VkSwapchainCreateInfoKHR createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.surface = GetVulkanSurface();
		createInfo.minImageCount = swapChainMinImageCount;
		createInfo.imageFormat = swapChainSettings.imageFormat;
		createInfo.imageColorSpace = swapChainSettings.imageColorSpace;
		createInfo.imageExtent = swapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		
		// Set the swap chain's queue family-related info
		uint32_t swapChainQueueFamilyIndices[]{ queueFamilyIndices.graphicsQueueIndex, queueFamilyIndices.presentQueueIndex };

		if(queueFamilyIndices.graphicsQueueIndex != queueFamilyIndices.presentQueueIndex) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = swapChainQueueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 1;
			createInfo.pQueueFamilyIndices = swapChainQueueFamilyIndices;
		}

		createInfo.preTransform = surfaceCapabilities.currentTransform;
		createInfo.compositeAlpha = swapChainSettings.compositeAlpha;
		createInfo.presentMode = swapChainSettings.presentMode;
		createInfo.clipped = swapChainSettings.clipped;
		createInfo.oldSwapchain = oldSwapChain;

		// Create the swap chain
		VkResult result = vkCreateSwapchainKHR(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &swapChain);
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Vulkan swap chain! Error code: %s", string_VkResult(result));
	}
	static void GetSwapChainImages() {
		// Exit the function if the swap chain wasn't created due to the window being minimized
		if(!swapChain)
			return;

		// Get the swap chain's image count
		uint32_t swapChainImageCount;
		vkGetSwapchainImagesKHR(GetVulkanDevice(), swapChain, &swapChainImageCount, nullptr);

		// Allocate the swap chain image array and fill it
		VkImage* swapChainImagesArr = (VkImage*)malloc(sizeof(VkImage) * swapChainImageCount, MEMORY_USAGE_ARRAY);
		if(!swapChainImagesArr)
			WFE_LOG_FATAL("Failed to allocate Vulkan swap chain images array!");
		
		vkGetSwapchainImagesKHR(GetVulkanDevice(), swapChain, &swapChainImageCount, swapChainImagesArr);

		// Copy the images to the swap chain image vector
		swapChainImages.resize(swapChainImageCount);
		for(uint32_t i = 0; i != swapChainImageCount; ++i)
			swapChainImages[i].image = swapChainImagesArr[i];
		
		// Free the swap chain image array
		free(swapChainImagesArr, MEMORY_USAGE_ARRAY);

		// Set the swap image views's create infos
		VkImageViewCreateInfo createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainSettings.imageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		// Create the image views
		for(auto& swapChainImage : swapChainImages) {
			// Set the target image in the create info
			createInfo.image = swapChainImage.image;
			
			// Create the image view
			VkResult result = vkCreateImageView(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &swapChainImage.imageView);
			if(result != VK_SUCCESS)
				WFE_LOG_FATAL("Failed to create Vulkan swap chain image view! Error code: %s", string_VkResult(result));
		} 
	}
	static void CreateDepthImages() {
		// Exit the function if the swap chain wasn't created due to the window being minimized
		if(!swapChain)
			return;

		// Get the device's queue family indices
		VulkanQueueFamilyIndices queueFamilyIndices = GetVulkanDeviceQueueFamilyIndices();

		// Set the depth images create info
		VkImageCreateInfo imageCreateInfo;

		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = swapChainDepthFormat;
		imageCreateInfo.extent = { swapChainExtent.width, swapChainExtent.height, 1 };
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

		uint32_t imageQueueFamilyIndices[]{ queueFamilyIndices.graphicsQueueIndex, queueFamilyIndices.presentQueueIndex };
		if(queueFamilyIndices.graphicsQueueIndex != queueFamilyIndices.presentQueueIndex) {
			imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
			imageCreateInfo.queueFamilyIndexCount = 2;
			imageCreateInfo.pQueueFamilyIndices = imageQueueFamilyIndices;
		} else {
			imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageCreateInfo.queueFamilyIndexCount = 1;
			imageCreateInfo.pQueueFamilyIndices = imageQueueFamilyIndices;
		}

		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		// Create the depth images
		for(auto& swapChainImage : swapChainImages) {
			VkResult result = vkCreateImage(GetVulkanDevice(), &imageCreateInfo, GetVulkanAllocCallbacks(), &swapChainImage.depthImage);
			if(result != VK_SUCCESS)
				WFE_LOG_FATAL("Failed to create Vulkan swap chain depth image! Error code: %s", string_VkResult(result));
		}

		// Check if dedicated allocations are supported
		if(GetVulkanPhysicalDeviceProperties().apiVersion >= VK_API_VERSION_1_1 || GetVulkanDeviceExtensions().find(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME)) {
			// Add the dedicated memory requirements struct to the memory requirements
			VkMemoryDedicatedRequirements dedicatedMemoryRequirements;

			dedicatedMemoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;
			dedicatedMemoryRequirements.pNext = nullptr;

			VkMemoryRequirements2 memoryRequirements;

			memoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
			memoryRequirements.pNext = &dedicatedMemoryRequirements;

			// Get the image's memory requirements
			VkImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo;

			imageMemoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
			imageMemoryRequirementsInfo.pNext = nullptr;
			imageMemoryRequirementsInfo.image = swapChainImages[0].depthImage;

			vkGetImageMemoryRequirements2(GetVulkanDevice(), &imageMemoryRequirementsInfo, &memoryRequirements);

			// Check if the depth images preffer or require dedicated allocations
			if(dedicatedMemoryRequirements.prefersDedicatedAllocation || dedicatedMemoryRequirements.requiresDedicatedAllocation) {
				// Set the dedicated alloc info
				VkMemoryDedicatedAllocateInfo dedicatedAllocInfo;

				dedicatedAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO;
				dedicatedAllocInfo.pNext = nullptr;
				dedicatedAllocInfo.buffer = VK_NULL_HANDLE;

				// Set the alloc info
				VkMemoryAllocateInfo allocInfo;

				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.pNext = nullptr;
				allocInfo.allocationSize = memoryRequirements.memoryRequirements.size;

				// Allocate the memories for every depth image
				for(auto& swapChainImage : swapChainImages) {
					// Allocate the current depth image's memory
					VkResult result = AllocVulkanDeviceMemory(allocInfo, memoryRequirements.memoryRequirements.memoryTypeBits, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, swapChainImage.depthImageMemory.memory);
					if(result != VK_SUCCESS)
						WFE_LOG_FATAL("Failed to allocate Vulkan depth image memory! Error code: %s", string_VkResult(result));

					// Set the current depth image's memory range
					swapChainImage.depthImageMemory.offset = 0;
					swapChainImage.depthImageMemory.size = allocInfo.allocationSize;
				}
			} else {
				// Calculate the total required size for every image
				VkDeviceSize size = memoryRequirements.memoryRequirements.size;
				VkDeviceSize alignment = memoryRequirements.memoryRequirements.alignment;
				VkDeviceSize alignedSize = (size + alignment - 1) & ~alignment;

				// Set the alloc info
				VkMemoryAllocateInfo allocInfo;

				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.pNext = nullptr;
				allocInfo.allocationSize = alignedSize * swapChainImages.size();

				// Allocate the depth images's memory
				VkDeviceMemory depthImageMemory;

				VkResult result = AllocVulkanDeviceMemory(allocInfo, memoryRequirements.memoryRequirements.memoryTypeBits, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, depthImageMemory);
				if(result != VK_SUCCESS)
					WFE_LOG_FATAL("Failed to allocate Vulkan depth image memory! Error code: %s", string_VkResult(result));
				
				// Set the swap chain depth images' memory ranges
				for(size_t i = 0; i != swapChainImages.size(); ++i) {
					swapChainImages[i].depthImageMemory.memory = depthImageMemory;
					swapChainImages[i].depthImageMemory.offset = alignedSize * i;
					swapChainImages[i].depthImageMemory.size = size;
				}
			}
		} else {
			// Get the image's memory requirements
			VkMemoryRequirements memoryRequirements;
			vkGetImageMemoryRequirements(GetVulkanDevice(), swapChainImages[0].depthImage, &memoryRequirements);

			// Calculate the total required size for every image
			VkDeviceSize size = memoryRequirements.size;
			VkDeviceSize alignment = memoryRequirements.alignment;
			VkDeviceSize alignedSize = (size + alignment - 1) & ~(alignment - 1);

			// Set the alloc info
			VkMemoryAllocateInfo allocInfo;

			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.pNext = nullptr;
			allocInfo.allocationSize = alignedSize * swapChainImages.size();

			// Allocate the depth images's memory
			VkDeviceMemory depthImageMemory;

			VkResult result = AllocVulkanDeviceMemory(allocInfo, memoryRequirements.memoryTypeBits, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, depthImageMemory);
			if(result != VK_SUCCESS)
				WFE_LOG_FATAL("Failed to allocate Vulkan depth image memory! Error code: %s", string_VkResult(result));
			
			// Set the swap chain depth images' memory ranges
			for(size_t i = 0; i != swapChainImages.size(); ++i) {
				swapChainImages[i].depthImageMemory.memory = depthImageMemory;
				swapChainImages[i].depthImageMemory.offset = alignedSize * i;
				swapChainImages[i].depthImageMemory.size = size;
			}
		}

		// Check if batched memory binding is supported
		if(GetVulkanPhysicalDeviceProperties().apiVersion >= VK_API_VERSION_1_1 || GetVulkanDeviceExtensions().count(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME)) {
			// Allocate the memory bind info array
			VkBindImageMemoryInfo* memoryBindInfo = (VkBindImageMemoryInfo*)malloc(sizeof(VkBindImageMemoryInfo) * swapChainImages.size(), MEMORY_USAGE_ARRAY);
			if(!memoryBindInfo)
				WFE_LOG_FATAL("Failed to allocate Vulkan swap chain depth image memory bind info array!");
			
			// Set the memory bind infos
			for(size_t i = 0; i != swapChainImages.size(); ++i) {
				memoryBindInfo[i].sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
				memoryBindInfo[i].pNext = nullptr;
				memoryBindInfo[i].image = swapChainImages[i].depthImage;
				memoryBindInfo[i].memory = swapChainImages[i].depthImageMemory.memory;
				memoryBindInfo[i].memoryOffset = swapChainImages[i].depthImageMemory.offset;
			}

			// Bind the image memories
			VkResult result = vkBindImageMemory2(GetVulkanDevice(), (uint32_t)swapChainImages.size(), memoryBindInfo);
			if(result != VK_SUCCESS)
				WFE_LOG_FATAL("Failed to bind Vulkan swap chain depth image memories! Error code: %s", string_VkResult(result));
			
			// Free the memory bind info array
			free(memoryBindInfo, MEMORY_USAGE_ARRAY);
		} else {
			// Bind every image memory
			for(auto& swapChainImage : swapChainImages) {
				VkResult result = vkBindImageMemory(GetVulkanDevice(), swapChainImage.depthImage, swapChainImage.depthImageMemory.memory, swapChainImage.depthImageMemory.offset);
				if(result != VK_SUCCESS)
					WFE_LOG_FATAL("Failed to bind Vulkan swap chain depth image memories! Error code: %s", string_VkResult(result));
			}
		}

		// Set the depth image views create info
		VkImageViewCreateInfo imageViewCreateInfo;

		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = nullptr;
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = swapChainDepthFormat;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		// Check if the stencil aspect flag must be added
		if(swapChainDepthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT || swapChainDepthFormat == VK_FORMAT_D24_UNORM_S8_UINT)
			imageViewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		
		// Create the depth image views
		for(auto& swapChainImage : swapChainImages) {
			// Set the target image in the create info
			imageViewCreateInfo.image = swapChainImage.depthImage;

			// Create the image view
			VkResult result = vkCreateImageView(GetVulkanDevice(), &imageViewCreateInfo, GetVulkanAllocCallbacks(), &swapChainImage.depthImageView);
			if(result != VK_SUCCESS)
				WFE_LOG_FATAL("Failed to create Vulkan swap chain depth image view! Error code: %s", string_VkResult(result));
		}
	}
	static void CreateFramebuffers() {
		// Exit the function if the swap chain wasn't created due to the window being minimized
		if(!swapChain)
			return;

		// Set the framebuffer create info
		VkFramebufferCreateInfo createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.renderPass = GetVulkanRenderPass();
		createInfo.attachmentCount = 2;
		createInfo.width = swapChainExtent.width;
		createInfo.height = swapChainExtent.height;
		createInfo.layers = 1;

		// Create every framebuffer
		for(auto& swapChainImage : swapChainImages) {
			// Set the framebuffer attachments
			VkImageView attachments[2]{ swapChainImage.imageView, swapChainImage.depthImageView };
			createInfo.pAttachments = attachments;

			// Create the current framebuffer
			VkResult result = vkCreateFramebuffer(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &swapChainImage.framebuffer);
			if(result != VK_SUCCESS)
				WFE_LOG_FATAL("Failed to create Vulkan framebuffer! Error code: %s", string_VkResult(result));
		}
	}

	static void DestroySwapChainImages() {
		// Exit the function if the swap chain wasn't created due to the window being minimized
		if(!swapChain)
			return;

		// Destroy the swap chain's images
		for(auto& swapChainImage : swapChainImages) {
			// Destroy the framebuffer
			vkDestroyFramebuffer(GetVulkanDevice(), swapChainImage.framebuffer, GetVulkanAllocCallbacks());

			// Destroy the depth image view and image
			vkDestroyImageView(GetVulkanDevice(), swapChainImage.depthImageView, GetVulkanAllocCallbacks());
			vkDestroyImage(GetVulkanDevice(), swapChainImage.depthImage, GetVulkanAllocCallbacks());

			// Destroy the image view
			vkDestroyImageView(GetVulkanDevice(), swapChainImage.imageView, GetVulkanAllocCallbacks());
		}

		// Check if the depth image memory is common or not
		if(swapChainImages.size() == 1 || swapChainImages[0].depthImageMemory.memory == swapChainImages[1].depthImageMemory.memory) {
			// The depth image memory is common; free the singular block
			vkFreeMemory(GetVulkanDevice(), swapChainImages[0].depthImageMemory.memory, GetVulkanAllocCallbacks());
		} else {
			// The depth image memory is not commot; free every memory block
			for(auto& swapChainImage : swapChainImages)
				vkFreeMemory(GetVulkanDevice(), swapChainImage.depthImageMemory.memory, GetVulkanAllocCallbacks());
		}

		// Resize the swap chain image vector to 0
		swapChainImages.resize(0);
	}

	// Resize event callback
	static void* ResizeEventCallback(void* params) {
		// Recreate the swap chain
		RecreateVulkanSwapChain();

		return nullptr;
	}

	// Public functions
	void CreateVulkanSwapChain() {
		// Set the swap chain's default settings
		SetSwapChainDefaultSettings();

		// Create the render pass
		CreateVulkanRenderPass();

		// Create the swap chain and its components
		CreateSwapChain(VK_NULL_HANDLE);
		GetSwapChainImages();
		CreateDepthImages();
		CreateFramebuffers();

		// Add the resize event callback
		GetWindowResizeEvent().AddListener(ResizeEventCallback);

		WFE_LOG_INFO("Created Vulkan swap chain with %u images.", (uint32_t)swapChainImages.size());
	}
	void DestroyVulkanSwapChain() {
		// Destroy the render pass
		DestroyVulkanRenderPass();

		// Destroy the swap chain's images
		DestroySwapChainImages();

		// Destroy the swap chain, if it exists
		if(swapChain)
			vkDestroySwapchainKHR(GetVulkanDevice(), swapChain, GetVulkanAllocCallbacks());
	}
	void RecreateVulkanSwapChain() {
		// Destroy the swap chain's previous images
		DestroySwapChainImages();

		// Save the old swap chain's handle
		VkSwapchainKHR oldSwapChain = swapChain;

		// Recreate the swap chain
		CreateSwapChain(oldSwapChain);

		// Destroy the old swap chain, if it exists
		if(oldSwapChain)
			vkDestroySwapchainKHR(GetVulkanDevice(), oldSwapChain, GetVulkanAllocCallbacks());

		// Create the swap chain's remaining components
		GetSwapChainImages();
		CreateDepthImages();
		CreateFramebuffers();
	}

	const VulkanSwapChainSettings& GetVulkanSwapChainSettings() {
		return swapChainSettings;
	}
	bool8_t SetVulkanSwapChainSettings(const VulkanSwapChainSettings& newSettings) {
		// Check if the settings' surface format is supported
		bool8_t surfaceFormatFound = false;
		for(const auto& surfaceFormat : GetVulkanSurfaceSupportedFormats()) {
			// Check if the settings' surface format is equal to the current surface format
			if(surfaceFormat.format == newSettings.imageFormat && surfaceFormat.colorSpace == newSettings.imageColorSpace) {
				// Save that a surface format was found and exit the loop
				surfaceFormatFound = true;
				break;
			}
		}

		// Exit the function if the given surface format is not supported
		if(!surfaceFormatFound)
			return false;

		// Check if the settings' present mode is supported
		bool8_t presentModeFound = false;
		for(auto presentMode : GetVulkanSurfaceSupportedPresentModes()) {
			// Check if the settings' present mode is equal to the current present mode 
			if(presentMode == newSettings.presentMode) {
				// Save that a present mode was found and exit the loop
				presentModeFound = true;
				break;
			}
		}

		// Exit the function if the given present mode is not supported
		if(!presentModeFound)
			return false;
		
		// The settings are supported; recreate the swap chain and exit the function
		swapChainSettings = newSettings;
		RecreateVulkanRenderPass();
		RecreateVulkanSwapChain();

		return true;
	}

	const VkSurfaceCapabilitiesKHR& GetVulkanSurfaceCapabilities() {
		return surfaceCapabilities;
	}
	VkExtent2D GetVulkanSwapChainExtent() {
		return swapChainExtent;
	}
	VkSwapchainKHR GetVulkanSwapChain() {
		return swapChain;
	}
	const vector<VulkanSwapChainImage>& GetVulkanSwapChainImages() {
		return swapChainImages;
	}
	VkFormat GetVulkanSwapChainDepthFormat() {
		return swapChainDepthFormat;
	}
}