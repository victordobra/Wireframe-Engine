#include "Image.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Buffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace wfe {
    // Internal helper functions
    void Image::CreateImageAndMemory(size_t width, size_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
        // Set some values
        this->width = width;
        this->height = height;
        this->format = format;
        imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        
        // Create the image create info
        VkImageCreateInfo createInfo{};
        
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VK_IMAGE_TYPE_2D;
        createInfo.format = format;
        createInfo.tiling = tiling;
        createInfo.initialLayout = imageLayout;
        createInfo.usage = usageFlags;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        createInfo.extent.width = (uint32_t)width;
        createInfo.extent.height = (uint32_t)height;
        createInfo.extent.depth = 1;

        createInfo.mipLevels = 1;
        createInfo.arrayLayers = 1;

        createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        createInfo.flags = 0;

        // Create the image
        CreateImage(createInfo, memoryPropertyFlags, image, imageMemory);
    }
    void Image::CreateImageView() {
        // Create the image create info
        VkImageViewCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        // Create the image view
        auto result = vkCreateImageView(GetDevice(), &createInfo, nullptr, &imageView);
        if(result != VK_SUCCESS)
            console::OutFatalError("Failed to create image view!", 1);
    }

    void Image::SetStageAndAccess(VkImageLayout layout, VkAccessFlags& accessMask, VkPipelineStageFlags& stage) const {
        // Check for every supported layout
        switch(layout) {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            accessMask = 0;
            stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            accessMask = VK_ACCESS_TRANSFER_READ_BIT;
            stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            accessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            accessMask = VK_ACCESS_SHADER_READ_BIT;
            stage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            break;
        default:
            console::OutFatalError("Unsupported image layout!", 1);
            break;
        }
    }

    // External functions
    Image::Image(size_t width, size_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
        CreateImageAndMemory(width, height, format, tiling, usageFlags, memoryPropertyFlags);
        CreateImageView();
    }

    void Image::TransitionImageLayout(VkImageLayout newLayout, VkCommandBuffer commandBuffer) {
        if(imageLayout == newLayout)
            return;

        // Create the barrier info
        VkImageMemoryBarrier barrier{};

        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = imageLayout;
        barrier.newLayout = newLayout;

        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        // Set the access masks and pipeline stages
        VkPipelineStageFlags srcStage, dstStage;
        SetStageAndAccess(imageLayout, barrier.srcAccessMask, srcStage);
        SetStageAndAccess(newLayout,   barrier.dstAccessMask, dstStage);

        // Write the pipeline barrier to the command buffer
        vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        // Set the new image layout
        imageLayout = newLayout;
    }
    void Image::TransitionImageLayout(VkImageLayout newLayout) {
        // Transition the image layout in a single time command buffer
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        TransitionImageLayout(newLayout, commandBuffer);

        EndSingleTimeCommands(commandBuffer);
    }

    void Image::LoadFromFile(const string& fileLocation) {
        // Load the stbi image
        int32_t width32, height32, channels;
        Color8* pixels = (Color8*)stbi_load(fileLocation.c_str(), &width32, &height32, &channels, STBI_rgb_alpha);

        if(!pixels)
            console::OutFatalError((string)"Failed to load image! Reason: " + stbi_failure_reason(), 1);
        
        width = (size_t)width32;
        height = (size_t)height32;

        // Create the vulkan image
        CreateImageAndMemory(width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        CreateImageView();
        TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Create a staging buffer to copy from
        VkDeviceSize bufferSize = (VkDeviceSize)(width * height * sizeof(Color8));

        Buffer stagingBuffer{bufferSize, 1, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};

        // Copy to the staging buffer
        stagingBuffer.Map();

        stagingBuffer.WriteToBuffer(pixels);
        stagingBuffer.Flush();

        stagingBuffer.Unmap();

        // Copy the buffer's contents to the image
        CopyBufferToImage(stagingBuffer.GetBuffer(), image, (uint32_t)width, (uint32_t)height, 1);
        
        // Transition the image's layout again
        TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // Delete the pixels
        delete[] pixels;
    }
    void Image::SaveToFile(const string& fileLocation) {
        // Save the image's layout and then transition the layout to copy to a staging buffer
        VkImageLayout oldLayout = imageLayout;
        TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        // Create a staging buffer to copy to
        VkDeviceSize bufferSize = (VkDeviceSize)(width * height * sizeof(Color8));

        Buffer stagingBuffer{bufferSize, 1, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};

        // Copy the image's contents to the buffer
        CopyImageToBuffer(image, stagingBuffer.GetBuffer(), (uint32_t)width, (uint32_t)height, 1);

        // Map the buffer and save its contents as an image
        stagingBuffer.Map();

        int32_t result = stbi_write_png(fileLocation.c_str(), (int32_t)width, (int32_t)height, sizeof(Color8), stagingBuffer.GetMappedMemory(), width * sizeof(Color8));

        if(!result)
            console::OutFatalError((string)"Failed to save image! Reason: " + stbi_failure_reason(), 1);
        
        stagingBuffer.Unmap();

        // Transition the image's layout to its old layout
        TransitionImageLayout(oldLayout);
    }

    Image::~Image() {
        vkDestroyImageView(GetDevice(), imageView, nullptr);
        vkDestroyImage(GetDevice(), image, nullptr);
        vkFreeMemory(GetDevice(), imageMemory, nullptr);
    }

    WFE_ASSET(Image)
}