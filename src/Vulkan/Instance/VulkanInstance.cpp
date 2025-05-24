#include "VulkanInstance.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <stdexcept>
#include <string.h>
#include <vulkan/vk_enum_string_helper.h>

#if defined(WFE_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(WFE_PLATFORM_LINUX)
#endif


namespace wfe {
	// Constants
	const uint32_t VulkanInstance::DEFAULT_REQUIRED_INSTANCE_API_VERSION = VK_API_VERSION_1_2;
	const std::vector<const char*> VulkanInstance::DEFAULT_REQUIRED_INSTANCE_EXTENSIONS {
#if defined(WFE_PLATFORM_WINDOWS)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif
#endif
		VK_KHR_SURFACE_EXTENSION_NAME
	};
	const std::vector<const char*> VulkanInstance::DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS { };
	const std::vector<const char*> VulkanInstance::DEFAULT_VALIDATION_LAYERS {
		"VK_LAYER_KHRONOS_validation"
	};

	// Debug messenger callback
	static VkBool32 VKAPI_CALL DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		// Get the logger
		Logger* logger = (Logger*)pUserData;

		// Get the corresponding message severity
		Logger::MessageLevel level;
		switch(messageSeverity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			level = Logger::MESSAGE_LEVEL_INFO;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			level = Logger::MESSAGE_LEVEL_WARNING;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			level = Logger::MESSAGE_LEVEL_ERROR;
			break;
		default:
			level = Logger::MESSAGE_LEVEL_INFO;
			break;
		}

		// Output the message
		logger->LogMessage(level, (std::string)"Vulkan validation: \"" + pCallbackData->pMessage + "\"");

