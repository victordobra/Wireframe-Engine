#pragma once

#include "VulkanAllocator.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSurface.hpp"

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A wrapper for a Vulkan swap chain.
	class VulkanSwapChain {
	public:
		/// @brief A struct containing the swap chain's settings.
		struct SwapChainSettings {
			/// @brief The format of the color images.
			VkFormat imageFormat;
			/// @brief The color space of the color images.
			VkColorSpaceKHR colorSpace;
			/// @brief The format of the depth images.
			VkFormat depthFormat;
			/// @brief The alpha composite mode to use.
			VkCompositeAlphaFlagBitsKHR compositeAlpha;
			/// @brief The swap chain's present mode.
			VkPresentModeKHR presentMode;
			/// @brief VK_TRUE if the swap chain can be clipped, otherwise VK_FALSE.
			VkBool32 clipped;
		};
		struct SwapChainImage {
			/// @brief The color image.
			VkImage image;
			/// @brief The color image's view.
			VkImageView imageView;
			/// @brief The depth image.
			VkImage depthImage;
			/// @brief The depth image's memory block.
			VulkanAllocator::MemoryBlock depthImageMemory;
			/// @brief The depth image's view.
			VkImageView depthImageView;
			/// @brief The swap chain image's framebuffer.
			VkFramebuffer framebuffer;
		};

		/// @brief Generates the Vulkan swap chain's default settings.
		/// @param surface The Vulkan surface of the swap chain.
		/// @param device The Vulkan device which will own the swap chain.
		/// @return A struct containing the Vulkan swap chain's default settings.
		static SwapChainSettings GetDefaultSwapChainSettings(VulkanSurface* surface, VulkanDevice* device);
		/// @brief Checks if the given Vulkan swap chain settings are supported.
		/// @param surface The Vulkan surface of the swap chain.
		/// @param device The Vulkan device which will own the swap chain.
		/// @param settings The Vulkan swap chain settings to test.
		/// @return True if the given settings are supported, otherwise false.
		static bool8_t CheckSwapChainSettingsSupport(VulkanSurface* surface, VulkanDevice* device, const SwapChainSettings& settings);

		/// @brief Creates a Vulkan swap chain.
		/// @param surface The Vulkan surface of the swap chain.
		/// @param device The Vulkan device which will own the swap chain.
		/// @param allocator The Vulkan allocator to use for the swap chain's depth images.
		/// @param swapChainSettings The Vulkan swap chain's settings.
		VulkanSwapChain(VulkanSurface* surface, VulkanDevice* device, VulkanAllocator* allocator);
		/// @brief Creates a Vulkan swap chain with the given settings.
		/// @param surface The Vulkan surface of the swap chain.
		/// @param device The Vulkan device which will own the swap chain.
		/// @param allocator The Vulkan allocator to use for the swap chain's depth images.
		/// @param swapChainSettings The Vulkan swap chain's settings.
		VulkanSwapChain(VulkanSurface* surface, VulkanDevice* device, VulkanAllocator* allocator, const SwapChainSettings& swapChainSettings);
		VulkanSwapChain(const VulkanSwapChain&) = delete;
		VulkanSwapChain(VulkanSwapChain&&) noexcept = delete;

		VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
		VulkanSwapChain& operator=(VulkanSwapChain&&) = delete;

		/// @brief Recreates the Vulkan swap chain.
		void RecreateSwapChain();

		/// @brief Get the Vulkan swap chain's settings.
		/// @return A struct containing the Vulkan swap chain's settings.
		const SwapChainSettings& GetSwapChainSettings() const {
			return settings;
		}
		/// @brief Sets the Vulkan swap chain's settings.
		/// @param newSettings A struct containing the Vulkan swpa chain's new settings.
		/// @return True if the given settings are supported, otherwise false.
		bool8_t SetSwapChainSettings(const SwapChainSettings& newSettings);

		/// @brief Gets the Vulkan swap chain's extent.
		/// @return The Vulkan swap chain's extent.
		VkExtent2D GetVulkanSwapChainExtent() const {
			return swapChainExtent;
		}
		/// @brief Gets the Vulkan swap chain's handle.
		/// @return The Vulkan swap chain's handle, or VK_NULL_HANDLE if the swap chain's window is minimized.
		VkSwapchainKHR GetVulkanSwapChain() {
			return swapChain;
		}
		/// @brief Gets the Vulkan swap chain's images.
		/// @return The Vulkan swap chain's images.
		const vector<SwapChainImage>& GetSwapChainImages() {
			return swapChainImages;
		}
		/// @brief Gets the Vulkan render pass's handle.
		/// @return The Vulkan render pass's handle.
		VkRenderPass GetRenderPass() {
			return renderPass;
		}

		/// @brief Destroys the Vulkan swap chain.
		~VulkanSwapChain();
	private:
		void CreateSwapChain(VkSwapchainKHR oldSwapChain);
		void GetSwapChainImageViews();
		void CreateSwapChainDepthImages();
		void CreateRenderPass();
		void CreateFramebuffers();
		void DestroySwapChain();

		VulkanSurface* surface;
		VulkanDevice* device;
		VulkanAllocator* allocator;

		SwapChainSettings settings;
		VkExtent2D swapChainExtent;
		VkSwapchainKHR swapChain;
		vector<SwapChainImage> swapChainImages;
		VkRenderPass renderPass;
	};
}