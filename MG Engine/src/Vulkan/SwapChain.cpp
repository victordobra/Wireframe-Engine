#include "SwapChain.h"
#include "SwapChainInternal.h"
#include "OSManager.h"
#include "VulkanManager.h"
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

void CreateSwapChain();
void CreateImageViews();
void CreateDepthResources();
void CreateRenderPass();
void CreateFramebuffers();
void CreateSyncObjects();

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

VkFormat SwapChainImageFormat;
VkExtent2D SwapChainExtent;

std::vector<VkFramebuffer> SwapChainFramebuffers;
VkRenderPass RenderPass;

std::vector<VkImage> DepthImages;
std::vector<VkDeviceMemory> DepthImageMemories;
std::vector<VkImageView> DepthImageViews;
std::vector<VkImage> SwapChainImages;
std::vector<VkImageView> SwapChainImageViews;

VkExtent2D WindowExtent;

VkSwapchainKHR SChain;

std::vector<VkSemaphore> ImageAvailableSemaphores;
std::vector<VkSemaphore> RenderFinishedSemaphores;
std::vector<VkFence> InFlightFences;
std::vector<VkFence> ImagesInFlight;
size_t CurrentFrame = 0;

void SwapChain::InitSwapChain() {
	WindowExtent = {(unsigned int)OSManager::GetScreenWidth(), (unsigned int)OSManager::GetScreenHeight()};
	CreateSwapChain();
	CreateImageViews();
	CreateDepthResources();
	CreateRenderPass();
	CreateFramebuffers();
	CreateSyncObjects();
}
void SwapChain::ClearSwapChain() {
    for (auto ImageView : SwapChainImageViews)
        vkDestroyImageView(Vulkan::GetDevice(), ImageView, nullptr);
    SwapChainImageViews.clear();

    if (SChain != nullptr) {
        vkDestroySwapchainKHR(Vulkan::GetDevice(), SChain, nullptr);
        SChain = nullptr;
    }

    for (int i = 0; i < DepthImages.size(); i++) {
        vkDestroyImageView(Vulkan::GetDevice(), DepthImageViews[i], nullptr);
        vkDestroyImage(Vulkan::GetDevice(), DepthImages[i], nullptr);
        vkFreeMemory(Vulkan::GetDevice(), DepthImageMemories[i], nullptr);
    }

    for (auto framebuffer : SwapChainFramebuffers)
        vkDestroyFramebuffer(Vulkan::GetDevice(), framebuffer, nullptr);

    vkDestroyRenderPass(Vulkan::GetDevice(), RenderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(Vulkan::GetDevice(), RenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(Vulkan::GetDevice(), ImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(Vulkan::GetDevice(), InFlightFences[i], nullptr);
    }
}

void CreateSwapChain() {
    Vulkan::SwapChainSupportDetails SwapChainSupport = Vulkan::GetSwapChainSupport();

    VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SwapChainSupport.Formats);
    VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SwapChainSupport.PresentModes);
    VkExtent2D Extent = ChooseSwapExtent(SwapChainSupport.Capabilities);

    unsigned int ImageCount = SwapChainSupport.Capabilities.minImageCount + 1;
    if (SwapChainSupport.Capabilities.maxImageCount > 0 && ImageCount > SwapChainSupport.Capabilities.maxImageCount)
        ImageCount = SwapChainSupport.Capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR CreateInfo = {};
    CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    CreateInfo.surface = Vulkan::GetSurface();

    CreateInfo.minImageCount = ImageCount;
    CreateInfo.imageFormat = SurfaceFormat.format;
    CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
    CreateInfo.imageExtent = Extent;
    CreateInfo.imageArrayLayers = 1;
    CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    Vulkan::QueueFamilyIndices Indices = Vulkan::FindPhysicalQueueFamilies();
    unsigned int QueueFamilyIndices[] = { Indices.GraphicsFamily, Indices.PresentFamily };

    if (Indices.GraphicsFamily != Indices.PresentFamily) {
        CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        CreateInfo.queueFamilyIndexCount = 2;
        CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
    } else {
        CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        CreateInfo.queueFamilyIndexCount = 0;
        CreateInfo.pQueueFamilyIndices = nullptr;
    }

    CreateInfo.preTransform = SwapChainSupport.Capabilities.currentTransform;
    CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    CreateInfo.presentMode = PresentMode;
    CreateInfo.clipped = VK_TRUE;

    CreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(Vulkan::GetDevice(), &CreateInfo, nullptr, &SChain) != VK_SUCCESS)
        throw std::runtime_error("failed to create swap chain!");

    vkGetSwapchainImagesKHR(Vulkan::GetDevice(), SChain, &ImageCount, nullptr);
    SwapChainImages.resize(ImageCount);
    vkGetSwapchainImagesKHR(Vulkan::GetDevice(), SChain, &ImageCount, SwapChainImages.data());

    SwapChainImageFormat = SurfaceFormat.format;
    SwapChainExtent = Extent;
}
void CreateImageViews() {
    SwapChainImageViews.resize(SwapChainImages.size());
    
    for (size_t i = 0; i < SwapChainImages.size(); i++) {
        VkImageViewCreateInfo ViewInfo{};
        ViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ViewInfo.image = SwapChainImages[i];
        ViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ViewInfo.format = SwapChainImageFormat;
        ViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ViewInfo.subresourceRange.baseMipLevel = 0;
        ViewInfo.subresourceRange.levelCount = 1;
        ViewInfo.subresourceRange.baseArrayLayer = 0;
        ViewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(Vulkan::GetDevice(), &ViewInfo, nullptr, &SwapChainImageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("failed to create texture image view!");
    }
}
void CreateDepthResources() {
    VkFormat DepthFormat = SwapChain::FindDepthFormat();

    DepthImages.resize(SwapChain::ImageCount());
    DepthImageMemories.resize(SwapChain::ImageCount());
    DepthImageViews.resize(SwapChain::ImageCount());

    for (int i = 0; i < DepthImages.size(); i++) {
        VkImageCreateInfo ImageInfo{};
        ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        ImageInfo.imageType = VK_IMAGE_TYPE_2D;
        ImageInfo.extent.width = SwapChainExtent.width;
        ImageInfo.extent.height = SwapChainExtent.height;
        ImageInfo.extent.depth = 1;
        ImageInfo.mipLevels = 1;
        ImageInfo.arrayLayers = 1;
        ImageInfo.format = DepthFormat;
        ImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        ImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        ImageInfo.flags = 0;

        Vulkan::CreateImageWithInfo(ImageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, DepthImages[i], DepthImageMemories[i]);

        VkImageViewCreateInfo ViewInfo{};
        ViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ViewInfo.image = DepthImages[i];
        ViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ViewInfo.format = DepthFormat;
        ViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        ViewInfo.subresourceRange.baseMipLevel = 0;
        ViewInfo.subresourceRange.levelCount = 1;
        ViewInfo.subresourceRange.baseArrayLayer = 0;
        ViewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(Vulkan::GetDevice(), &ViewInfo, nullptr, &DepthImageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("failed to create texture image view!");
    }
}
void CreateRenderPass() {
    VkAttachmentDescription DepthAttachment{};
    DepthAttachment.format = SwapChain::FindDepthFormat();
    DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference DepthAttachmentRef{};
    DepthAttachmentRef.attachment = 1;
    DepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription ColorAttachment = {};
    ColorAttachment.format = SwapChainImageFormat;
    ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference ColorAttachmentRef = {};
    ColorAttachmentRef.attachment = 0;
    ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription Subpass = {};
    Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    Subpass.colorAttachmentCount = 1;
    Subpass.pColorAttachments = &ColorAttachmentRef;
    Subpass.pDepthStencilAttachment = &DepthAttachmentRef;

    VkSubpassDependency Dependency = {};
    Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    Dependency.srcAccessMask = 0;
    Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    Dependency.dstSubpass = 0;
    Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> Attachments = { ColorAttachment, DepthAttachment };

    VkRenderPassCreateInfo RenderPassInfo = {};
    RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    RenderPassInfo.attachmentCount = (unsigned int)Attachments.size();
    RenderPassInfo.pAttachments = Attachments.data();
    RenderPassInfo.subpassCount = 1;
    RenderPassInfo.pSubpasses = &Subpass;
    RenderPassInfo.dependencyCount = 1;
    RenderPassInfo.pDependencies = &Dependency;

    if (vkCreateRenderPass(Vulkan::GetDevice(), &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS)
        throw std::runtime_error("Failed to create render pass!");
}
void CreateFramebuffers() {
    SwapChainFramebuffers.resize(SwapChain::ImageCount());
    for (size_t i = 0; i < SwapChain::ImageCount(); i++) {
        std::array<VkImageView, 2> Attachments = { SwapChainImageViews[i], DepthImageViews[i] };

        VkExtent2D SwapChainExtent = SwapChain::GetSwapChainExtent();
        VkFramebufferCreateInfo FramebufferInfo = {};
        FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        FramebufferInfo.renderPass = RenderPass;
        FramebufferInfo.attachmentCount = (unsigned int)Attachments.size();
        FramebufferInfo.pAttachments = Attachments.data();
        FramebufferInfo.width = SwapChainExtent.width;
        FramebufferInfo.height = SwapChainExtent.height;
        FramebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Vulkan::GetDevice(), &FramebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create framebuffer!");
    }
}
void CreateSyncObjects() {
    ImageAvailableSemaphores.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    RenderFinishedSemaphores.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    InFlightFences.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    ImagesInFlight.resize(SwapChain::ImageCount(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo SemaphoreInfo = {};
    SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo FenceInfo = {};
    FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++)
        if (vkCreateSemaphore(Vulkan::GetDevice(), &SemaphoreInfo, nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(Vulkan::GetDevice(), &SemaphoreInfo, nullptr, &RenderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(Vulkan::GetDevice(), &FenceInfo, nullptr, &InFlightFences[i]) != VK_SUCCESS)
            throw std::runtime_error("failed to create synchronization objects for a frame!");
}

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats) {
    for (const auto& AvailableFormat : AvailableFormats)
        if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return AvailableFormat;

    return AvailableFormats[0];
}
VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes) {
    for (const auto& AvailablePresentMode : AvailablePresentModes)
        if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            std::cout << "Present mode: Mailbox \n";
            return AvailablePresentMode;
        }

    std::cout << "Present mode: V-Sync \n";
    return VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities) {
    if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return Capabilities.currentExtent;

    VkExtent2D ActualExtent = WindowExtent;
    ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
    ActualExtent.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));

    return ActualExtent;
}

VkFramebuffer SwapChain::GetFramebuffer(size_t Index) { return SwapChainFramebuffers[Index]; }
VkRenderPass SwapChain::GetRenderPass() { return RenderPass; }
VkImageView SwapChain::GetImageView(size_t Index) { return SwapChainImageViews[Index]; }
size_t SwapChain::ImageCount() { return SwapChainImageViews.size(); }
VkFormat SwapChain::GetSwapChainImageFormat() { return SwapChainImageFormat; }
VkExtent2D SwapChain::GetSwapChainExtent() { return SwapChainExtent; }
size_t SwapChain::Width() { return (size_t)SwapChainExtent.width; }
size_t SwapChain::Height() { return (size_t)SwapChainExtent.height; }

float SwapChain::ExtentAspectRatio() { return (float)SwapChainExtent.width / SwapChainExtent.height; }
VkFormat SwapChain::FindDepthFormat() { return Vulkan::FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT); }

