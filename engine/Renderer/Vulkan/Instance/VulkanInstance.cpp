#include "VulkanInstance.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"
#include "ProjectInfo.hpp"

#if defined(WFE_PLATFORM_WINDOWS)
#include <vulkan/vulkan_win32.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <vulkan/vulkan_xcb.h>
#endif
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	static const VkApplicationInfo DEFAULT_APPLICATION_INFO {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = WFE_PROJECT_NAME,
		.applicationVersion = VK_MAKE_API_VERSION(0, WFE_PROJECT_VERSION_MAJOR, WFE_PROJECT_VERSION_MINOR, WFE_PROJECT_VERSION_PATCH),
		.pEngineName = WFE_ENGINE_NAME,
		.engineVersion = VK_MAKE_API_VERSION(0, WFE_ENGINE_VERSION_MAJOR, WFE_ENGINE_VERSION_MINOR, WFE_ENGINE_VERSION_PATCH),
		.apiVersion = VK_API_VERSION_1_3
	};
	static const set<const char_t*> DEFAULT_REQUIRED_INSTANCE_EXTENSIONS {
#if defined(WFE_PLATFORM_WINDOWS)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(WFE_PLATFORM_LINUX)
		VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
		VK_KHR_SURFACE_EXTENSION_NAME
	};
	static const set<const char_t*> DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS {
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
	};
	static const set<const char_t*> DEFAULT_REQUIRED_INSTANCE_DEBUG_EXTENSIONS {
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};
	static const set<const char_t*> DEFAULT_OPTIONAL_INSTANCE_DEBUG_EXTENSIONS {
		VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
	};
	static const set<const char_t*> DEFAULT_REQUIRED_INSTANCE_LAYERS {
		"VK_LAYER_KHRONOS_validation"
	};
	static const set<const char_t*> DEFAULT_OPTIONAL_INSTANCE_LAYERS {
		
	};

	// Debug messenger callback
	static VkBool32 DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		// Get the logger from the user data pointer
		Logger* logger = (Logger*)pUserData;

		// Log the message absed on its severity
		switch(messageSeverity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			logger->LogInfoMessage("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			logger->LogWarningMessage("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			logger->LogErrorMessage("Vulkan validation: %s", pCallbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}

	// Internal helper functions
	static size_t InternalCheckForExtensionSupport(const set<const char_t*>& extensionNames, bool8_t* supported, uint32_t supportedCount, VkExtensionProperties* supportedExtensions) {
		// Set every value in the supported array to false, if it exists
		if(supported)
			memset(supported, 0, extensionNames.size() * sizeof(bool8_t));

		// Loop through every supported extension and check if all extensions in the set are in the supported array
		size_t supportedExtensionCount = 0;
		VkExtensionProperties* supportedExtensionsEnd = supportedExtensions + supportedCount;

		for(VkExtensionProperties* extension = supportedExtensions; extension != supportedExtensionsEnd; ++extension) {
			// Check if the current extension is in the given set
			const char_t* const* currentExtension = extensionNames.find(extension->extensionName);
			if(currentExtension != extensionNames.end()) {
				// Set the corresponding bool, if a supported array was given, and increment the supported extension count
				if(supported)
					supported[currentExtension - extensionNames.begin()] = true;
				++supportedExtensionCount;
			}
		}

		// Return the number of supported extensions
		return supportedExtensionCount;
	}
	static size_t InternalCheckForLayerSupport(const set<const char_t*>& layerNames, bool8_t* supported, uint32_t supportedCount, VkLayerProperties* supportedLayers) {
		// Set every value in the supported array to false, if it exists
		if(supported)
			memset(supported, 0, layerNames.size() * sizeof(bool8_t));

		// Loop through every supported layer and check if all layers in the set are in the supported array
		size_t supportedLayerCount = 0;
		VkLayerProperties* supportedLayersEnd = supportedLayers + supportedCount;

		for(VkLayerProperties* layer = supportedLayers; layer != supportedLayersEnd; ++layer) {
			// Check if the current layer is in the given set
			const char_t* const* currentLayer = layerNames.find(layer->layerName);
			if(currentLayer != layerNames.end()) {
				// Set the curresponding bool, if a supported array was given, and increment the supported layer count
				if(supported)
					supported[currentLayer - layerNames.begin()] = true;
				++supportedLayerCount;
			}
		}

		// Return the number of supported layers
		return supportedLayerCount;
	}

	bool8_t VulkanInstance::CheckInstanceSupport(bool8_t& debugEnabled, const set<const char_t*>& requiredExtensions, const set<const char_t*>& optionalExtensions, const set<const char_t*>& requiredDebugExtensions, const set<const char_t*>& optionalDebugExtensions, const set<const char_t*>& requiredLayers, const set<const char_t*>& optionalLayers) {
		// Get the number of supported extensions and layers
		uint32_t supportedExtensionCount, supportedLayerCount;
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr);
		loader->vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr);

		// Allocate all required arrays
		VkExtensionProperties* supportedExtensions = (VkExtensionProperties*)AllocMemory(supportedExtensionCount * sizeof(VkExtensionProperties) + supportedLayerCount * sizeof(VkLayerProperties) + (optionalExtensions.size() + optionalDebugExtensions.size() + optionalLayers.size()) * sizeof(bool8_t));
		if(!supportedExtensions)
			throw BadAllocException("Failed to allocate Vulkan instance support arrays!");
		
		VkLayerProperties* supportedLayers = (VkLayerProperties*)(supportedExtensions + supportedExtensionCount);
		bool8_t* supportedOptionalExtensions = (bool8_t*)(supportedLayers + supportedLayerCount);
		bool8_t* supportedOptionalDebugExtensions = supportedOptionalExtensions + optionalExtensions.size();
		bool8_t* supportedOptionalLayers = supportedOptionalDebugExtensions + optionalDebugExtensions.size();

		// Gets all supported extensions and layers
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions);
		loader->vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers);

		// Check for extension support
		if(InternalCheckForExtensionSupport(requiredExtensions, nullptr, supportedExtensionCount, supportedExtensions) != requiredExtensions.size()) {
			// Free the allocated arrays and exit the functions
			FreeMemory(supportedExtensions);
			return false;
		}
		
		InternalCheckForExtensionSupport(optionalExtensions, supportedOptionalExtensions, supportedExtensionCount, supportedExtensions);

		// Check for debug extension and layer support
		if(debugEnabled)
			debugEnabled = debugEnabled && (InternalCheckForExtensionSupport(requiredDebugExtensions, nullptr, supportedExtensionCount, supportedExtensions) == requiredDebugExtensions.size());
		if(debugEnabled)
			debugEnabled = debugEnabled && (InternalCheckForLayerSupport(requiredLayers, nullptr, supportedLayerCount, supportedLayers) == requiredLayers.size());
		if(debugEnabled) {
			InternalCheckForExtensionSupport(optionalDebugExtensions, supportedOptionalDebugExtensions, supportedExtensionCount, supportedExtensions);
			InternalCheckForLayerSupport(optionalLayers, supportedOptionalLayers, supportedLayerCount, supportedLayers);
		}
		
		// Add the supported layers and extensions to the instance's set
		extensions = requiredExtensions;
		for(size_t i = 0; i != optionalExtensions.size(); ++i)
			if(supportedOptionalExtensions[i])
				extensions.insert(*(optionalExtensions.begin() + i));
		
		if(debugEnabled) {
			for(const char_t* extension : requiredDebugExtensions)
				extensions.insert(extension);
			for(size_t i = 0; i != optionalDebugExtensions.size(); ++i)
				if(supportedOptionalDebugExtensions[i])
					extensions.insert(*(optionalDebugExtensions.begin() + i));
			
			layers = requiredLayers;
			for(size_t i = 0; i != optionalLayers.size(); ++i)
				if(supportedOptionalLayers[i])
					layers.insert(*(optionalLayers.begin() + i));
		}

		// Free the allocated arrays
		FreeMemory(supportedExtensions);

		return true;
	}
	void VulkanInstance::CreateInstance(bool8_t debugEnabled, Logger* logger, const VkApplicationInfo& appInfo) {
		// Set the validation features info
		VkValidationFeatureEnableEXT enabledValidationFeatures[] { 
			VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT, 
			VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT, 
			VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT 
		};
		VkValidationFeaturesEXT validationFeatureInfo {
			.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
			.pNext = nullptr,
			.enabledValidationFeatureCount = 3,
			.pEnabledValidationFeatures = enabledValidationFeatures,
			.disabledValidationFeatureCount = 0,
			.pDisabledValidationFeatures = nullptr
		};

		// Set the debug messenger info
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags = 0,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = DebugMessengerCallback,
			.pUserData = logger
		};

		// Add the validation features struct to the pNext chain, if it is enabled
		if(debugEnabled && extensions.count(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME))
			debugMessengerInfo.pNext = &validationFeatureInfo;

		// Set the instance create info
		VkInstanceCreateInfo instanceInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = (uint32_t)layers.size(),
			.ppEnabledLayerNames = layers.begin(),
			.enabledExtensionCount = (uint32_t)extensions.size(),
			.ppEnabledExtensionNames = extensions.begin()
		};

		// Check if the instance creation will use the debug messenger
		if(debugEnabled)
			instanceInfo.pNext = &debugMessengerInfo;

		// Create the instance
		VkResult result = loader->vkCreateInstance(&instanceInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &instance);
		if(result != VK_SUCCESS)
			throw Exception("Failed to create Vulkan instance! Error code: %s", string_VkResult(result));
		
		// Try to get the instance's version
		auto pfn_vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)loader->vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");

		if(pfn_vkEnumerateInstanceVersion)
			pfn_vkEnumerateInstanceVersion(&version);
		else
			version = VK_API_VERSION_1_0;
		
		// Exit the function if debugging is disabled
		if(!debugEnabled)
			return;
		
		// Unlink the validation features from the debug messenger
		debugMessengerInfo.pNext = nullptr;

		// Get the debug utild messenger's create function
		auto pfn_vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)loader->vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if(!pfn_vkCreateDebugUtilsMessengerEXT)
			throw Exception("Failed to get Vulkan debug utils messenger create function!");

		// Create the debug messenger
		result = pfn_vkCreateDebugUtilsMessengerEXT(instance, &debugMessengerInfo, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS, &debugMessenger);
		if(result != VK_SUCCESS)
			throw Exception("Failed to create Vulkan debug utils messenger! Error code: %s", string_VkResult(result));
	}

	// Public functions
	const VkApplicationInfo& VulkanInstance::GetDefaultApplicationInfo() {
		return DEFAULT_APPLICATION_INFO;
	}
	const set<const char_t*>& VulkanInstance::GetDefaultRequiredInstanceExtensions() {
		return DEFAULT_REQUIRED_INSTANCE_EXTENSIONS;
	}
	const set<const char_t*>& VulkanInstance::GetDefaultOptionalInstanceExtensions() {
		return DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS;
	}
	const set<const char_t*>& VulkanInstance::GetDefaultRequiredInstanceDebugExtensions() {
		return DEFAULT_REQUIRED_INSTANCE_DEBUG_EXTENSIONS;
	}
	const set<const char_t*>& VulkanInstance::GetDefaultOptionalInstanceDebugExtensions() {
		return DEFAULT_OPTIONAL_INSTANCE_DEBUG_EXTENSIONS;
	}
	const set<const char_t*>& VulkanInstance::GetDefaultRequiredInstanceLayers() {
		return DEFAULT_REQUIRED_INSTANCE_LAYERS;
	}
	const set<const char_t*>& VulkanInstance::GetDefaultOptionalInstanceLayers() {
		return DEFAULT_OPTIONAL_INSTANCE_LAYERS;
	}

	size_t VulkanInstance::CheckForExtensionSupport(const VulkanLoader* loader, const set<const char_t*>& extensionNames, bool8_t* supported) {
		// Get the number of supported extensions
		uint32_t supportedCount;
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &supportedCount, nullptr);

		// Get the supported extensions
		PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);
		VkExtensionProperties* supportedExtensions = (VkExtensionProperties*)AllocMemory(supportedCount * sizeof(VkExtensionProperties));
		PopMemoryUsageType();
		if(!supportedExtensions)
			throw BadAllocException("Failed to allocate Vulkan supported instance extensions array!");
		
		loader->vkEnumerateInstanceExtensionProperties(nullptr, &supportedCount, supportedExtensions);

		// Use the internal function to check for extension support
		size_t extensionCount = InternalCheckForExtensionSupport(extensionNames, supported, supportedCount, supportedExtensions);

		// Free the extension array
		FreeMemory(supportedExtensions);

		return extensionCount;
	}
	size_t VulkanInstance::CheckForLayerSupport(const VulkanLoader* loader, const set<const char_t*>& layerNames, bool8_t* supported) {
		// Get the number of supported layers
		uint32_t supportedCount;
		loader->vkEnumerateInstanceLayerProperties(&supportedCount, nullptr);

		// Get the supported extensions
		PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);
		VkLayerProperties* supportedLayers = (VkLayerProperties*)AllocMemory(supportedCount * sizeof(VkLayerProperties));
		PopMemoryUsageType();
		if(!supportedLayers)
			throw BadAllocException("Failed to allocate Vulkan supported instance layers array!");

		loader->vkEnumerateInstanceLayerProperties(&supportedCount, supportedLayers);

		// Use the internal function to check for layer support
		size_t layerCount = InternalCheckForLayerSupport(layerNames, supported, supportedCount, supportedLayers);

		// Free the layer array
		FreeMemory(supportedLayers);

		return layerCount;
	}

	VulkanInstance::VulkanInstance(const VulkanLoader* loader, bool8_t debugEnabled, Logger* logger) : loader(loader) {
		// Push the renderer memory usage
		PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);

		// Check if the required extensions and layers are supported
		if(!CheckInstanceSupport(debugEnabled, DEFAULT_REQUIRED_INSTANCE_EXTENSIONS, DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS, DEFAULT_REQUIRED_INSTANCE_DEBUG_EXTENSIONS, DEFAULT_OPTIONAL_INSTANCE_DEBUG_EXTENSIONS, DEFAULT_REQUIRED_INSTANCE_LAYERS, DEFAULT_OPTIONAL_INSTANCE_LAYERS))
			throw Renderer::UnsupportedAPIException("Unsupported required Vulkan instance extensions!");
		
		// Create the instance
		CreateInstance(debugEnabled, logger, DEFAULT_APPLICATION_INFO);

		// Pop the renderer memory usage type
		PopMemoryUsageType();
	}
	VulkanInstance::VulkanInstance(const VulkanLoader* loader, bool8_t debugEnabled, Logger* logger, const VkApplicationInfo& appInfo, const set<const char_t*>& requiredExtensions, const set<const char_t*>& optionalExtensions, const set<const char_t*>& requiredDebugExtensions, const set<const char_t*>& optionalDebugExtensions, const set<const char_t*>& requiredLayers, const set<const char_t*>& optionalLayers) : loader(loader) {
		// Push the renderer memory usage
		PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);

		// Check if the required extensions and layers are supported
		if(!CheckInstanceSupport(debugEnabled, requiredExtensions, optionalExtensions, requiredDebugExtensions, optionalDebugExtensions, requiredLayers, optionalLayers))
			throw Renderer::UnsupportedAPIException("Unsupported required Vulkan instance extensions!");

		// Create the instance
		CreateInstance(debugEnabled, logger, appInfo);

		// Pop the renderer memory usage type
		PopMemoryUsageType();
	}

	const VulkanLoader* VulkanInstance::GetLoader() const {
		return loader;
	}

	VkInstance VulkanInstance::GetInstance() {
		return instance;
		
	}
	VkDebugUtilsMessengerEXT VulkanInstance::GetDebugMessenger() {
		return debugMessenger;
	}

	const set<const char_t*>& VulkanInstance::GetInstanceExtensions() const {
		return extensions;
	}
	const set<const char_t*>& VulkanInstance::GetInstanceLayers() const {
		return layers;
	}
	uint32_t VulkanInstance::GetInstanceVersion() const {
		return version;
	}

	bool8_t VulkanInstance::IsDebuggingEnabled() const {
		return debugMessenger;
	}

	VulkanInstance::~VulkanInstance() {
		// Destroy the debug messenger, if it exists
		if(debugMessenger)
			loader->vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);
		
		// Destroy the instance
		loader->vkDestroyInstance(instance, &VulkanRenderer::VULKAN_ALLOC_CALLBACKS);
	}
}