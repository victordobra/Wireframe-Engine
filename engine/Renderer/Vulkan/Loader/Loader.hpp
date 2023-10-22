#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
    /// @brief Loads all 1.0 core Vulkan functions.
    /// @return True if the Vulkan functions were loaded successfully, otherwise false.
    bool8_t LoadVulkanFunctions();
    /// @brief Frees the Vulkan dynamic library from the program.
    void FreeVulkanFunctions();
}