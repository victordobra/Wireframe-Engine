#pragma once

#include "VulkanInclude.hpp"

namespace wfe {
    void CreateSampler();
    void DeleteSampler();

    VkSampler GetSampler();
}