#pragma once
#include "VulkanInclude.h"
#include <string>
#include <vector>

namespace SwapChain {
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    VkFramebuffer GetFramebuffer(size_t Index);
    VkRenderPass GetRenderPass();
    VkImageView GetImageView(size_t Index);
    size_t ImageCount();
    VkFormat GetSwapChainImageFormat();
    VkExtent2D GetSwapChainExtent();
    size_t Width();
    size_t Height();

    float ExtentAspectRatio();
    VkFormat FindDepthFormat();

    VkResult AcquireNextImage(size_t* ImageIndex);
    VkResult SubmitCommandBuffers(const VkCommandBuffer* Buffers, size_t* ImageIndex);
}