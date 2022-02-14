#pragma once
#include "VulkanDevice.h"
#include "Defines.h"

#include <string>
#include <vector>

namespace mge {
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    void InitiateSwapChain(VkExtent2D extent);
    void ClearSwapChain();

    VkFramebuffer GetFrameBuffer(int index);
    VkRenderPass GetRenderPass();
    VkImageView GetImageView(int index);
    size_t GetImageCount();
    VkFormat GetSwapChainImageFormat();
    VkExtent2D GetSwapChainExtent();
    uint32_t GetWidth();
    uint32_t GetHeight();

    float ExtentAspectRatio();
    VkFormat FindDepthFormat();

    VkResult AcquireNextImage(uint32_t* imageIndex);
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
}