VkResult SwapChain::AcquireNextImage(size_t* ImageIndex) {
    vkWaitForFences(Vulkan::GetDevice(), 1, &InFlightFences[CurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    unsigned int ImageIndex32;
    VkResult Result = vkAcquireNextImageKHR(Vulkan::GetDevice(), SChain, std::numeric_limits<uint64_t>::max(), ImageAvailableSemaphores[CurrentFrame], VK_NULL_HANDLE, &ImageIndex32);
    *ImageIndex = (size_t)ImageIndex32;
    
    return Result;
}
VkResult SwapChain::SubmitCommandBuffers(const VkCommandBuffer* Buffers, size_t* ImageIndex) {
    if (ImagesInFlight[*ImageIndex] != VK_NULL_HANDLE)
        vkWaitForFences(Vulkan::GetDevice(), 1, &ImagesInFlight[*ImageIndex], VK_TRUE, UINT64_MAX);

    ImagesInFlight[*ImageIndex] = InFlightFences[CurrentFrame];

    VkSubmitInfo SubmitInfo = {};
    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore WaitSemaphores[] = { ImageAvailableSemaphores[CurrentFrame] };
    VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    SubmitInfo.waitSemaphoreCount = 1;
    SubmitInfo.pWaitSemaphores = WaitSemaphores;
    SubmitInfo.pWaitDstStageMask = WaitStages;

    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = Buffers;

    VkSemaphore SignalSemaphores[] = { RenderFinishedSemaphores[CurrentFrame] };
    SubmitInfo.signalSemaphoreCount = 1;
    SubmitInfo.pSignalSemaphores = SignalSemaphores;

    vkResetFences(Vulkan::GetDevice(), 1, &InFlightFences[CurrentFrame]);
    if (vkQueueSubmit(Vulkan::GetGraphicsQueue(), 1, &SubmitInfo, InFlightFences[CurrentFrame]) != VK_SUCCESS)
        throw std::runtime_error("failed to submit draw command buffer!");

    VkPresentInfoKHR PresentInfo = {};
    PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    PresentInfo.waitSemaphoreCount = 1;
    PresentInfo.pWaitSemaphores = SignalSemaphores;

    VkSwapchainKHR SwapChains[] = { SChain };
    PresentInfo.swapchainCount = 1;
    PresentInfo.pSwapchains = SwapChains;

    unsigned int ImageIndex32{};
    PresentInfo.pImageIndices = &ImageIndex32;

    VkResult Result = vkQueuePresentKHR(Vulkan::GetPresentQueue(), &PresentInfo);

    CurrentFrame = (CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    *ImageIndex = ImageIndex32;

    return Result;
}
