#pragma once

#ifndef MGE_VULKAN_UTILS_DONT_INCLUDE_VULKAN
#include "VulkanInclude.hpp"
#endif
#include "Core.hpp"

namespace mge {
    string VkResultToString(VkResult result);
}