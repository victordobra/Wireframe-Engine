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

    // Variables
    static DynamicLib vulkanLib;

    static uint32_t apiVersion;
    static vector<const char_t*> instanceExtensions;

    static VkInstance instance;

    // Internal helper functions
    static bool8_t CheckForExtensionSupport() {
        // Load the list of available extensions
        uint32_t extensionCount;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties), MEMORY_USAGE_ARRAY);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

        // Loop through every available extension, checking if it is either mandatory or optional
        uint32_t mandatoryExtensionCount = 0;

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
        }

        // Check if all mandatory extensions are supported
        return mandatoryExtensionCount == MANDATORY_EXTENTIONS.size();
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
        if(!CheckForExtensionSupport())
            return false;

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

        // Set the instance create info
        VkInstanceCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
        createInfo.ppEnabledExtensionNames = instanceExtensions.begin();

        // Create the instance
        VkResult result = vkCreateInstance(&createInfo, GetVulkanAllocCallbacks(), &instance);
        if(result == VK_ERROR_INCOMPATIBLE_DRIVER)
            return false;
        if(result != VK_SUCCESS)
            WFE_LOG_FATAL("Failed to create Vulkan instance! Error code: %s", string_VkResult(result));
        
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
        // Delete the Vulkan instance
        vkDestroyInstance(instance, GetVulkanAllocCallbacks());
    }
}
