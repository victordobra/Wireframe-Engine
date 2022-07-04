#include "SwapChain.hpp"

namespace wfe {
    // Variables
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    vector<VkFramebuffer> swapChainFramebuffers;
    VkRenderPass renderPass;

    vector<VkImage> depthImages;
    vector<VkDeviceMemory> depthImageMemories;
    vector<VkImageView> depthImageViews;
    vector<VkImage> swapChainImages;
    vector<VkImageView> swapChainImageViews;

    VkExtent2D windowExtent;

    VkSwapchainKHR swapChain;

    vector<VkSemaphore> imageAvailableSemaphores;
    vector<VkSemaphore> renderFinishedSemaphores;
    vector<VkFence> inFlightFences;
    vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    // Helper functions
    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats) {
        for(size_t i = 0; i < availableFormats.size(); i++)
            if(availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormats[i];

        return availableFormats[0];
    }
    static VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR>& availablePresentModes) {
        for(size_t i = 0; i < availablePresentModes.size(); i++)
            if(availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                console::OutMessageFunction("Present mode: Mailbox");
                return availablePresentModes[i];
            }

        console::OutMessageFunction("Present mode: V-Sync");
        return VK_PRESENT_MODE_FIFO_KHR;
    }
    static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if(capabilities.currentExtent.width != UINT_MAX)
            return capabilities.currentExtent;
        else {
            VkExtent2D actualExtent = windowExtent;
            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    static void CreateSwapChain() {
        SwapChainSupportDetails swapChainSupport = GetSwapChainSupport();

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
            imageCount = swapChainSupport.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = GetSurface();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = FindPhysicalQueueFamilies();
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

        if(indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = (::uint32_t*)queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;      // Optional
            createInfo.pQueueFamilyIndices = nullptr;  // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        auto result = vkCreateSwapchainKHR(GetDevice(), &createInfo, nullptr, &swapChain);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create swap chain! Error code: " + VkResultToString(result), 1);

        result = vkGetSwapchainImagesKHR(GetDevice(), swapChain, (::uint32_t*)&imageCount, nullptr);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to get the swap chain images! Error code: " + VkResultToString(result), 1);

        swapChainImages.resize(imageCount);

        result = vkGetSwapchainImagesKHR(GetDevice(), swapChain, (::uint32_t*)&imageCount, swapChainImages.data());
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to get the swap chain images! Error code: " + VkResultToString(result), 1);

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }
    static void CreateImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        for(size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = swapChainImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = swapChainImageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            auto result = vkCreateImageView(GetDevice(), &viewInfo, nullptr, &swapChainImageViews[i]);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create image view! Error code: " + VkResultToString(result), 1);
        }
    }
    static void CreateRenderPass() {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = FindDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = GetSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependencies[2]{};

        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].srcAccessMask = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencies[0].dstSubpass = 0;
        dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = dependencies;

        auto result = vkCreateRenderPass(GetDevice(), &renderPassInfo, nullptr, &renderPass);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create render pass! Error code: " + VkResultToString(result), 1);
    }
    static void CreateDepthResources() {
        VkFormat depthFormat = FindDepthFormat();
        VkExtent2D swapChainExtent = GetSwapChainExtent();

        depthImages.resize(GetImageCount());
        depthImageMemories.resize(GetImageCount());
        depthImageViews.resize(GetImageCount());

        for(size_t i = 0; i < depthImages.size(); i++) {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExtent.width;
            imageInfo.extent.height = swapChainExtent.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = depthFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;

            CreateImage(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImages[i], depthImageMemories[i]);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = depthImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            auto result = vkCreateImageView(GetDevice(), &viewInfo, nullptr, &depthImageViews[i]);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create texture image view! Error code: " + VkResultToString(result), 1);
        }
    }
    static void CreateFramebuffers() {
        swapChainFramebuffers.resize(GetImageCount());
        for(size_t i = 0; i < GetImageCount(); i++) {
            std::array<VkImageView, 2> attachments = { swapChainImageViews[i], depthImageViews[i] };

            VkExtent2D swapChainExtent = GetSwapChainExtent();
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = (uint32_t)attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            auto result = vkCreateFramebuffer(GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create framebuffer! Error code: " + VkResultToString(result), 1);
        }
    }
    static void CreateSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(GetImageCount());

        for(size_t i = 0; i < imagesInFlight.size(); i++)
            imagesInFlight[i] = VK_NULL_HANDLE;

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            auto result1 = vkCreateSemaphore(GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
            auto result2 = vkCreateSemaphore(GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
            auto result3 = vkCreateFence(GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]);

            if (result1 != VK_SUCCESS || result2 != VK_SUCCESS || result3 != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create synchronization objects for a frame! Error codes: " + VkResultToString(result1) + ", " + VkResultToString(result2) + ", " + VkResultToString(result3), 1);
        }
    }

    // External functions
    void CreateSwapChain(VkExtent2D extent) {
        windowExtent = extent;
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFramebuffers();
        CreateSyncObjects();
    }
    void DeleteSwapChain() {
        for(size_t i = 0; i < swapChainImageViews.size(); i++)
            vkDestroyImageView(GetDevice(), swapChainImageViews[i], nullptr);

        vkDestroySwapchainKHR(GetDevice(), swapChain, nullptr);

        for(size_t i = 0; i < depthImages.size(); i++) {
            vkDestroyImageView(GetDevice(), depthImageViews[i], nullptr);
            vkDestroyImage(GetDevice(), depthImages[i], nullptr);
            vkFreeMemory(GetDevice(), depthImageMemories[i], nullptr);
        }

        for(size_t i = 0; i < swapChainFramebuffers.size(); i++)
            vkDestroyFramebuffer(GetDevice(), swapChainFramebuffers[i], nullptr);

        vkDestroyRenderPass(GetDevice(), renderPass, nullptr);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(GetDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(GetDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(GetDevice(), inFlightFences[i], nullptr);
        }
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
        return (float32_t)swapChainExtent.width / (float32_t)swapChainExtent.height;
    }
    VkFormat FindDepthFormat() {
        return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkResult AcquireNextImage(uint32_t* imageIndex) {
        auto result = vkWaitForFences(GetDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, ULLONG_MAX);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to wait for fences! Error code: " + VkResultToString(result), 1);

        result = vkAcquireNextImageKHR(GetDevice(), swapChain, ULLONG_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, (::uint32_t*)imageIndex);

        return result;
    }
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex, uint32_t bufferCount) {
        if(imagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
            auto result = vkWaitForFences(GetDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to wait for fences! Error code: " + VkResultToString(result), 1);
        }

        imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = bufferCount;
        submitInfo.pCommandBuffers = buffers;

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        auto result = vkResetFences(GetDevice(), 1, &inFlightFences[currentFrame]);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to reset fences! Error code: " + VkResultToString(result), 1);

        result = vkQueueSubmit(GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to submit draw command buffer! Error code: " + VkResultToString(result), 1);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = (const ::uint32_t*)imageIndex;

        result = vkQueuePresentKHR(GetPresentQueue(), &presentInfo);

        ++currentFrame;
        if(currentFrame == MAX_FRAMES_IN_FLIGHT)
            currentFrame = 0;

        return result;
    }
}