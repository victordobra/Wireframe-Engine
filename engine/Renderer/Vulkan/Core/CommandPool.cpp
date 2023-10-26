#include "CommandPool.hpp"
#include "Allocator.hpp"
#include "Device.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
    // Internal variables
    static VkCommandPool graphicsCommandPool;
    static VkCommandPool transferCommandPool;
    static VkCommandPool computeCommandPool;

    void CreateVulkanCommandPools() {
        // Get the queue family indices
        VulkanQueueFamilyIndices queueFamilies = GetVulkanDeviceQueueFamilyIndices();
        uint32_t uniqueCommandPoolCount = 1;

        // Create the graphics command pool
        VkCommandPoolCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = queueFamilies.graphicsQueueIndex;

        VkResult result = vkCreateCommandPool(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &graphicsCommandPool);
        if(result != VK_SUCCESS)
            WFE_LOG_FATAL("Failed to create Vulkan graphics command pool! Error code: %s", string_VkResult(result));
        
        // Set the transfer command pool handle if the transfer queue family index is not unique
        if(queueFamilies.transferQueueIndex == queueFamilies.graphicsQueueIndex)
            transferCommandPool = graphicsCommandPool;
        else {
            // The transfer queue family index is valid and unique; create the transfer command pool
            createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            createInfo.queueFamilyIndex = queueFamilies.transferQueueIndex;

            result = vkCreateCommandPool(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &transferCommandPool);
            if(result != VK_SUCCESS)
                WFE_LOG_FATAL("Failed to create Vulkan transfer command pool! Error code: %s", string_VkResult(result));
            
            // Increment the unique command pool count
            ++uniqueCommandPoolCount;
        }

        // Set the compute command pool handle if the compute queue family index is not unique
        if(queueFamilies.computeQueueIndex == queueFamilies.graphicsQueueIndex)
            computeCommandPool = graphicsCommandPool;
        else if (queueFamilies.computeQueueIndex == queueFamilies.transferQueueIndex)
            computeCommandPool = transferCommandPool;
        else {
            // The compute queue family index is valid and unique; create the compute command pool
            createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            createInfo.queueFamilyIndex = queueFamilies.computeQueueIndex;

            result = vkCreateCommandPool(GetVulkanDevice(), &createInfo, GetVulkanAllocCallbacks(), &computeCommandPool);
            if(result != VK_SUCCESS)
                WFE_LOG_FATAL("Failed to create Vulkan present command pool! Error code: %s", string_VkResult(result));
            
            // Increment the unique command pool count
            ++uniqueCommandPoolCount;
        }

        WFE_LOG_INFO("Created %u unique Vulkan command pools.", uniqueCommandPoolCount);
    }
    void DestroyVulkanCommandPools() {
        // Destroy all unique command pools
        vkDestroyCommandPool(GetVulkanDevice(), graphicsCommandPool, GetVulkanAllocCallbacks());
        if(transferCommandPool != graphicsCommandPool)
            vkDestroyCommandPool(GetVulkanDevice(), transferCommandPool, GetVulkanAllocCallbacks());
        if(computeCommandPool != graphicsCommandPool && computeCommandPool != transferCommandPool)
            vkDestroyCommandPool(GetVulkanDevice(), computeCommandPool, GetVulkanAllocCallbacks());
    }

    VkCommandPool GetVulkanGraphicsCommandPool() {
        return graphicsCommandPool;
    }
    VkCommandPool GetVulkanTransferCommandPool() {
        return transferCommandPool;
    }
    VkCommandPool GetVulkanComputeCommandPool() {
        return computeCommandPool;
    }
}