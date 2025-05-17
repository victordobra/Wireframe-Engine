#pragma once

#include "Core/Types/Defines.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

namespace wfe {
	/// @brief A class implementing a Vulkan logical device.
	class VulkanDevice {
	public:
		/// @brief A struct containing the device's queues and their queue family indices.
		struct DeviceQueues {
			/// @brief The graphics queue family index.
			uint32_t graphicsIndex;
			/// @brief The present queue family index.
			uint32_t presentIndex;
			/// @brief The transfer queue family index.
			uint32_t transferIndex;
			/// @brief The compute queue family index.
			uint32_t computeIndex;
			/// @brief The graphics queue.
			VkQueue graphicsQueue;
			/// @brief The present queue.
			VkQueue presentQueue;
			/// @brief The transfer queue.
			VkQueue transferQueue;
			/// @brief The compute queue.
			VkQueue computeQueue;
		};

		/// @brief A struct chain containing all required features for the Vulkan device.
		static const VkPhysicalDeviceFeatures2 DEFAULT_REQUIRED_DEVICE_FEATURES;
		/// @brief A struct chain containing all optional features for the Vulkan device.
		static const VkPhysicalDeviceFeatures2 DEFAULT_OPTIONAL_DEVICE_FEATURES;
		/// @brief A vector containing all required extensions for the Vulkan device.
		static const std::vector<const char*> DEFAULT_REQUIRED_DEVICE_EXTENSIONS;
		/// @brief A vector containing all optional extensions for the Vulkan device.
		static const std::vector<const char*> DEFAULT_OPTIONAL_DEVICE_EXTENSIONS;

		/// @brief Creates a Vulkan logical device using the most suitable physical device.
		/// @param instance The Vulkan instance owning the logical device.
		/// @param surface The Vulkan surface that the device must support.
		/// @param requiredFeatures All features which must be enabled on device creation. An exception will be thrown if any of the features are not supported.
		/// @param optionalFeatures All features which can be enabled on device creation. Some features might not be enabled, if not supported by the implementation.
		/// @param requiredExtensions All extensions which must be enabled on device creation. An exception will be thrown if any of the extensions are not present.
		/// @param optionalExtensions All extensions which can be enabled on device creation. Some extensions might not be enabled, if not supported by the implementation.
		VulkanDevice(VulkanInstance* instance, VulkanSurface* surface, const VkPhysicalDeviceFeatures2& requiredFeatures = DEFAULT_REQUIRED_DEVICE_FEATURES, const VkPhysicalDeviceFeatures2& optionalFeatures = DEFAULT_OPTIONAL_DEVICE_FEATURES, const std::vector<const char*>& requiredExtensions = DEFAULT_REQUIRED_DEVICE_EXTENSIONS, const std::vector<const char*>& optionalExtensions = DEFAULT_OPTIONAL_DEVICE_EXTENSIONS);
		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice(VulkanDevice&&) = delete;

		VulkanDevice& operator=(const VulkanDevice&) = delete;
		VulkanDevice& operator=(VulkanDevice&&) = delete;

		/// @brief Gets the Vulkan instance that owns the device.
		/// @return The Vulkan instance that owns the device.
		VulkanInstance* GetInstance() const {
			return instance;
		}
		/// @brief Gets the Vulkan loader including this device's functions.
		/// @return The Vulkan loader including this device's functions.
		VulkanLoader* GetLoader() const {
			return instance->GetLoader();
		}

		/// @brief Gets the internal handle of the Vulkan physical device.
		/// @return The internal handle of the Vulkan physical device.
		VkPhysicalDevice GetPhysicalDevice() const {
			return physicalDevice;
		}
		/// @brief Gets the internal handle of the Vulkan logical device.
		/// @return The internal handle of the Vulkan logical device.
		VkDevice GetDevice() const {
			return device;
		}
		/// @brief Gets the Vulkan device's queues and their corresponding queue family indices.
		/// @return A struct containing the Vulkan device's queues and their corresponding queue family indices.
		const DeviceQueues& GetDeviceQueues() const {
			return queues;
		}
		/// @brief Gets the Vulkan device's properties.
		/// @return A struct containing the Vulkan device's properties.
		const VkPhysicalDeviceProperties& GetDeviceProperties() const {
			return deviceProperties;
		}
		/// @brief Gets the Vulkan device's memory properties.
		/// @return A struct containing the Vulkan device's memory properties.
		const VkPhysicalDeviceMemoryProperties& GetDeviceMemoryProperties() const {
			return deviceMemoryProperties;
		}
		/// @brief Gets the Vulkan device's features.
		/// @return A struct containing the Vulkan device's features.
		const VkPhysicalDeviceFeatures2& GetDeviceFeatures() const {
			return deviceFeatures;
		}
		/// @brief Gets the Vulkan device's enabled extensions.
		/// @return A vector containing the names of all extensions.
		const std::vector<const char*>& GetEnabledExtensions() const {
			return enabledExtensions;
		}

		/// @brief Destroys the Vulkan logical device.
		~VulkanDevice();
	private:
		bool GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VulkanSurface* surface, uint32_t& graphicsIndex, uint32_t& presentIndex, uint32_t& transferIndex, uint32_t& computeIndex);
		bool GetDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2& supportedFeatures, const VkPhysicalDeviceFeatures2& requiredFeatures, const VkPhysicalDeviceFeatures2& optionalFeatures);
		bool GetDeviceExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& supportedExtensions, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions);
		void SetBestDevice(VulkanSurface* surface, const VkPhysicalDeviceFeatures2& requiredFeatures, const VkPhysicalDeviceFeatures2& optionalFeatures, const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions);
		void CreateLogicalDevice();

		VulkanInstance* instance;

		VkPhysicalDevice physicalDevice;
		VkDevice device;
		DeviceQueues queues;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
		VkPhysicalDeviceFeatures2 deviceFeatures;
		std::vector<const char*> enabledExtensions;
	};
}