#include "DescriptorPool.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

namespace wfe {
    // Internal helper functions
    void DescriptorPool::CreateDescriptorPool() {
        // Set the descriptor pool create info
        VkDescriptorPoolCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.maxSets = (uint32_t)(info.descriptorSetLayouts.size() * MAX_FRAMES_IN_FLIGHT);
        createInfo.poolSizeCount = (uint32_t)info.descriptorPoolSizes.size();
        createInfo.pPoolSizes = info.descriptorPoolSizes.data();

        // Create the descriptor pool
        auto result = vkCreateDescriptorPool(GetDevice(), &createInfo, GetVulkanAllocator(), &descriptorPool);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to create descriptor pool! Error code: " + VkResultToString(result), 1);
    }
    void DescriptorPool::CreateDescriptorSetLayouts() {
        /*
typedef struct VkDescriptorSetLayoutCreateInfo {
    VkStructureType                        sType;
    const void*                            pNext;
    VkDescriptorSetLayoutCreateFlags       flags;
    uint32_t                               bindingCount;
    const VkDescriptorSetLayoutBinding*    pBindings;
} VkDescriptorSetLayoutCreateInfo;
*/
        // Loop through every descriptor set layout info
        for(size_t i = 0; i < info.descriptorSetLayouts.size(); ++i) {
            auto& layout = info.descriptorSetLayouts[i];

            // Set the descriptor set layout info
            VkDescriptorSetLayoutCreateInfo createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            createInfo.pNext = nullptr;
            createInfo.flags = 0;
            createInfo.bindingCount = (uint32_t)layout.bindings.size();
            createInfo.pBindings = layout.bindings.data();

            // Create the descriptor set layout
            auto result = vkCreateDescriptorSetLayout(GetDevice(), &createInfo, GetVulkanAllocator(), &layout.layout);
            if(result != VK_SUCCESS)
                console::OutFatalError((string)"Failed to create descriptor set layout! Error code: " + VkResultToString(result), 1);
        }
    }
    void DescriptorPool::CreateDescriptorSets() {
        // Resize the descriptor set to fit every descriptor set for every possible frame in flight
        descriptorSets.resize(info.descriptorSetLayouts.size() * MAX_FRAMES_IN_FLIGHT);
        descriptorSets.shrink_to_fit();

        // Add every descriptor set layout once for every possible frame in flight
        vector<VkDescriptorSetLayout> layouts;

        for(size_t i = 0; i < info.descriptorSetLayouts.size(); ++i)
            for(size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; ++j)
                layouts.push_back(info.descriptorSetLayouts[i].layout);
        
        // Allocate the descriptor sets
        VkDescriptorSetAllocateInfo allocateInfo;

        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo.pNext = nullptr;
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = (uint32_t)descriptorSets.size();
        allocateInfo.pSetLayouts = layouts.data();

        auto result = vkAllocateDescriptorSets(GetDevice(), &allocateInfo, descriptorSets.data());
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to allocate descriptor sets! Error code: " + VkResultToString(result), 1);
    }

    // External functions
    DescriptorPool::DescriptorPool(const DescriptorPoolInfo& descriptorPoolInfo) {
        info = descriptorPoolInfo;

        CreateDescriptorPool();
        CreateDescriptorSetLayouts();
        CreateDescriptorSets();
    }

    DescriptorPool::~DescriptorPool() {
        vkDeviceWaitIdle(GetDevice());

        // Destroy descriptor set layouts and the descriptor pool
        for(size_t i = 0; i < info.descriptorSetLayouts.size(); ++i)
            vkDestroyDescriptorSetLayout(GetDevice(), info.descriptorSetLayouts[i].layout, GetVulkanAllocator());
        vkDestroyDescriptorPool(GetDevice(), descriptorPool, GetVulkanAllocator());
    }
}