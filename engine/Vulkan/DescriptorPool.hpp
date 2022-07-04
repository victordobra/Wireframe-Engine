#pragma once
#include "VulkanInclude.hpp"
#include "SwapChain.hpp"
#include "Core.hpp"

namespace wfe {
    class DescriptorPool {
    public:
        struct DescriptorPoolInfo {
            struct DescriptorSetLayout {
                VkDescriptorSetLayout layout;
                vector<VkDescriptorSetLayoutBinding> bindings;
            };

            vector<VkDescriptorPoolSize> descriptorPoolSizes;
            vector<DescriptorSetLayout> descriptorSetLayouts;
            VkDescriptorPoolCreateFlags poolCreateFlags = 0;
        };

        DescriptorPool() = delete;
        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool(DescriptorPool&&) noexcept = delete;
        DescriptorPool(const DescriptorPoolInfo& descriptorPoolInfo);

        DescriptorPool& operator=(const DescriptorPool&) = delete;
        DescriptorPool& operator=(DescriptorPool&&) noexcept = delete;

        DescriptorPoolInfo GetInfo() const {
            return info;
        }
        VkDescriptorPool GetDescriptorPool() const {
            return descriptorPool;
        }
        vector<VkDescriptorSet> GetDescriptorSets() const {
            return descriptorSets;
        }

        size_t GetDescriptorSetIndex(size_t index, size_t frameIndex = GetCurrentFrame()) {
            return index * MAX_FRAMES_IN_FLIGHT + frameIndex;
        }

        ~DescriptorPool();
    private:
        void CreateDescriptorPool();
        void CreateDescriptorSetLayouts();
        void CreateDescriptorSets();

        DescriptorPoolInfo info;

        VkDescriptorPool descriptorPool;
        vector<VkDescriptorSet> descriptorSets;
    };
}