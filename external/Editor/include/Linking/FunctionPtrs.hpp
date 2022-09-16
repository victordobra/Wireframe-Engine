#pragma once

#include "Core.hpp"
#include "Base/Window.hpp"
#include "Vulkan/Device.hpp"

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

namespace wfe::editor {
    // Base/Window.hpp
    typedef void(*PtrFn_LoadWorkspace)();
    typedef void(*PtrFn_SaveWorkspace)();
    typedef void(*PtrFn_RenderWindows)();

    typedef LoadCallback(*PtrFn_GetLoadCallback)();
    typedef void(*PtrFn_SetLoadCallback)(LoadCallback);
    typedef SaveCallback(*PtrFn_GetSaveCallback)();
    typedef void(*PtrFn_SetSaveCallback)(SaveCallback);

    typedef string(*PtrFn_GetWorkspaceDir)();
    typedef void(*PtrFn_SetWorkspaceDir)(const string&, bool8_t);
    typedef void(*PtrFn_CloseWorkspace)();

    // Window/MainWindow.hpp
    typedef size_t(*PtrFn_GetMainWindowWidth)();
    typedef size_t(*PtrFn_GetMainWindowHeight)();

    typedef const string&(*PtrFn_GetMainWindowName)();
    typedef void(*PtrFn_SetMainWindowName)(const string&);

    typedef string(*PtrFn_OpenFolderDialog)(bool8_t&, const string&);
    typedef void(*PtrFn_CopyFolder)(const string&, const string&);

    // Platform specific
#if defined(PLATFORM_WINDOWS)
    typedef HWND(*PtrFn_GetWindowHandle)();
    typedef HINSTANCE(*PtrFn_GetWindowsInstance)();
#elif defined(PLATFORM_LINUX)
    typedef Display*(*PtrFn_GetScreenConnection)();
    typedef Window(*PtrFn_GetWindowHandle)();
#endif

    // Vulkan/Device.hpp
    typedef void(*PtrFn_CreateDevice)();
    typedef void(*PtrFn_DeleteDevice)();

    typedef const VkAllocationCallbacks*(*PtrFn_GetVulkanAllocator)();
    typedef VkInstance(*PtrFn_GetVulkanInstance)();
    typedef VkPhysicalDevice(*PtrFn_GetPhysicalDevice)();
    typedef VkCommandPool(*PtrFn_GetCommadPool)();
    typedef VkDevice(*PtrFn_GetDevice)();
    typedef VkSurfaceKHR(*PtrFn_GetSurface)();
    typedef VkQueue(*PtrFn_GetGraphicsQueue)();
    typedef VkQueue(*PtrFn_GetPresentQueue)();
    typedef const VkPhysicalDeviceProperties&(*PtrFn_GetPhysicalDeviceProperties)();
    typedef const VkPhysicalDeviceFeatures&(*PtrFn_GetPhysicalDeviceFeatures)();
    typedef bool8_t(*PtrFn_AreValidationLayersEnabled)();
    typedef void(*PtrFn_EnableValidationLayers)();
    typedef void(*PtrFn_DisableValidationLayers)();

    typedef SwapChainSupportDetails(*PtrFn_GetSwapChainSupport)();
    typedef QueueFamilyIndices(*PtrFn_FindPhysicalQueueFamilies)();

    typedef uint32_t(*PtrFn_FindMemoryType)(uint32_t, VkMemoryPropertyFlags);
    typedef VkFormat(*PtrFn_FindSupportedFormat)(const vector<VkFormat>&, VkImageTiling, VkFormatFeatureFlags);

