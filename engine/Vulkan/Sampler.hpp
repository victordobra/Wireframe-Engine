#pragma once

#include "VulkanInclude.hpp"

namespace wfe {
    /// @brief Creates the Vulkan image sampler. Internal use only.
    void CreateSampler();
    /// @brief Deleted the Vulkan image sampler. Internal use only.
    void DeleteSampler();

    /// @brief Returns the VUlkan image sampler.
    VkSampler GetSampler();
}