		return VK_FALSE;
	}

	// Internal helper functions
	void VulkanInstance::GetSupportedExtensions(const std::vector<const char*>& requiredExtensions, const std::vector<const char*>& optionalExtensions) {
		// Get all extensions supported by the instance
		uint32_t extensionCount;
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		// Check if all required extensions are supported
		for(const char* extension : requiredExtensions) {
			// Compare all extension names
			bool supported = false;
			for(size_t i = 0; i != extensions.size() && !supported; ++i)
				supported = !strncmp(extension, extensions[i].extensionName, VK_MAX_EXTENSION_NAME_SIZE);
			
			// Throw an exception if the extension is not supported
			if(!supported)
				throw std::runtime_error((std::string)"Vulkan implementation does not support " + extension + ", which is required by the program!");
		}

		// Add all required extensions to the vector
		enabledExtensions = requiredExtensions;

		// Add all supported optional extensions
		for(const char* extension : optionalExtensions) {
			// Compare all extension names
			bool supported = false;
			for(size_t i = 0; i != extensions.size() && !supported; ++i)
				supported = !strncmp(extension, extensions[i].extensionName, VK_MAX_EXTENSION_NAME_SIZE);
			
			// Add the exception to the vector, if supported
			if(supported)
				enabledExtensions.push_back(extension);
		}
	}
	bool VulkanInstance::GetDebugSupport(const std::vector<const char*>& validationLayers) {
		// Get all extensions supported by the instance
		uint32_t extensionCount;
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		// Check if the debug extension is supported
		bool debugSupported = false;
		for(size_t i = 0; i != extensions.size() && !debugSupported; ++i)
			debugSupported = !strncmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, extensions[i].extensionName, VK_MAX_EXTENSION_NAME_SIZE);
		
		// Exit the function if the extension is not supported
		if(!debugSupported)
			return false;
		
		// Get all layers supported by the instance
		uint32_t layerCount;
		loader->vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		loader->vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		// Check if all requested layers are supported
		for(const char* layer : validationLayers) {
			// Compare all layer names
			debugSupported = false;
			for(size_t i = 0; i != layers.size() && !debugSupported; ++i)
				debugSupported = !strncmp(layer, layers[i].layerName, VK_MAX_EXTENSION_NAME_SIZE);
			
			// Exit the function if the layer is not supported
			if(!debugSupported)
				return false;
		}

		// Add the required extensions and layers
		enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		enabledLayers = validationLayers;

		return true;
	}
	void VulkanInstance::CreateInstance(const VkApplicationInfo& appInfo, Logger* logger) {
		// Set the debug messenger create info
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags = 0,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = DebugMessengerCallback,
			.pUserData = logger
		};

		// Set the instance create info
		VkInstanceCreateInfo instanceInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = logger ? &debugMessengerInfo : nullptr,
			.flags = 0,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = (uint32_t)enabledLayers.size(),
			.ppEnabledLayerNames = enabledLayers.data(),
			.enabledExtensionCount = (uint32_t)enabledExtensions.size(),
			.ppEnabledExtensionNames = enabledExtensions.data()
		};

		// Create the instance
		VkResult result = loader->vkCreateInstance(&instanceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &instance);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan instance! Error code: " + string_VkResult(result));
		
		// Load all instance functions
		loader->LoadInstanceFunctions(instance);

		// Create the debug messenger, if requested and supported
		if(logger) {
			result = loader->vkCreateDebugUtilsMessengerEXT(instance, &debugMessengerInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &debugMessenger);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan debug messenger! Error code: " + string_VkResult(result));
		} else {
			debugMessenger = VK_NULL_HANDLE;
		}
	}

	// Public functions
	VulkanInstance::VulkanInstance(VulkanLoader* loader, const VkApplicationInfo& appInfo, Logger* logger, const std::vector<const char*> requiredExtensions, const std::vector<const char*> optionalExtensions, const std::vector<const char*> validationLayers) : loader(loader) {
		// Set the API version
		apiVersion = appInfo.apiVersion;

		// Check if the Vulkan version if not 1.0, if a higher version was requested
		if(apiVersion != VK_API_VERSION_1_0) {
			// Get the instance getter function
			PFN_vkEnumerateInstanceVersion pfn_vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)loader->vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
			if(!pfn_vkEnumerateInstanceVersion)
				throw std::runtime_error("Vulkan implementation does not support version " + std::to_string(VK_API_VERSION_VARIANT(apiVersion)) + "." + std::to_string(VK_API_VERSION_MAJOR(apiVersion)) + "." + std::to_string(VK_API_VERSION_MINOR(apiVersion)) + "." + std::to_string(VK_API_VERSION_PATCH(apiVersion)) + "!");
			
			// Check if the implemented version is high enough
			uint32_t apiVersionInstance;
			VkResult result = pfn_vkEnumerateInstanceVersion(&apiVersionInstance);
			if(result != VK_SUCCESS || apiVersionInstance < apiVersion)
				throw std::runtime_error("Vulkan implementation does not support version " + std::to_string(VK_API_VERSION_VARIANT(apiVersion)) + "." + std::to_string(VK_API_VERSION_MAJOR(apiVersion)) + "." + std::to_string(VK_API_VERSION_MINOR(apiVersion)) + "." + std::to_string(VK_API_VERSION_PATCH(apiVersion)) + "!");
		}

		// Get all supported extensions
		GetSupportedExtensions(requiredExtensions, optionalExtensions);

		// Check for debug support, if requested
		if(logger) {
			if(!GetDebugSupport(validationLayers)) {
				// Output a warning and disable debugging
				logger->LogMessage(Logger::MESSAGE_LEVEL_WARNING, "Vulkan validation was requested, however it is not supported.");
				logger = nullptr;
			}
		}

		// Create the instance
		CreateInstance(appInfo, logger);
	}

	VulkanInstance::~VulkanInstance() {
		// Destroy all existing components
		if(debugMessenger)
			loader->vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, &VulkanRenderer::ALLOCATION_CALLBACKS);
		loader->vkDestroyInstance(instance, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}