    typedef void(*PtrFn_CreateBuffer)(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer&, VkDeviceMemory&);
    typedef void(*PtrFn_CreateImage)(const VkImageCreateInfo&, VkMemoryPropertyFlags, VkImage&, VkDeviceMemory&);
    typedef VkCommandBuffer(*PtrFn_BeginSingleTimeCommands)();
    typedef void(*PtrFn_EndSingleTimeCommands)(VkCommandBuffer);
    typedef void(*PtrFn_CopyBuffer)(VkBuffer, VkBuffer, VkDeviceSize, VkCommandBuffer);
    typedef void(*PtrFn_CopyImage)(VkImage, VkImage, uint32_t, uint32_t, uint32_t, uint32_t, VkCommandBuffer);
    typedef void(*PtrFn_CopyBufferToImage)(VkBuffer, VkImage, uint32_t, uint32_t, uint32_t, VkCommandBuffer);
    typedef void(*PtrFn_CopyImageToBuffer)(VkImage, VkBuffer, uint32_t, uint32_t, uint32_t, VkCommandBuffer);
    typedef void(*PtrFn_TransitionImageLayout)(VkImage, VkImageLayout, VkImageLayout, VkFormat, VkCommandBuffer);

    typedef VkDeviceSize(*PtrFn_PadUniformBufferSize)(VkDeviceSize);

    // Vulkan/ImGuiPipeline.hpp
    typedef void(*PtrFn_CreateImGuiPipeline)();
    typedef void(*PtrFn_DeleteImGuiPipeline)();
    typedef void(*PtrFn_DrawImGui)();

    typedef VkPipeline(*PtrFn_GetImGuiPipeline)();
    typedef VkPipelineLayout(*PtrFn_GetImGuiPipelineLayout)();
    typedef VkCommandBuffer(*PtrFn_GetImGuiCommandBuffer)();

    // Vulkan/SwapChain.hpp
    typedef void(*PtrFn_CreateSwapChain)();
    typedef void(*PtrFn_DeleteSwapChain)();
    typedef void(*PtrFn_RecreateSwapChain)();

    typedef VkFramebuffer(*PtrFn_GetFrameBuffer)(size_t);
    typedef VkRenderPass(*PtrFn_GetRenderPass)();
    typedef VkImageView(*PtrFn_GetImageView)(size_t);
    typedef size_t(*PtrFn_GetImageCount)();
    typedef VkFormat(*PtrFn_GetSwapChainImageFormat)();
    typedef VkExtent2D(*PtrFn_GetSwapChainExtent)();
    typedef size_t(*PtrFn_GetSwapChainWidth)();
    typedef size_t(*PtrFn_GetSwapChainHeight)();
    typedef uint32_t(*PtrFn_GetCurrentFrame)();

    typedef float32_t(*PtrFn_ExtentAspectRatio)();
    typedef VkFormat(*PtrFn_FindDepthFormat)();
    
    typedef VkResult(*PtrFn_AcquireNextImage)(uint32_t*);
    typedef VkResult(*PtrFn_SubmitCommandBuffers)(const VkCommandBuffer*, uint32_t*, uint32_t);

    // Windows/EditorPropertiesWindow.hpp
    typedef void(*PtrFn_LoadEditorProperties)();
    typedef void(*PtrFn_SaveEditorProperties)();

    typedef string(*PtrFn_GetDefaultTemplateLocation)();
    typedef void(*PtrFn_SetDefaultTemplateLocation)(const string&);
    typedef string(*PtrFn_GetDefaultProjectLocation)();
    typedef void(*PtrFn_SetDefaultProjectLocation)(const string&);
    typedef vector<string>(*PtrFn_GetTemplateLocations)();

    // Windows/NewProjectWindow.hpp
    typedef void(*PtrFn_CreateNewProject)();

    struct EditorCallbacks {
        PtrFn_LoadWorkspace loadWorkspace;
        PtrFn_SaveWorkspace saveWorkspace;
        PtrFn_RenderWindows renderWindows;

        PtrFn_GetLoadCallback getLoadCallback;
        PtrFn_SetLoadCallback setLoadCallback;
        PtrFn_GetSaveCallback getSaveCallback;
        PtrFn_SetSaveCallback setSaveCallback;

        PtrFn_GetWorkspaceDir getWorkspaceDir;
        PtrFn_SetWorkspaceDir setWorkspaceDir;
        PtrFn_CloseWorkspace closeWorkspace;

        PtrFn_GetMainWindowWidth getMainWindowWidth;
        PtrFn_GetMainWindowHeight getMainWindowHeight;

        PtrFn_GetMainWindowName getMainWindowName;
        PtrFn_SetMainWindowName setMainWindowName;

        PtrFn_OpenFolderDialog openFolderDialog;
        PtrFn_CopyFolder copyFolder;

