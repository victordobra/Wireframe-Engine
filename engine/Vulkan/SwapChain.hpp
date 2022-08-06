#pragma once

#include "VulkanInclude.hpp"

namespace wfe {
    const size_t MAX_FRAMES_IN_FLIGHT = 2;

    /// @brief Creates the Vulkan swap chain. Internal use only.
    void CreateSwapChain();
    /// @brief Deletes the Vulkan swap chain. Internal use only.
    void DeleteSwapChain();

    /// @brief Returns the framebuffer at the specified index.
    VkFramebuffer GetFrameBuffer(size_t index);
    /// @brief Returns the render pass
    VkRenderPass GetRenderPass();
    /// @brief Returns the image view at the specified index.
    VkImageView GetImageView(size_t index);
    /// @brief Returns the number of images.
    size_t GetImageCount();
    /// @brief Returns the swap chain image format.
    VkFormat GetSwapChainImageFormat();
    /// @brief Returns the swap chain extent.
    VkExtent2D GetSwapChainExtent();
    /// @brief Returns the width of the swap chain.
    size_t GetSwapChainWidth();
    /// @brief Returns the height of the swap chain.
    size_t GetSwapChainHeight();
    /// @brief Returns the current frame index % MAX_FRAMES_IN_FLIGHT.
    uint32_t GetCurrentFrame();

    /// @brief Returns the aspect ratio of the swap chain's extent.
    float32_t ExtentAspectRatio();
    /// @brief Returns the swap chain depth format.
    VkFormat FindDepthFormat();

    /// @brief Acquires the next image in the swap chain.
    /// @param imageIndex A pointer to which the current swap chain image's index is written.
    VkResult AcquireNextImage(uint32_t* imageIndex);
    /// @brief Submits the specified command buffers.
    /// @param buffer A pointer to the submitted command buffers.
    /// @param imageIndex A pointer to the current swap chain image's index.
    /// @param bufferCount The number of command buffers to be submitted.
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex, uint32_t bufferCount = 1);
}