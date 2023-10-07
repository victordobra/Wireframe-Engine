#pragma once

#include <Core.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_platform.h>

namespace wfe {
    /// @brief Creates the Vulkan surface.
    void CreateVulkanSurface();
    /// @brief Destroys the Vulkan surface.
    void DestroyVulkanSurface();

    /// @brief Gets the window's Vulkan surface.
    /// @return The handle of the Vulkan surface.
    VkSurfaceKHR GetVulkanSurface();
}