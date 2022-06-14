#pragma once

#include "Vulkan/VulkanInclude.hpp"
#include "Math/EngineMath.hpp"
#include "Base/Asset.hpp"

namespace mge {
    class Image : public Asset {
    public:
        Image() = default;
        Image(const Image&) = delete;
        Image(Image&&) noexcept = delete;
        Image(size_t width, size_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags);

        void TransitionImageLayout(VkImageLayout newLayout, VkCommandBuffer commandBuffer);
        void TransitionImageLayout(VkImageLayout newLayout);

        size_t         GetWidth()       const {
            return width;
        }
        size_t         GetHeight()      const {
            return height;
        }
        VkImage        GetImage()       const { 
            return image; 
        }
        VkDeviceMemory GetImageMemory() const {
            return imageMemory;
        }
        VkFormat       GetFormat()      const {
            return format;
        }
        VkImageLayout  GetImageLayout() const {
            return imageLayout;
        }

        ~Image();
    protected:
        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile  (const string& fileLocation) override;
    private:
        void CreateImageAndMemory(size_t width, size_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
        void CreateImageView();

        void SetStageAndAccess(VkImageLayout layout, VkAccessFlags& accessMask, VkPipelineStageFlags& stage) const;

        size_t width, height;

        VkImage image;
        VkDeviceMemory imageMemory;
        VkImageView imageView;

        VkFormat format;
        VkImageLayout imageLayout;
    };
}