#pragma once

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
    /// @brief Loads all 1.0 core Vulkan functions.
    /// @return True if the Vulkan functions were loaded successfully, otherwise false.
    bool8_t LoadVulkanCoreFunctions();
    /// @brief Loads all Vulkan instance functions.
    /// @param instance The instance to load the functions from.
    void LoadVulkanInstanceFunctions(VkInstance instance);
    /// @brief Loads all Vulkan device functions.
    /// @param device The device to load the functions from.
    void LoadVulkanDeviceFunctions(VkDevice device);
    /// @brief Frees the Vulkan dynamic library from the program.
    void FreeVulkanFunctions();
}