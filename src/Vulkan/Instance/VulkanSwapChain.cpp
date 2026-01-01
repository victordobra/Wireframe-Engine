#include "VulkanSwapChain.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include "Vulkan/Resources/VulkanImage.hpp"
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Resize listener
	static void* ResizeListener(void* userData, void* params) {
		// Recreate the swap chain
		VulkanSwapChain* swapChain = (VulkanSwapChain*)userData;
		swapChain->RecreateSwapChain(swapChain->GetPresentMode() == VK_PRESENT_MODE_FIFO_KHR);
		return nullptr;
	}

	// Internal helper functions
	void VulkanSwapChain::GetSwapChainCapabilities() {
		// Get the supported surface formats
		uint32_t formatCount;
		device->GetLoader()->vkGetPhysicalDeviceSurfaceFormatsKHR(device->GetPhysicalDevice(), surface->GetSurface(), &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		device->GetLoader()->vkGetPhysicalDeviceSurfaceFormatsKHR(device->GetPhysicalDevice(), surface->GetSurface(), &formatCount, formats.data());

		// Find the best surface format
		surfaceFormat = formats[0];
		for(const VkSurfaceFormatKHR& format : formats) {
			// Check if the format has the desired image format and color space
			if((format.format == VK_FORMAT_R8G8B8A8_SRGB || format.format == VK_FORMAT_B8G8R8A8_SRGB) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				surfaceFormat = format;
				break;
			}

			// Save the format if it has the desired image format
			if(format.format == VK_FORMAT_R8G8B8A8_SRGB || format.format == VK_FORMAT_B8G8R8A8_SRGB)
				surfaceFormat = format;
		}

		// Get the supported present modes
		uint32_t presentModeCount;
		device->GetLoader()->vkGetPhysicalDeviceSurfacePresentModesKHR(device->GetPhysicalDevice(), surface->GetSurface(), &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		device->GetLoader()->vkGetPhysicalDeviceSurfacePresentModesKHR(device->GetPhysicalDevice(), surface->GetSurface(), &presentModeCount, presentModes.data());

		// Check if VSync and non-VSync are supported
		vsyncSupported = false;
		nonVsyncSupported = false;

		for(const VkPresentModeKHR& mode : presentModes) {
			if(mode == VK_PRESENT_MODE_FIFO_KHR) {
				vsyncSupported = true;
			} else if(mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				nonVsyncSupported = true;
			}

			if(vsyncSupported && nonVsyncSupported)
				break;
		}
	}
	void VulkanSwapChain::CreateSwapChain(VkSwapchainKHR oldSwapChain) {
		// Get the surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		device->GetLoader()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->GetPhysicalDevice(), surface->GetSurface(), &surfaceCapabilities);

		// Set the swap chain's extent
		if(surfaceCapabilities.currentExtent.width == UINT32_MAX || surfaceCapabilities.currentExtent.height == UINT32_MAX) {
			// If the extent is not defined, use the window's size
			extent = { surface->GetWindow()->GetWidth(), surface->GetWindow()->GetHeight() };

			// Clamp the extent to the minimum and maximum extents
			if(extent.width < surfaceCapabilities.minImageExtent.width) {
				extent.width = surfaceCapabilities.minImageExtent.width;
			} else if(extent.width > surfaceCapabilities.maxImageExtent.width) {
				extent.width = surfaceCapabilities.maxImageExtent.width;
			}

			if(extent.height < surfaceCapabilities.minImageExtent.height) {
				extent.height = surfaceCapabilities.minImageExtent.height;
			} else if(extent.height > surfaceCapabilities.maxImageExtent.height) {
				extent.height = surfaceCapabilities.maxImageExtent.height;
			}
		} else {
			// Use the surface's current extent
			extent = surfaceCapabilities.currentExtent;
		}

		// Set the swap chain's image count
		uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
		if(surfaceCapabilities.maxImageCount && imageCount > surfaceCapabilities.maxImageCount)
			imageCount = surfaceCapabilities.maxImageCount;

		// Set the swap chain's image sharing mode
		uint32_t familyIndices[] = { device->GetDeviceQueues().graphicsIndex, device->GetDeviceQueues().presentIndex };
		uint32_t familyCount = 1 + (uint32_t)(familyIndices[0] != familyIndices[1]);
		VkSharingMode sharingMode = (familyCount == 1) ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
		
		// Set the swap chain create info
		VkSwapchainCreateInfoKHR swapChainInfo {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.surface = surface->GetSurface(),
			.minImageCount = imageCount,
			.imageFormat = surfaceFormat.format,
			.imageColorSpace = surfaceFormat.colorSpace,
			.imageExtent = extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = sharingMode,
			.queueFamilyIndexCount = familyCount,
			.pQueueFamilyIndices = familyIndices,
			.preTransform = surfaceCapabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = presentMode,
			.clipped = VK_TRUE,
			.oldSwapchain = oldSwapChain
		};

		// Create the swap chain
		VkResult result = device->GetLoader()->vkCreateSwapchainKHR(device->GetDevice(), &swapChainInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &swapChain);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan swap chain! Error code: " + string_VkResult(result));
	}
	void VulkanSwapChain::CreateSwapChainImages() {
		// Get the swap chain's images
		uint32_t imageCount;
		device->GetLoader()->vkGetSwapchainImagesKHR(device->GetDevice(), swapChain, &imageCount, nullptr);
		std::vector<VkImage> images(imageCount);
		device->GetLoader()->vkGetSwapchainImagesKHR(device->GetDevice(), swapChain, &imageCount, images.data());

		// Set the image view info
		VkImageViewCreateInfo imageViewInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = VK_NULL_HANDLE,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = surfaceFormat.format,
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

		// Set every color image's info
		swapChainImages.resize(imageCount);

		for(uint32_t i = 0; i != imageCount; ++i) {
			// Set the image's handle
			swapChainImages[i].image = images[i];

			// Create the image view
			imageViewInfo.image = images[i];
			VkResult result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &imageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &swapChainImages[i].imageView);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan swap chain image view! Error code: " + string_VkResult(result));
		}
	}
	void VulkanSwapChain::CreateDepthImages() {
		// Check if the device supports the depth format
		VkFormatProperties depthFormatProperties;
		device->GetLoader()->vkGetPhysicalDeviceFormatProperties(device->GetPhysicalDevice(), VK_FORMAT_D32_SFLOAT, &depthFormatProperties);
	
		if(!(depthFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT))
			throw std::runtime_error("The Vulkan device does not support the depth format VK_FORMAT_D32_SFLOAT!");

		// Create all depth images
		for(SwapChainImage& image : swapChainImages) {
			image.depthImage = new VulkanImage(
				device,
				0,
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_D32_SFLOAT,
				{ extent.width, extent.height, 1 }, 1, 1,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_ASPECT_DEPTH_BIT,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				VulkanDevice::QUEUE_TYPE_GRAPHICS,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);
		}
		
		// Set the depth image view info
		VkImageViewCreateInfo depthImageViewInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = VK_NULL_HANDLE,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_D32_SFLOAT,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY
			},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		// Create all depth image views
		for(SwapChainImage& image : swapChainImages) {
			// Create the depth image view
			depthImageViewInfo.image = image.depthImage->GetImage();
			VkResult result = device->GetLoader()->vkCreateImageView(device->GetDevice(), &depthImageViewInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &image.depthImageView);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan swap chain depth image view! Error code: " + string_VkResult(result));
		}
	}

	// Public functions
	VulkanSwapChain::VulkanSwapChain(VulkanDevice* device, VulkanSurface* surface, bool vsync) : device(device), surface(surface) {
		// Get the swap chain's capabilities
		GetSwapChainCapabilities();

		// Set the new present mode
		if(vsync && !vsyncSupported) {
			throw std::runtime_error("VSync was requested, yet is not supported by the Vulkan swap chain!");
		} else if(!vsync && !nonVsyncSupported) {
			throw std::runtime_error("Non-VSync was requested, yet is not supported by the Vulkan swap chain!");
		}

		presentMode = vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR;

		// Create the swap chain
		CreateSwapChain();
		CreateSwapChainImages();
		CreateDepthImages();

		// Add the resize listener to the window
		surface->GetWindow()->GetResizeEvent().AddListener({ ResizeListener, this });
	}

	void VulkanSwapChain::RecreateSwapChain(bool vsync) {
		// Wait for all presents to finish
		device->GetLoader()->vkQueueWaitIdle(device->GetDeviceQueues().presentQueue);

		// Set the new present mode
		if(vsync && !vsyncSupported) {
			throw std::runtime_error("VSync was requested, yet is not supported by the Vulkan swap chain!");
		} else if(!vsync && !nonVsyncSupported) {
			throw std::runtime_error("Non-VSync was requested, yet is not supported by the Vulkan swap chain!");
		}

		presentMode = vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR;

		if(swapChain) {
			// Destroy the old swap chain's images
			for(SwapChainImage& image : swapChainImages) {
				device->GetLoader()->vkDestroyImageView(device->GetDevice(), image.imageView, &VulkanRenderer::ALLOCATION_CALLBACKS);
				delete image.depthImage;
				device->GetLoader()->vkDestroyImageView(device->GetDevice(), image.depthImageView, &VulkanRenderer::ALLOCATION_CALLBACKS);
			}

			swapChainImages.clear();
		}

		// Destroy the old swap chain and exit the function if the window is minimized
		if(surface->GetWindow()->IsMinimized()) {
			if(swapChain) {
				device->GetLoader()->vkDestroySwapchainKHR(device->GetDevice(), swapChain, &VulkanRenderer::ALLOCATION_CALLBACKS);
				swapChain = VK_NULL_HANDLE;
			}
			extent = { 0, 0 };
			return;
		}

		// Create the new swap chain using the old one
		VkSwapchainKHR oldSwapChain = swapChain;
		CreateSwapChain(oldSwapChain);

		if(oldSwapChain != VK_NULL_HANDLE)
			device->GetLoader()->vkDestroySwapchainKHR(device->GetDevice(), oldSwapChain, &VulkanRenderer::ALLOCATION_CALLBACKS);
		
		// Create the swap chain's components
		CreateSwapChainImages();
		CreateDepthImages();
	}

	void VulkanSwapChain::LogInfo(Logger* logger) const {
		// Log the swap chain's information
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)"Using Vulkan swap chain with " + std::to_string(swapChainImages.size()) + " images.");
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)"Vulkan swap chain surface format: " + string_VkFormat(surfaceFormat.format) + ", color space " + string_VkColorSpaceKHR(surfaceFormat.colorSpace) + ".");
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)"VSync is " + (vsyncSupported ? "supported" : "not supported") + ", non-VSync is " + (nonVsyncSupported ? "supported" : "not supported") + ".");
	}

	VulkanSwapChain::~VulkanSwapChain() {
		if(swapChain) {
			// Destroy the swap chain's images
			for(SwapChainImage& image : swapChainImages) {
				device->GetLoader()->vkDestroyImageView(device->GetDevice(), image.imageView, &VulkanRenderer::ALLOCATION_CALLBACKS);
				delete image.depthImage;
				device->GetLoader()->vkDestroyImageView(device->GetDevice(), image.depthImageView, &VulkanRenderer::ALLOCATION_CALLBACKS);
			}

			// Destroy the swap chain
			device->GetLoader()->vkDestroySwapchainKHR(device->GetDevice(), swapChain, &VulkanRenderer::ALLOCATION_CALLBACKS);
		}

		// Remove the resize listener from the window
		surface->GetWindow()->GetResizeEvent().RemoveListener({ ResizeListener, this });
	}
}