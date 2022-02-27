#pragma once
#include "VulkanInclude.h"
#include "Defines.h"

#include <string>
#include <vector>

namespace mge {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats{};
        std::vector<VkPresentModeKHR> presentModes{};
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily{};
        uint32_t presentFamily{};
        bool8_t graphicsFamilyHasValue = false;
        bool8_t presentFamilyHasValue = false;
        bool8_t IsComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

#ifdef NDEBUG
    const bool8_t enableValidationLayers = false;
#else
    const bool8_t enableValidationLayers = true;
#endif

    void InitiateDevice();
    void ClearDevice();

    VkCommandPool GetCommandPool();
    VkDevice GetDevice();
    VkSurfaceKHR GetSurface();
    VkQueue GetGraphicsQueue();
    VkQueue GetPresentQueue();
    VkPhysicalDeviceProperties& GetProperties();

    SwapChainSupportDetails GetSwapChainSupport();
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    QueueFamilyIndices FindPhysicalQueueFamilies();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Buffer Helper Functions
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
    size_t PadUniformBufferSize(size_t originalSize);

    void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
}