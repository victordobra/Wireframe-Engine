#pragma once

#include "VulkanInclude.hpp"

namespace wfe {
    struct QueueFamilyIndices {
        uint32_t graphicsFamily; bool8_t graphicsFamilyHasValue;
        uint32_t presentFamily;  bool8_t presentFamilyHasValue;

        bool8_t IsComplete() {
            return graphicsFamilyHasValue && presentFamilyHasValue;
        }
    };
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        vector<VkSurfaceFormatKHR> formats;
        vector<VkPresentModeKHR> presentModes;
    };

#ifdef NDEBUG
    const bool8_t enableValidationLayers = false;
#else
    const bool8_t enableValidationLayers = true;
#endif

    /// @brief Creates the Vulkan device. Internal use only
    void CreateDevice();
    /// @brief Deletes the Vulkan device. Internal use only
    void DeleteDevice();

    /// @brief Returns the Vulkan allocator.
    const VkAllocationCallbacks* GetVulkanAllocator();
    /// @brief Returns the Vulkan instance.
    VkInstance GetVulkanInstance();
    /// @brief Returns the Vulkan physical device.
    VkPhysicalDevice GetPhysicalDevice();
    /// @brief Returns the Vulkan command pool.
    VkCommandPool GetCommandPool();
    /// @brief Returns the Vulkan device.
    VkDevice GetDevice();
    /// @brief Returns the Vulkan surface.
    VkSurfaceKHR GetSurface();
    /// @brief Returns the Vulkan graphics queue.
    VkQueue GetGraphicsQueue();
    /// @brief Returns the Vulkan present queue.
    VkQueue GetPresentQueue();
    /// @brief Returns the properties of the Vulkan physical device.
    const VkPhysicalDeviceProperties& GetDeviceProperties();

    /// @brief Returns details about swap chain support.
    SwapChainSupportDetails GetSwapChainSupport();
    /// @brief Finds the indices for the graphics and present queue families.
    QueueFamilyIndices FindPhysicalQueueFamilies();

    /// @brief Finds the memory type of the Vulkan physical device.
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    /// @brief Finds the supported image formating of the Vulkan physical device.
    VkFormat FindSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    /// @brief Creates a Vulkan buffer with the given info.
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    /// @brief Creates a Vulkan image with the given info.
    void CreateImage(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    /// @brief Begins single time commands.
    VkCommandBuffer BeginSingleTimeCommands();
    /// @brief Ends single time commands.
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    /// @brief Copies a buffer to another buffer.
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Copies an image to another image.
    void CopyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height, uint32_t srcLayerCount, uint32_t dstLayerCount, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Copies a buffer to an image.
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Copies an image to a buffer.
    void CopyImageToBuffer(VkImage image, VkBuffer buffer, uint32_t width, uint32_t height, uint32_t layerCount, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Transitions the image's layout.
    void TransitionImageLayout(VkImage image, VkImageLayout srcLayout, VkImageLayout dstLayout, VkFormat format, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Pads the given uniform buffer size.
    VkDeviceSize PadUniformBufferSize(VkDeviceSize originalSize);
}