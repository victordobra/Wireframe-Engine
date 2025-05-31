#pragma once

#include "Core/Types/Defines.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSurface.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class implementing a Vulkan swap chain.
	class VulkanSwapChain {
	public:
		/// @brief A struct containing the swap chain's color and depth image information.
		struct SwapChainImage {
			/// @brief The color image's handle.
			VkImage image;
			/// @brief The color image's view handle.
			VkImageView imageView;
			/// @brief The color image's attachment info.
			VkRenderingAttachmentInfoKHR attachmentInfo;

			/// @brief The depth image's handle.
			VkImage depthImage;
			/// @brief The depth image's memory block.
			VulkanAllocator::Memory depthImageMemory;
			/// @brief The depth image's view handle.
			VkImageView depthImageView;
			/// @brief The depth image's attachment info.
			VkRenderingAttachmentInfoKHR depthAttachmentInfo;

			/// @brief The semaphore that the swap chain will wait for before presenting the image.
			VkSemaphore renderingFinisedSemaphore;
		};

		/// @brief Creates a Vulkan swap chain.
		/// @param device The Vulkan device to create the swap chain for.
		/// @param surface The Vulkan surface to create the swap chain for.
		/// @param vsync Whether to enable vertical synchronization (VSync) for the swap chain. Defaulted to true.
		VulkanSwapChain(VulkanDevice* device, VulkanSurface* surface, bool vsync = true);
		VulkanSwapChain(const VulkanSwapChain&) = delete;
		VulkanSwapChain(VulkanSwapChain&&) = delete;

		VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
		VulkanSwapChain& operator=(VulkanSwapChain&&) = delete;

		/// @brief Gets the Vulkan device that owns the swap chain.
		/// @return The Vulkan device that owns the swap chain.
		VulkanDevice* GetDevice() const {
			return device;
		}
		/// @brief Gets the Vulkan surface that the swap chain is created for.
		/// @return The Vulkan surface that the swap chain is created for.
		VulkanSurface* GetSurface() const {
			return surface;
		}

		/// @brief Gets the Vulkan swap chain's surface format.
		/// @return The Vulkan swap chain's surface format.
		VkSurfaceFormatKHR GetSurfaceFormat() const {
			return surfaceFormat;
		}
		/// @brief Gets the Vulkan swap chain's present mode.
		/// @return The Vulkan swap chain's present mode.
		VkPresentModeKHR GetPresentMode() const {
			return presentMode;
		}
		/// @brief Checks if VSync is supported.
		/// @return True if VSync is supported, false otherwise.
		bool IsVSyncSupported() const {
			return vsyncSupported;
		}
		/// @brief Checks if non-VSync is supported.
		/// @return True if non-VSync is supported, false otherwise.
		bool IsNonVSyncSupported() const {
			return nonVsyncSupported;
		}
		/// @brief Gets the Vulkan swap chain's image extent.
		/// @return The Vulkan swap chain's image extent.
		VkExtent2D GetExtent() const {
			return extent;
		}
		/// @brief Gets the Vulkan swap chain's internal handle.
		/// @return The Vulkan swap chain's internal handle, or VK_NULL_HANDLE if the window is minimized.
		VkSwapchainKHR GetSwapChain() const {
			return swapChain;
		}
		/// @brief Gets the Vulkan swap chain's images.
		/// @return A vector containing the Vulkan swap chain's images.
		const std::vector<SwapChainImage>& GetSwapChainImages() const {
			return swapChainImages;
		}

		/// @brief Logs informaton about the Vulkana instance to the given logger.
		/// @param logger The logger to log the information to.
		void LogInfo(Logger* logger) const;

		/// @brief Recreates the Vulkan swap chain.
		/// @param vsync Whether to enable vertical synchronization (VSync) for the swap chain. Defaulted to true.
		void RecreateSwapChain(bool vsync = true);

		/// @brief Destroys the Vulkan swap chain.
		~VulkanSwapChain();
	private:
		void GetSwapChainCapabilities();
		void CreateSwapChain(VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE);
		void CreateSwapChainImages();
		void CreateDepthImages();
		void CreateSyncObjects();

		VulkanDevice* device;
		VulkanSurface* surface;

		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR presentMode;
		bool vsyncSupported, nonVsyncSupported;
		VkExtent2D extent;

		VkSwapchainKHR swapChain;
		std::vector<SwapChainImage> swapChainImages;
	};
}