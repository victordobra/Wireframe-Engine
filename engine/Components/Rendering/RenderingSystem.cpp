#include "RenderingSystem.hpp"
#include "Vulkan/EngineVulkan.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

namespace wfe {
    // Function declarations
    static void FindRenderers(vector<Renderer*>& renderers, GameObject* start);
    static void RenderForCamera(Camera* camera, const vector<Renderer*>& renderers);
    static void RenderingSystemCallback(const vector<Component*>& components, System::CallbackType callbackType);

    // System
    System renderingSystem { "RenderingSystem", RenderingSystemCallback };

    // Function definitons
    static void FindRenderers(vector<Renderer*>& renderers, GameObject* start) {
        // Check every component and add it to the vector if it's a renderer
        for(auto* component : start->GetComponents()) {
            Renderer* renderer = dynamic_cast<Renderer*>(component);
            if(renderer)
                renderers.push_back(renderer);
        }

        // Run this function for every one of the game object's children
        for(auto* child : start->GetChildren())
            FindRenderers(renderers, child);
    }
    void RenderingSystemCallback(const vector<Component*>& components, System::CallbackType callbackType) {
        if(callbackType != System::CALLBACK_TYPE_RENDER)
            return;
        
        // Find every renderer
        vector<Renderer*> renderers;
        FindRenderers(renderers, GameObject::scene);

        // Run the camera callback for every camera
        for(auto* component : components) {
            Camera* camera = dynamic_cast<Camera*>(component);
            if(!camera)
                continue;
            RenderForCamera(camera, renderers);
        }
    }
    static void RenderForCamera(Camera* camera, const vector<Renderer*>& renderers) {
        VkCommandBuffer commandBuffer;

        // Acquire the next image
        uint32_t imageIndex;
        auto result = AcquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            console::OutFatalError((string)"Failed to acquire the next image! Error code: " + VkResultToString(result), 1);
        
        // Allocate the command buffer
        VkCommandBufferAllocateInfo allocInfo;

        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.commandPool = GetCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        result = vkAllocateCommandBuffers(GetDevice(), &allocInfo, &commandBuffer);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to allocate the command buffer! Error code: " + VkResultToString(result), 1);
        
        // Begin recording the command buffer
        VkCommandBufferBeginInfo beginInfo;

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = nullptr;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to begin recording the command buffer! Error code: " + VkResultToString(result), 1);
        
        // Begin the render pass
        VkRenderPassBeginInfo renderPassInfo;

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.pNext = nullptr;
        renderPassInfo.renderPass = GetRenderPass();
        renderPassInfo.framebuffer = GetFrameBuffer((int32_t)imageIndex);
        renderPassInfo.renderArea = { { 0, 0 }, GetSwapChainExtent() };

        VkClearValue clearValues[2];
        clearValues[0].color = { camera->clearColor.r, camera->clearColor.g, camera->clearColor.b, camera->clearColor.a };
        clearValues[1].depthStencil = { 1.f, 0 };

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = clearValues;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Run the bind and draw functions for every renderer
        for(auto* renderer : renderers) {
            renderer->Bind(commandBuffer, camera);
            renderer->Draw(commandBuffer, camera);
        }

        // End the render pass
        vkCmdEndRenderPass(commandBuffer);

        // Stop recording the command buffer
        result = vkEndCommandBuffer(commandBuffer);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to end recording the command buffer! Error code: " + VkResultToString(result), 1);
        
        // Submit the command buffer
        result = SubmitCommandBuffers(&commandBuffer, &imageIndex);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to submit the command buffer! Error code: " + VkResultToString(result), 1);
        
        // Free the command buffer
        vkDeviceWaitIdle(GetDevice());
        vkFreeCommandBuffers(GetDevice(), GetCommandPool(), 1, &commandBuffer);
    }

    // Public functions
    System* GetRenderingSystem() {
        return &renderingSystem;
    }
}