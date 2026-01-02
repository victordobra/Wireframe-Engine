#pragma once

#include "Core/Types/Defines.hpp"
#include "VulkanDevice.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <filesystem>

namespace wfe {
    class VulkanPipelineCache {
    public:
        /// @brief Creates a Vulkan pipeline cache.
        /// @param device The Vulkan device owning the pipeline cache.
        /// @param path The file path in which the cache data is stored.
        VulkanPipelineCache(VulkanDevice* device, const std::filesystem::path& path);
        VulkanPipelineCache(const VulkanPipelineCache&) = delete;
        VulkanPipelineCache(VulkanPipelineCache&&) = delete;

        VulkanPipelineCache& operator=(const VulkanPipelineCache&) = delete;
        VulkanPipelineCache& operator=(VulkanPipelineCache&&) = delete;

        /// @brief Gets the Vulkan device owning the pipeline cache.
        /// @return The Vulkan device owning the pipeline cache.
        VulkanDevice* GetDevice() const {
            return device;
        }
        /// @brief Gets the internal handle of the Vulkan pipeline cache.
        /// @return The internal handle of the Vulkan pipeline cache.
        VkPipelineCache GetPipelineCache() const {
            return pipelineCache;
        }

        /// @brief Gets the file path in which the cache data is stored.
        /// @return The file path in which the cache data is stored.
        const std::filesystem::path& GetPath() const {
            return path;
        }
        /// @brief Sets the file path in which the cache data is stored.
        /// @param newPath The new file path in which the cache data is stored.
        void SetPath(const std::filesystem::path& newPath) {
            path = newPath;
        }
        /// @brief Saves the pipeline cache's data to its path.
        void SaveData();
    
        /// @brief Destroys the Vulkan pipeline cache and saves its data.
        ~VulkanPipelineCache();
    private:
        VulkanDevice* device;
        VkPipelineCache pipelineCache;
        std::filesystem::path path;
    };
}