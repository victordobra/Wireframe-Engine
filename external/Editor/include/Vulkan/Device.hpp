#pragma once

#include "VulkanInclude.hpp"

namespace wfe::editor {
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
        
        bool8_t IsAdequate() {
            return !formats.empty() && !presentModes.empty();
        }
    };

    /// @brief Creates the Vulkan device. Internal use only.
    void CreateDevice();
    /// @brief Deletes the Vulkan device. Internal use only.
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
    const VkPhysicalDeviceProperties& GetPhysicalDeviceProperties();
    /// @brief Returns the features of the VUlkan physical device.
    const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures();
    /// @brief Returns true if validation layers are enabled, otherwise false.
    bool8_t AreValidationLayersEnabled();
    /// @brief Enables validation layers. Internal use only.
    void EnableValidationLayers();
    /// @brief Disables validation layers. Internal use only.
    void DisableValidationLayers();

    /// @brief Returns details about swap chain support.
    SwapChainSupportDetails GetSwapChainSupport();
    /// @brief Finds the indices for the graphics and present queue families.
    QueueFamilyIndices FindPhysicalQueueFamilies();

    /// @brief Finds the memory type of the Vulkan physical device.
    /// @param typeFilter The memory type filter.
    /// @param properties THe memory property flags.
    /// @return The index of the memory type.
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    /// @brief Finds the supported image formating of the Vulkan physical device.
    /// @param candidates A vector with all possible image formats.
    /// @param tiling The image tiling.
    /// @param features The format feature flags.
    /// @return The first supported format.
    VkFormat FindSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    /// @brief Creates a Vulkan buffer with the given info.
    /// @param size The buffer's size.
    /// @param usage The buffer usage flags.
    /// @param properties The buffer memory property flags.
    /// @param buffer A reference to the buffer.
    /// @param bufferMemory A reference to the buffer memory.
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    /// @brief Creates a Vulkan image with the given info.
    /// @param imageInfo The image create info.
    /// @param properties The image memory property flags.
    /// @param image A reference to the image.
    /// @param imageMemory A reference to the image memory.
    void CreateImage(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    /// @brief Begins single time commands.
    VkCommandBuffer BeginSingleTimeCommands();
    /// @brief Ends single time commands.
    /// @param commandBuffer The single time command buffer.
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    /// @brief Copies a buffer to another buffer.
    /// @param srcBuffer The buffer to copy from.
    /// @param dstBuffer The buffer to copy to.
    /// @param size The size of the buffers.
    /// @param commandBuffer The command buffer to record the command to.
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Copies an image to another image.
    /// @param srcImage The image to copy form.
    /// @param dstImage The image to copy to.
    /// @param width The width of the images.
    /// @param height The height of the images.
    /// @param srcLayerCount The layer count of the source image.
    /// @param dstLayerCount THe layer count of the destination image.
    /// @param commandBuffer The command buffer to record the command to.
    void CopyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height, uint32_t srcLayerCount, uint32_t dstLayerCount, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Copies a buffer to an image.
    /// @param buffer The buffer to copy from.
    /// @param image The image to copy to.
    /// @param width The width of the image.
    /// @param height The height of the image.
    /// @param layerCount The layer count of the image.
    /// @param commandBuffer The command buffer to record the command to.
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Copies an image to a buffer.
    /// @param image The image to copy from.
    /// @param buffer The buffer to copy to.
    /// @param width The width of the image.
    /// @param height The height of the image.
    /// @param layerCount The layer count of the image.
    /// @param commandBuffer The command buffer to record the command to.
    void CopyImageToBuffer(VkImage image, VkBuffer buffer, uint32_t width, uint32_t height, uint32_t layerCount, VkCommandBuffer commandBuffer = nullptr);
    /// @brief Transitions the image's layout.
    /// @param image The image to transition the layout of.
    /// @param srcLayout The current layout of the image.
    /// @param dstLayout The target layout of the image.
    /// @param format THe image's format.
    /// @param commandBuffer The command buffer to record the command to.
    void TransitionImageLayout(VkImage image, VkImageLayout srcLayout, VkImageLayout dstLayout, VkFormat format, VkCommandBuffer commandBuffer = nullptr);
    
    /// @brief Pads the given uniform buffer size.
    /// @param originalSize The original size of the uniform buffer.
    /// @return The padded size of the uniform buffer.
    VkDeviceSize PadUniformBufferSize(VkDeviceSize originalSize);
}