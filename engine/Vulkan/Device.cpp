#include "Device.hpp"
#include "Window/MainWindow.hpp"
#include "VulkanInclude.hpp"
#include "Core.hpp"
#include "ProjectInfo.hpp"

namespace wfe {
    // Variables
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool;

    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue graphicsQueue, presentQueue;
    VkPhysicalDeviceProperties deviceProperties;

    const vector<const char_t*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const vector<const char_t*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    // Extensions
#ifdef PLATFORM_WINDOWS
#ifdef NDEBUG
    const vector<const char_t*> requiredExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
#else
    const vector<const char_t*> requiredExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
#endif
#endif

    // Debug callbacks
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        switch(messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            console::OutErrorFunction((string)"Validation layer: " + pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            console::OutWarningFunction((string)"Validation layer: " + pCallbackData->pMessage);
            break;
        default:
            console::OutMessageFunction((string)"Validation layer: " + pCallbackData->pMessage);
            break;
        }

        return VK_FALSE;
    }
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if(func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        else
            return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
            func(instance, debugMessenger, pAllocator);
    }

    // Helper functions
    static bool8_t IsDeviceSuitable(VkPhysicalDevice physicalDevice);
    static bool8_t CheckValidationLayerSupport();
    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static void HasRequiredInstanceExtensions();
    static bool8_t CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
    static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);

    static bool8_t IsDeviceSuitable(VkPhysicalDevice physicalDevice) {
        QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

        bool8_t extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

        bool8_t swapChainAdequate = false;
        if(extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }
    static bool8_t CheckValidationLayerSupport() {
        uint32_t layerCount = 0;
        auto result = vkEnumerateInstanceLayerProperties((::uint32_t*)&layerCount, nullptr);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to enumerate instance layer properties! Error code: " + VkResultToString(result), 1);

        vector<VkLayerProperties> availableLayers(layerCount);
        result = vkEnumerateInstanceLayerProperties((::uint32_t*)&layerCount, availableLayers.data());
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to enumerate instance layer properties! Error code: " + VkResultToString(result), 1);

        for(size_t i = 0; i < validationLayers.size(); i++) {
            const char_t* layerName = validationLayers[i];
            bool8_t layerFound = false;

            for(size_t j = 0; j < availableLayers.size(); j++)
                if(strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                    layerFound = true;
                    break;
                }

            if (!layerFound)
                return false;
        }

        return true;
    }
    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, (::uint32_t*)&queueFamilyCount, nullptr);

        vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, (::uint32_t*)&queueFamilyCount, queueFamilies.data());

        for(size_t i = 0; i < queueFamilies.size(); i++) {
            const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

            if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
                indices.graphicsFamilyHasValue = true;
            }

            VkBool32 presentSupport = false;
            auto result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to get physical device surface support! Error code: " + VkResultToString(result), 1);

            if(queueFamily.queueCount > 0 && presentSupport) {
                indices.presentFamily = i;
                indices.presentFamilyHasValue = true;
            }
            if(indices.IsComplete())
                break;
        }

        return indices;
    }
    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr;
    }
    static void HasRequiredInstanceExtensions() {
        uint32_t extensionCount = 0;
        auto result = vkEnumerateInstanceExtensionProperties(nullptr, (::uint32_t*)&extensionCount, nullptr);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to enumerate instance extension properties! Error code: " + VkResultToString(result), 1);

        vector<VkExtensionProperties> extensions(extensionCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, (::uint32_t*)&extensionCount, extensions.data());
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to enumerate instance extension properties! Error code: " + VkResultToString(result), 1);

        unordered_set<string> available;
        for(const auto& extension : extensions)
            available.insert(extension.extensionName);

        for(const auto& required : requiredExtensions)
            if (!available.count(required))
                console::OutFatalError("Failed to find a required extension!", 1);
    }
    static bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
        uint32_t extensionCount;
        auto result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, (::uint32_t*)&extensionCount, nullptr);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to enumerate device extension properties! Error code: " + VkResultToString(result), 1);

        vector<VkExtensionProperties> availableExtensions(extensionCount);
        result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, (::uint32_t*)&extensionCount, availableExtensions.data());
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to enumerate device extension properties! Error code: " + VkResultToString(result), 1);

        unordered_set<string> requiredExtensions;

        for(size_t i = 0; i < deviceExtensions.size(); i++)
            requiredExtensions.insert(deviceExtensions[i]);

        for(const auto& extension : availableExtensions)
            requiredExtensions.remove(extension.extensionName);

        return requiredExtensions.count() == 0;
    }
    static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;
        auto result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to get physical device surface capabilities! Error code: " + VkResultToString(result), 1);

        uint32_t formatCount = 0;
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, (::uint32_t*)&formatCount, nullptr);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to get physical device surface capabilities! Error code: " + VkResultToString(result), 1);

        if(formatCount) {
            details.formats.resize(formatCount);
            result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, (::uint32_t*)&formatCount, details.formats.data());
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to get physical device surface formats! Error code: " + VkResultToString(result), 1);
        }

        uint32_t presentModeCount;
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, (::uint32_t*)&presentModeCount, nullptr);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to get physical device surface present modes! Error code: " + VkResultToString(result), 1);

        if(presentModeCount) {
            details.presentModes.resize(presentModeCount);
            result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, (::uint32_t*)&presentModeCount, details.presentModes.data());
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to get physical device surface present modes! Error code: " + VkResultToString(result), 1);
        }
        return details;
    }

    static void CreateInstance() {
        if(enableValidationLayers && !CheckValidationLayerSupport())
            console::OutFatalError("validation layers requested, but not available!", 1);

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = PROJECT_NAME;
        appInfo.applicationVersion = VK_MAKE_VERSION(PROJECT_VERSION_MAJOR, PROJECT_VERSION_MAJOR, PROJECT_VERSION_PATCH);
        appInfo.pEngineName = ENGINE_NAME;
        appInfo.engineVersion = VK_MAKE_VERSION(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if(enableValidationLayers) {
            createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        auto result = vkCreateInstance(&createInfo, nullptr, &instance);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create Vulkan instance! Error code: " + VkResultToString(result), 1);

        HasRequiredInstanceExtensions();
    }
    static void SetupDebugMessenger() {
        if(!enableValidationLayers) 
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);
        
        auto result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to set up debug messenger! Error code: " + VkResultToString(result), 1);
    }
    static void CreateSurface() {
#ifdef PLATFORM_WINDOWS
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = GetMainWindowHandle();
        createInfo.hinstance = GetWindowsInstance();

        auto result = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create surface! Error code: " + VkResultToString(result), 1);
#endif
    }
    static void PickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, (::uint32_t*)&deviceCount, nullptr);
        if (deviceCount == 0)
            console::OutFatalError("Failed to find GPUs with Vulkan support!", 1);

        console::OutMessageFunction((string)"Vulkan device count: " + ToString(deviceCount));

        vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, (::uint32_t*)&deviceCount, devices.data());

        for(size_t i = 0; i < deviceCount; i++)
            if(IsDeviceSuitable(devices[i])) {
                physicalDevice = devices[i];
                break;
            }

        if(physicalDevice == VK_NULL_HANDLE)
            console::OutFatalError("Failed to find a suitable GPU!", 1);
        
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        console::OutMessageFunction((string)"Physical Vulkan device: " + deviceProperties.deviceName);
    }
    static void CreateLogicalDevice() {
        QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

        vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        vector<uint32_t> uniqueQueueFamilies;
        uniqueQueueFamilies.push_back(indices.graphicsFamily);
        if(indices.presentFamily != indices.graphicsFamily)
            uniqueQueueFamilies.push_back(indices.presentFamily);

        float32_t queuePriority = 1.0f;
        for(uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if(enableValidationLayers) {
            createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else
            createInfo.enabledLayerCount = 0;

        auto result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create logical device! Error code: " + VkResultToString(result), 1);

        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
    }
    static void CreateCommandPool() {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        auto result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create command pool! Error code: " + VkResultToString(result), 1);
    }

    // External functions
    void CreateVulkanDevice() {
        CreateInstance();
        SetupDebugMessenger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateCommandPool();
    }
    void DeleteVulkanDevice() {
        vkDestroyCommandPool(device, commandPool, nullptr);
        vkDestroyDevice(device, nullptr);

        if (enableValidationLayers)
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    VkInstance GetVulkanInstance() {
        return instance;
    }
    VkPhysicalDevice GetPhysicalDevice() {
        return physicalDevice;
    }
    VkCommandPool GetCommandPool() {
        return commandPool;
    }
    VkDevice GetDevice() {
        return device;
    }
    VkSurfaceKHR GetSurface() {
        return surface;
    }
    VkQueue GetGraphicsQueue() {
        return graphicsQueue;
    }
    VkQueue GetPresentQueue() {
        return presentQueue;
    }
    const VkPhysicalDeviceProperties& GetDeviceProperties() {
        return deviceProperties;
    }

    SwapChainSupportDetails GetSwapChainSupport() {
        return QuerySwapChainSupport(physicalDevice);
    }
    QueueFamilyIndices FindPhysicalQueueFamilies() {
        return FindQueueFamilies(physicalDevice);
    }

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for(size_t i = 0; i < memProperties.memoryTypeCount; i++)
            if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }

        console::OutFatalError("Failed to find suitable memory type!", 1);
        return 0;
    }
    VkFormat FindSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for(size_t i = 0; i < candidates.size(); i++) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, candidates[i], &props);

            if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return candidates[i];
            } else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return candidates[i];
            }
        }
        console::OutFatalError("Failed to find supported format!", 1);
        return VK_FORMAT_UNDEFINED;
    }

    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create buffer! Error code: " + VkResultToString(result), 1);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to allocate buffer memory! Error code: " + VkResultToString(result), 1);

        result = vkBindBufferMemory(device, buffer, bufferMemory, 0);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to bind buffer memory! Error code: " + VkResultToString(result), 1);
    }
    void CreateImage(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
        auto result = vkCreateImage(device, &imageInfo, nullptr, &image);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create image! Error code: " + VkResultToString(result), 1);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        result = vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to allocate image memory! Error code: " + VkResultToString(result), 1 );

        result = vkBindImageMemory(device, image, imageMemory, 0);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to bind image memory! Error code: " + VkResultToString(result), 1);
    }
    VkCommandBuffer BeginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        auto result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to allocate command buffers! Error code: " + VkResultToString(result), 1);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to begin command buffer! Error code: " + VkResultToString(result), 1);

        return commandBuffer;
    }
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
        auto result = vkEndCommandBuffer(commandBuffer);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to end command buffer! Error code: " + VkResultToString(result), 1);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to submit command buffers to the queue! Error code: " + VkResultToString(result), 1);
        
        result = vkQueueWaitIdle(graphicsQueue);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to wait for the queue to idle! Error code: " + VkResultToString(result), 1);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);
    }
    void CopyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height, uint32_t srcLayerCount, uint32_t dstLayerCount) {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageCopy region{};
        region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.srcSubresource.mipLevel = 0;
        region.srcSubresource.baseArrayLayer = 0;
        region.srcSubresource.layerCount = srcLayerCount;

        region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.dstSubresource.mipLevel = 0;
        region.dstSubresource.baseArrayLayer = 0;
        region.dstSubresource.layerCount = dstLayerCount;

        region.srcOffset = { 0, 0, 0 };
        region.dstOffset = { 0, 0, 0 };
        region.extent = { width, height, 1 };

        vkCmdCopyImage(commandBuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        EndSingleTimeCommands(commandBuffer);
    }
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = layerCount;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        EndSingleTimeCommands(commandBuffer);
    }
    void CopyImageToBuffer(VkImage image, VkBuffer buffer, uint32_t width, uint32_t height, uint32_t layerCount) {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = layerCount;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyImageToBuffer(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer, 1, &region);
        EndSingleTimeCommands(commandBuffer);
    }
    size_t PadUniformBufferSize(size_t originalSize) {
        size_t minUboAlignment = deviceProperties.limits.nonCoherentAtomSize;
        size_t alignedSize = originalSize;

        if (minUboAlignment > 0)
            alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);

        return alignedSize;
    }
}
