#pragma once

#include "Core.hpp"

namespace wfe {
    /// @brief Creates the program's Vulkan instance.
    /// @return True if Vulkan is supported, otherwise false.
    bool8_t CreateVulkanInstance();
    /// @brief Deletes the program's Vulkan instance.
    void DeleteVulkanInstance();
}