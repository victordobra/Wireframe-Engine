#pragma once

#include "Core/Types/BuildInfo.hpp"
#include "Core/Types/Defines.hpp"
#include "Core/Utils/Logger.hpp"
#include "Vulkan/Loader/VulkanLoader.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

namespace wfe {
	/// @brief A class implementing a Vulkan instance.
	class VulkanInstance {
	public:
		/// @brief The required version to be implemented by the Vulkan API.
		static const uint32_t DEFAULT_REQUIRED_INSTANCE_API_VERSION;
		/// @brief A vector containing all required extensions for the Vulkan instance.
		static const std::vector<const char*> DEFAULT_REQUIRED_INSTANCE_EXTENSIONS;
		/// @brief A vector containing all optional extensions for the Vulkan instance.
		static const std::vector<const char*> DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS;
		/// @brief A vector containing all validation layers to use if debugging is enabled.
		static const std::vector<const char*> DEFAULT_VALIDATION_LAYERS;

		/// @brief Creates a Vulkan instance.
		/// @param loader The Vulkan loader whose functions to use.
		/// @param appInfo A struct containing the application's general info.
		/// @param logger The logger to use for the debug messenger, or nullptr if validation shouldn't be enabled.
		/// @param requiredExtensions All extensions which must be enabled on instance creation. An exception will be thrown if any of the exceptions are not present.
		/// @param optionalExtensions All extensions which can be enabled on instance creation. Some extensions might not be enabled, if not supported by the implementation.
		/// @param validationLayers All validation layers to enable, if debugging if enabled. If any of the layers is not supported, debugging will be disabled. Ignored if debugging is already disabled.
		VulkanInstance(VulkanLoader* loader, const VkApplicationInfo& appInfo, Logger* logger, const std::vector<const char*> requiredExtensions = DEFAULT_REQUIRED_INSTANCE_EXTENSIONS, const std::vector<const char*> optionalExtensions = DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS, const std::vector<const char*> validationLayers = DEFAULT_VALIDATION_LAYERS);
		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) = delete;

		VulkanInstance& operator=(const VulkanInstance&) = delete;
		VulkanInstance& operator=(VulkanInstance&&) = delete;

		/// @brief Gets the Vulkan loader including this instance's functions.
		/// @return The Vulkan loader including this instance's functions.
		VulkanLoader* GetLoader() const {
			return loader;
		}

		/// @brief Gets the internal handle of the Vulkan instance.
		/// @return The internal handle of the Vulkan instance.
		VkInstance GetInstance() const {
			return instance;
		}
		/// @brief Gets the internal handle of the Vulkan debug messenger.
		/// @return The internal handle of the Vulkan debug messenger, or VK_NULL_HANDLE if debugging is not enabled.
		VkDebugUtilsMessengerEXT GetDebugMessenger() const {
			return debugMessenger;
		}
		/// @brief Gets the enabled API version of the Vulkan instance.
		/// @return The enabled API version of the Vulkan instance.
		uint32_t GetAPIVersion() const {
			return apiVersion;
		}
		/// @brief Gets the Vulkan instance's enabled extensions.
		/// @return A vector containing the names of all extensions.
		const std::vector<const char*>& GetEnabledExtensions() const {
			return enabledExtensions;
		}
		/// @brief Gets the Vulkan instance's enabled validation layers.
		/// @return A vector containing the names of all validation layers.
		const std::vector<const char*>& GetEnabledLayers() const {
			return enabledLayers;
		}

		/// @brief Logs informaton about the Vulkana instance to the given logger.
		/// @param logger The logger to log the information to.
		void LogInfo(Logger* logger) const;

		/// @brief Destroys the Vulkan instance.
		~VulkanInstance();
	private:
		void GetSupportedExtensions(const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions);
		bool GetDebugSupport(const std::vector<const char*>& validationLayers);
		void CreateInstance(const VkApplicationInfo& appInfo, Logger* logger);

		VulkanLoader* loader;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;

		uint32_t apiVersion;
		std::vector<const char*> enabledExtensions;
		std::vector<const char*> enabledLayers;
	};
}