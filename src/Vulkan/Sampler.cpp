#include "Sampler.hpp"
#include "Device.hpp"

namespace mge {
    VkSampler sampler;

    void CreateSampler() {
        // Create the sampler create info
        VkSamplerCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        createInfo.magFilter = VK_FILTER_LINEAR;
        createInfo.minFilter = VK_FILTER_LINEAR;

        createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        createInfo.anisotropyEnable = VK_TRUE;
        createInfo.maxAnisotropy = GetDeviceProperties().limits.maxSamplerAnisotropy;

        createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        createInfo.unnormalizedCoordinates = VK_FALSE;

        createInfo.compareEnable = VK_TRUE;
        createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        createInfo.mipLodBias = 0.f;
        createInfo.minLod = 0.f;
        createInfo.maxLod = 0.f;

        auto result = vkCreateSampler(GetDevice(), &createInfo, nullptr, &sampler);
        if(result != VK_SUCCESS)
            console::OutFatalError("Failed to create sampler!", 1);
    }
    void DeleteSampler() {
        // Wait for the device to idle and destroy the sampler afterwards
        vkDeviceWaitIdle(GetDevice());

        vkDestroySampler(GetDevice(), sampler, nullptr);
    }

    VkSampler GetSampler() {
        return sampler;
    }
}