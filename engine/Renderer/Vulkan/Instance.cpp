#include "Instance.hpp"
#include "Allocator.hpp"
#include "ProjectInfo.hpp"

// Include Vulkan with the current platform define
#if defined(WFE_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(WFE_PLATFORM_LINUX)
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
#if defined(WFE_PLATFORM_WINDOWS)
	#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(WFE_PLATFORM_LINUX)
	#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif

	static const uint32_t MIN_API_VERSION = VK_API_VERSION_1_0;

	static const vector<const char_t*> MANDATORY_EXTENTIONS = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME
	};
	static const vector<const char_t*> OPTIONAL_EXTENSIONS = {
		VK_KHR_DEVICE_GROUP_EXTENSION_NAME
	};
	static const vector<const char_t*> DEBUG_EXTENSIONS = {
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	static const vector<const char_t*> DEBUG_LAYERS = {
		"VK_LAYER_KHRONOS_validation"
	};

	// Variables
	static DynamicLib vulkanLib;

	static uint32_t apiVersion;
	static vector<const char_t*> instanceExtensions;
	static vector<const char_t*> instanceLayers;

#ifdef WFE_BUILD_MODE_DEBUG
	static bool8_t debugEnabled = true;
#else
	static bool8_t debugEnabled = false;
#endif

	static VkInstance instance;
	static VkDebugUtilsMessengerEXT debugMessenger;

	// Debug message callback
	static VkBool32 DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		// Output a message with the given severity
		switch(messageSeverity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			WFE_LOG_DEBUG("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			WFE_LOG_INFO("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			WFE_LOG_WARNING("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			WFE_LOG_ERROR("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}

	// Internal helper functions
	static bool8_t CheckForExtensionSupport() {
		// Load the list of available extensions
		uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties), MEMORY_USAGE_ARRAY);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

		// Loop through every available extension, checking if it is mandatory, optional or for debugging
		uint32_t mandatoryExtensionCount = 0, debugExtensionCount = 0;

		VkExtensionProperties* extensionsEnd = extensions + extensionCount;
		for(VkExtensionProperties* extension = extensions; extension != extensionsEnd; ++extension) {
			// Check if the current extension is mandatory
			for(const char_t* mandatoryExtension : MANDATORY_EXTENTIONS)
				if(!strncmp(mandatoryExtension, extension->extensionName, VK_MAX_EXTENSION_NAME_SIZE)) {
					// Increment the mandatory extension count and insert the current extension in the instance extension list
					++mandatoryExtensionCount;
					instanceExtensions.push_back(mandatoryExtension);

					// Don't bother to check for any other matches
					break;
				}

			// Check if the current extension is optional
			for(const char_t* optionalExtension : OPTIONAL_EXTENSIONS)
				if(!strncmp(optionalExtension, extension->extensionName, VK_MAX_EXTENSION_NAME_SIZE)) {
					// Insert the current extension in the instance extension list
					instanceExtensions.push_back(optionalExtension);

					// Don't bother to check for any other matches
					break;
				}
			
			// Only proceed if debugging is enabled
			if(debugEnabled) {
				// Check if the current extension is for debugging
				for(const char_t* debugExtension : DEBUG_EXTENSIONS)
					if(!strncmp(debugExtension, extension->extensionName, VK_MAX_EXTENSION_NAME_SIZE)) {
						// Increment the debug extension count
						++debugExtensionCount;

						// Don't bother to check for any other matches
						break;
					}
			}
		}

		// Free the allocated extension list memory
		free(extensions, MEMORY_USAGE_ARRAY);

		// Disable debugging if not all extensions are enabled
		debugEnabled = debugExtensionCount == DEBUG_EXTENSIONS.size();

		// Check if all mandatory extensions are supported
		return mandatoryExtensionCount == MANDATORY_EXTENTIONS.size();
	}
	static bool8_t CheckForLayerSupport() {
		// Exit the function if debugging is not enabled
		if(!debugEnabled)
			return false;

		// Load the list of available layers
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		VkLayerProperties* layers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties), MEMORY_USAGE_ARRAY);
		vkEnumerateInstanceLayerProperties(&layerCount, layers);

		// Loop through every available layer, checking if it is wanted
		size_t debugLayerCount = 0;

		VkLayerProperties* layersEnd = layers + layerCount;
		for(VkLayerProperties* layer = layers; layer != layersEnd; ++layer) {
			// Check if the current layer is wanted
			for(const char_t* debugLayer : DEBUG_LAYERS)
				if(!strncmp(debugLayer, layer->layerName, VK_MAX_EXTENSION_NAME_SIZE)) {
					// Increment the debug layer count and insert the current layer in the layer list
					++debugLayerCount;
					instanceLayers.push_back(debugLayer);

					// Don't bother to check for any other matches
					break;
				}
		}

		// Return true if any layer was found
		return debugLayerCount;
	}

	static bool8_t CheckForVulkanSupport() {
		// Try to load the Vulkan dll
		if(!vulkanLib.LoadLib("vulkan-1.dll"))
			return false;
		
		// Check if the vkCreateInstance function is available in the DLL
		if(!vulkanLib.LoadFunc("vkCreateInstance")) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}
		
		// Check if the Vulkan API version is too low
		vkEnumerateInstanceVersion(&apiVersion);
		if(apiVersion < MIN_API_VERSION) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		// Check if all required extensions are supported
		if(!CheckForExtensionSupport()) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		// Check if any validation layers are supported
		if(CheckForLayerSupport()) {
			// Insert all debug extensions into the instance extension vector
			for(const char_t* debugExtension : DEBUG_EXTENSIONS)
				instanceExtensions.push_back(debugExtension);
		} else {
			// Disable debugging
			debugEnabled = false;
		}

		// Shrink the instance extension and layer vectors to fit
		instanceExtensions.shrink_to_fit();
		instanceLayers.shrink_to_fit();

		return true;
	}
	static bool8_t CreateInstance() {
		// Set the application info
		VkApplicationInfo appInfo;

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = WFE_PROJECT_NAME;
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, WFE_PROJECT_VERSION_MAJOR, WFE_PROJECT_VERSION_MINOR, WFE_PROJECT_VERSION_PATCH);
		appInfo.pEngineName = WFE_ENGINE_NAME;
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, WFE_ENGINE_VERSION_MAJOR, WFE_ENGINE_VERSION_MINOR, WFE_ENGINE_VERSION_PATCH);
		appInfo.apiVersion = apiVersion;

		// Set the debug messenger create info (if debugging is enabled)
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

		if(debugEnabled) {
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.pNext = nullptr;
			debugCreateInfo.flags = 0;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
			debugCreateInfo.pfnUserCallback = DebugMessageCallback;
			debugCreateInfo.pUserData = nullptr;
		}

		// Set the instance create info
		VkInstanceCreateInfo createInfo;

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		// Extend the create info with the debug create info for instance creation validation
		if(debugEnabled)
			createInfo.pNext = &debugCreateInfo;
		else
			createInfo.pNext = nullptr;

		createInfo.flags = 0;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = (uint32_t)instanceLayers.size();
		createInfo.ppEnabledLayerNames = instanceLayers.data();
		createInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		createInfo.ppEnabledExtensionNames = instanceExtensions.begin();

		// Create the instance
		VkResult result = vkCreateInstance(&createInfo, GetVulkanAllocCallbacks(), &instance);
		if(result == VK_ERROR_INCOMPATIBLE_DRIVER)
			return false;
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Vulkan instance! Error code: %s", string_VkResult(result));
		
		// Exit the function if debugging is disabled
		if(!debugEnabled)
			return true;
		
		// Get the create debug messenger callback
		PFN_vkCreateDebugUtilsMessengerEXT createMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if(!createMessenger)
			WFE_LOG_FATAL("Failed to create Vulkan debug messenger! Messenger creation callback not found.");
		
		// Create the debug messenger
		result = createMessenger(instance, &debugCreateInfo, GetVulkanAllocCallbacks(), &debugMessenger);
		if(result != VK_SUCCESS)
			WFE_LOG_FATAL("Failed to create Vulkan debug messenger! Error code: %s", string_VkResult(result));
		
		return true;
	}

	// Public functions
	bool8_t CreateVulkanInstance() {
		// Check for Vulkan support
		if(!CheckForVulkanSupport())
			return false;
		
		// Try to create the Vulkan instance
		if(!CreateInstance()) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		return true;
	}
	void DeleteVulkanInstance() {
		// Destroy the Vulkan debug messenger, if debugging is enabled
		if(debugEnabled) {
			// Get the destroy debug messenger callback
			PFN_vkDestroyDebugUtilsMessengerEXT destroyMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if(!destroyMessenger)
				WFE_LOG_FATAL("Failed to destroy Vulkan debug messenger! Messenger deletion callback not found.");
			
			// Destroy the debug messenger
			destroyMessenger(instance, debugMessenger, GetVulkanAllocCallbacks());
		}

		// Destroy the Vulkan instance
		vkDestroyInstance(instance, GetVulkanAllocCallbacks());
	}
}
