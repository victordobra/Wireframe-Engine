#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Device.hpp"
#include "Window/MainWindow.hpp"

#include <limits.h>

namespace wfe {
    // Variables
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    vector<VkImage> swapChainImages;
    vector<VkImageView> swapChainImageViews;
    vector<VkImage> depthImages;
    vector<VkDeviceMemory> depthImageMemories;
    vector<VkImageView> depthImageViews;

    VkRenderPass renderPass;

    vector<VkFramebuffer> swapChainFramebuffers;

    VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
    VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];
    vector<VkFence> imagesInFlight;

    uint32_t currentFrame;

    // Internal helper functions
    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats) {
        // Select a format with the wanted settings
        for(const auto& availableFormat : availableFormats)
            if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        
        // Select the first format
        return availableFormats[0];
    }
    static VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes) {
        // Select mailbox if available
        for(const auto& availablePresentMode : availablePresentModes)
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;
        
        // Select VSync
        return VK_PRESENT_MODE_FIFO_KHR;
    }
    static VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        // Return the surfase's extent
        if(capabilities.currentExtent.width != UINT32_MAX && capabilities.currentExtent.height != UINT32_MAX)
            return capabilities.currentExtent;
        else {
            // Get the main window's extent
            uint32_t width = (uint32_t)GetMainWindowWidth(), height = (uint32_t)GetMainWindowHeight();
            
            // Clamp the window's extent to the min/max extents
            width = (width < capabilities.minImageExtent.width) ? capabilities.minImageExtent.width : (width > capabilities.maxImageExtent.width) ? capabilities.maxImageExtent.width : width;
            height = (height < capabilities.minImageExtent.height) ? capabilities.minImageExtent.height : (height > capabilities.maxImageExtent.height) ? capabilities.maxImageExtent.height : height;

            return { width, height };
        }
    }
    static bool8_t HasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    static void CreateSwapChainInternal(VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE) {
        QueueFamilyIndices indices = FindPhysicalQueueFamilies();
        SwapChainSupportDetails swapChainSupport = GetSwapChainSupport();

        // Get the surface format, present mode and extent
        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        swapChainExtent = ChooseSwapChainExtent(swapChainSupport.capabilities);

        // Set the image count
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
            imageCount = swapChainSupport.capabilities.maxImageCount;

        // Set the swapchain create info
        VkSwapchainCreateInfoKHR createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.surface = GetSurface();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = swapChainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

        if(indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 1;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = oldSwapchain;

        // Create the swap chain
        auto result = vkCreateSwapchainKHR(GetDevice(), &createInfo, GetVulkanAllocator(), &swapChain);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create swap chain! Error code: " + VkResultToString(result), 1);
        
        // Set the swap chain image format
        swapChainImageFormat = surfaceFormat.format;
        
        // Acquire the swap chain images
        uint32_t swapChainImageCount;
        vkGetSwapchainImagesKHR(GetDevice(), swapChain, &swapChainImageCount, nullptr);
        swapChainImages.resize(swapChainImageCount);
        vkGetSwapchainImagesKHR(GetDevice(), swapChain, &swapChainImageCount, swapChainImages.data());
    }
    static void CreateImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        // Set the image view create info without the image; the image will be set in the for loop
        VkImageViewCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        // Create every image view
        for(size_t i = 0; i < swapChainImageViews.size(); ++i) {
            createInfo.image = swapChainImages[i];

            auto result = vkCreateImageView(GetDevice(), &createInfo, GetVulkanAllocator(), swapChainImageViews.data() + i);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create swap chain image view! Error code: " + VkResultToString(result), 1);
        }
    }
    static void CreateDepthResources() {
        VkFormat depthFormat = FindDepthFormat();

        depthImages.resize(swapChainImages.size());
        depthImageMemories.resize(swapChainImages.size());
        depthImageViews.resize(swapChainImages.size());

        // Set the image create info
        VkImageCreateInfo imageInfo;

        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.pNext = nullptr;
        imageInfo.flags = 0;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = depthFormat;
        imageInfo.extent = { swapChainExtent.width, swapChainExtent.height, 1 };
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = nullptr;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        // Set the image view create info without the image; it will be set in the for loop
        VkImageViewCreateInfo imageViewInfo;

        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.pNext = nullptr;
        imageViewInfo.flags = 0; 
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format = depthFormat;
        imageViewInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
        
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if(HasStencilComponent(depthFormat))
            imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        // Set the image memory barrier info without the image; it will be set in the for loop
        VkImageMemoryBarrier barrier;

        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext = nullptr;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        barrier.srcQueueFamilyIndex = 0;
        barrier.dstQueueFamilyIndex = 0;
        barrier.subresourceRange = imageViewInfo.subresourceRange;

        // Begin single time commands
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        // Create every image
        for(size_t i = 0; i < swapChainImages.size(); ++i) {
            // Create the image
            CreateImage(imageInfo, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, depthImages[i], depthImageMemories[i]);

            // Create the image view
            imageViewInfo.image = depthImages[i];
            auto result = vkCreateImageView(GetDevice(), &imageViewInfo, GetVulkanAllocator(), depthImageViews.data() + i);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create depth image view! Error code: " + VkResultToString(result), 1);
            
            // Transition the image layout
            barrier.image = depthImages[i];
            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        }

        // End single time commands
        EndSingleTimeCommands(commandBuffer);
    }
    static void CreateRenderPass() {
        // Set the color and depth attachment descriptions
        VkAttachmentDescription attachments[2];

        attachments[0].flags = 0;
        attachments[0].format = swapChainImageFormat;
        attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        attachments[1].flags = 0;
        attachments[1].format = FindDepthFormat();
        attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // Set the color attachment reference
        VkAttachmentReference colorAttachmentRef;

        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        // Set the depth attachment reference
        VkAttachmentReference depthAttachmentRef;
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // Set the subpass description
        VkSubpassDescription subpass;
        
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = nullptr;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pResolveAttachments = nullptr;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments = nullptr;

        // Set the render pass create info
        VkRenderPassCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.attachmentCount = 2;
        createInfo.pAttachments = attachments;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;
        createInfo.dependencyCount = 0;
        createInfo.pDependencies = nullptr;

        auto result = vkCreateRenderPass(GetDevice(), &createInfo, GetVulkanAllocator(), &renderPass);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create render pass! Error code: " + VkResultToString(result), 1);
    }
    static void CreateFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        // Set the framebuffer create info without the attachment; it will be set for every framebuffer
        VkFramebufferCreateInfo framebufferInfo;

        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.flags = 0;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 2;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        // Create every framebuffer
        for(size_t i = 0; i < swapChainFramebuffers.size(); ++i) {
            VkImageView attachments[] = { swapChainImageViews[i], depthImageViews[i] };

            framebufferInfo.pAttachments = attachments;

            auto result = vkCreateFramebuffer(GetDevice(), &framebufferInfo, GetVulkanAllocator(), swapChainFramebuffers.data() + i);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create framebuffer! Error code: " + VkResultToString(result), 1);
        }
    }
    static void CreateSyncObjects() {
        imagesInFlight.resize(swapChainImages.size());

        // Set the semaphore create info
        VkSemaphoreCreateInfo semaphoreInfo;
        
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        semaphoreInfo.flags = 0;

        // Set the fence create info
        VkFenceCreateInfo fenceInfo;

        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.pNext = nullptr;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        // Create every image available semaphore
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto result = vkCreateSemaphore(GetDevice(), &semaphoreInfo, GetVulkanAllocator(), imageAvailableSemaphores + i);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create image available semaphore! Error code: " + VkResultToString(result), 1);
        }

        // Create every render finished semaphore
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto result = vkCreateSemaphore(GetDevice(), &semaphoreInfo, GetVulkanAllocator(), renderFinishedSemaphores + i);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create render finished semaphore! Error code: " + VkResultToString(result), 1);
        }

        // Create every in flight fence
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto result = vkCreateFence(GetDevice(), &fenceInfo, GetVulkanAllocator(), inFlightFences + i);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create in flight fence! Error code: " + VkResultToString(result), 1);
        }

        for(auto& imageInFlight : imagesInFlight)
            imageInFlight = VK_NULL_HANDLE;
    }

    // External functions
    void CreateSwapChain() {
        CreateSwapChainInternal();
        CreateImageViews();
        CreateDepthResources();
        CreateRenderPass();
        CreateFramebuffers();
        CreateSyncObjects();
        console::OutMessageFunction("Created swap chain successfully.");
    }
    void DeleteSwapChain() {
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            vkDestroySemaphore(GetDevice(), imageAvailableSemaphores[i], GetVulkanAllocator());
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            vkDestroySemaphore(GetDevice(), renderFinishedSemaphores[i], GetVulkanAllocator());
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            vkDestroyFence(GetDevice(), inFlightFences[i], GetVulkanAllocator());

        for(auto framebuffer : swapChainFramebuffers)
            vkDestroyFramebuffer(GetDevice(), framebuffer, GetVulkanAllocator());
        
        vkDestroyRenderPass(GetDevice(), renderPass, GetVulkanAllocator());

        for(auto depthImageView : depthImageViews)
            vkDestroyImageView(GetDevice(), depthImageView, GetVulkanAllocator());
        for(auto depthImage : depthImages)
            vkDestroyImage(GetDevice(), depthImage, GetVulkanAllocator());
        for(auto depthImageMemory : depthImageMemories)
            vkFreeMemory(GetDevice(), depthImageMemory, GetVulkanAllocator());
        
        for(auto imageView : swapChainImageViews)
            vkDestroyImageView(GetDevice(), imageView, GetVulkanAllocator());
        
        vkDestroySwapchainKHR(GetDevice(), swapChain, GetVulkanAllocator());
        console::OutMessageFunction("Deleted swap chain successfully.");
    }

    VkFramebuffer GetFrameBuffer(size_t index) {
        return swapChainFramebuffers[index];
    }
    VkRenderPass GetRenderPass() {
        return renderPass;
    }
    VkImageView GetImageView(size_t index) {
        return swapChainImageViews[index];
    }
    size_t GetImageCount() {
        return swapChainImages.size();
    }
    VkFormat GetSwapChainImageFormat() {
        return swapChainImageFormat;
    }
    VkExtent2D GetSwapChainExtent() {
        return swapChainExtent;
    }
    size_t GetSwapChainWidth() {
        return swapChainExtent.width;
    }
    size_t GetSwapChainHeight() {
        return swapChainExtent.height;
    }
    uint32_t GetCurrentFrame() {
        return currentFrame;
    }

    float32_t ExtentAspectRatio() {
        return (float32_t)swapChainExtent.width / swapChainExtent.height;
    }
    VkFormat FindDepthFormat() {
        return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkResult AcquireNextImage(uint32_t* imageIndex) {
        // Wait for the current in flight fence
        auto result = vkWaitForFences(GetDevice(), 1, inFlightFences + currentFrame, VK_TRUE, UINT64_MAX);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to wait for fence! Error code: " + VkResultToString(result), 1);

        return vkAcquireNextImageKHR(GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, imageIndex);
    }
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex, uint32_t bufferCount) {
        // Wait for the current image in flight
        if(imagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
            auto result = vkWaitForFences(GetDevice(), 1, imagesInFlight.data() + *imageIndex, VK_TRUE, UINT64_MAX);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to wait for fence! Error code: " + VkResultToString(result), 1);
        }

        imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        // Set the submit info
        VkSubmitInfo submitInfo;

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = imageAvailableSemaphores + currentFrame;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = bufferCount;
        submitInfo.pCommandBuffers = buffers;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = renderFinishedSemaphores + currentFrame;

        // Reset the in flight fence
        auto result = vkResetFences(GetDevice(), 1, inFlightFences + currentFrame);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to reset fence! Error code: " + VkResultToString(result), 1);
        
        // Submit to the queue
        result = vkQueueSubmit(GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to submit draw command buffer! Error code: " + VkResultToString(result), 1);

        // Set the present info
        VkPresentInfoKHR presentInfo;

        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = renderFinishedSemaphores + currentFrame;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapChain;
        presentInfo.pImageIndices = imageIndex;
        presentInfo.pResults = nullptr;

        // Present the image
        result = vkQueuePresentKHR(GetPresentQueue(), &presentInfo);

        // Increment the frame counter
        if(++currentFrame == MAX_FRAMES_IN_FLIGHT)
            currentFrame = 0;

        return result;
    }
}