        PtrFn_CreateDevice createDevice;
        PtrFn_DeleteDevice deleteDevice;

        PtrFn_GetVulkanAllocator getVulkanAllocator;
        PtrFn_GetVulkanInstance getVulkanInstance;
        PtrFn_GetPhysicalDevice getPhysicalDevice;
        PtrFn_GetCommadPool getCommandPool;
        PtrFn_GetDevice getDevice;
        PtrFn_GetSurface getSurface;
        PtrFn_GetGraphicsQueue getGraphicsQueue;
        PtrFn_GetPresentQueue getPresentQueue;
        PtrFn_GetPhysicalDeviceProperties getPhysicalDeviceProperties;
        PtrFn_GetPhysicalDeviceFeatures getPhysicalDeviceFeatures;
        PtrFn_AreValidationLayersEnabled areValidationLayersEnabled;
        PtrFn_EnableValidationLayers enableValidationLayers;
        PtrFn_DisableValidationLayers disableValidationLayers;

        PtrFn_GetSwapChainSupport getSwapChainSupport;
        PtrFn_FindPhysicalQueueFamilies findPhysicalQueueFamilies;

        PtrFn_FindMemoryType findMemoryType;
        PtrFn_FindSupportedFormat findSupportedFormat;

        PtrFn_CreateBuffer createBuffer;
        PtrFn_CreateImage createImage;
        PtrFn_BeginSingleTimeCommands beginSingleTimeCommands;
        PtrFn_EndSingleTimeCommands endSingleTimeCommands;
        PtrFn_CopyBuffer copyBuffer;
        PtrFn_CopyImage copyImage;
        PtrFn_CopyBufferToImage copyBufferToImage;
        PtrFn_CopyImageToBuffer copyImageToBuffer;
        PtrFn_TransitionImageLayout transitionImageLayout;

        PtrFn_PadUniformBufferSize padUniformBufferSize;

        PtrFn_CreateImGuiPipeline createImGuiPipeline;
        PtrFn_DeleteImGuiPipeline deleteImGuiPipeline;
        PtrFn_DrawImGui drawImGui;
        
        PtrFn_GetImGuiPipeline getImGuiPipeline;
        PtrFn_GetImGuiPipelineLayout getImGuiPipelineLayout;
        PtrFn_GetImGuiCommandBuffer getImGuiCommandBuffer;

        PtrFn_CreateSwapChain createSwapChain;
        PtrFn_DeleteSwapChain deleteSwapChain;
        PtrFn_RecreateSwapChain recreateSwapChain;

        PtrFn_GetFrameBuffer getFrameBuffer;
        PtrFn_GetRenderPass getRenderPass;
        PtrFn_GetImageView getImageView;
        PtrFn_GetImageCount getImageCount;
        PtrFn_GetSwapChainImageFormat getSwapChainImageFormat;
        PtrFn_GetSwapChainExtent getSwapChainExtent;
        PtrFn_GetSwapChainWidth getSwapChainWidth;
        PtrFn_GetSwapChainHeight getSwapChainHeight;
        PtrFn_GetCurrentFrame getCurrentFrame;

        PtrFn_ExtentAspectRatio extentAspectRatio;
        PtrFn_FindDepthFormat findDepthFormat;
        
        PtrFn_AcquireNextImage acquireNextImage;
        PtrFn_SubmitCommandBuffers submitCommandBuffers;

        PtrFn_LoadEditorProperties loadEditorProperties;
        PtrFn_SaveEditorProperties saveEditorProperties;

        PtrFn_GetDefaultTemplateLocation getDefaultTemplateLocation;
        PtrFn_SetDefaultTemplateLocation setDefaultTemplateLocation;
        PtrFn_GetDefaultProjectLocation getDefaultProjectLocation;
        PtrFn_SetDefaultProjectLocation setDefaultProjectLocation;
        PtrFn_GetTemplateLocations getTemplateLocations;

        PtrFn_CreateNewProject createNewProject;

        map<string, WindowType>* windowTypeMap;
    };

    void GenerateEditorCallbacks(EditorCallbacks& callbacks, map<string, WindowType>* windowTypeMap);
}