#pragma once

#include "VulkanInclude.hpp"

namespace mge {
    void CreateSampler();
    void DeleteSampler();

    VkSampler GetSampler();
}