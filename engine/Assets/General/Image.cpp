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

        // Set the image create info
        VkImageCreateInfo createInfo;
        
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.imageType = VK_IMAGE_TYPE_2D;
        createInfo.format = format;
        createInfo.extent = { (uint32_t)width, (uint32_t)height, 1 };
        createInfo.mipLevels = 1;
        createInfo.arrayLayers = 1;
        createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        createInfo.tiling = tiling;
        createInfo.usage = usageFlags;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
        createInfo.initialLayout = imageLayout;

        // Create the image
        CreateImage(createInfo, memoryPropertyFlags, image, imageMemory);
    }
    void Image::CreateImageView() {
        // Create the image create info
        VkImageViewCreateInfo createInfo;

        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };

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

    // External functions
    Image::Image(size_t width, size_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
        CreateImageAndMemory(width, height, format, tiling, usageFlags, memoryPropertyFlags);
        CreateImageView();
    }

    void Image::TransitionImageLayout(VkImageLayout newLayout, VkCommandBuffer commandBuffer) {
        wfe::TransitionImageLayout(image, imageLayout, newLayout, format, commandBuffer);
        imageLayout = newLayout;
    }

    void Image::LoadFromFile(const string& fileLocation) {
        // Begin single time commands
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

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
        TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandBuffer);

        // Create a staging buffer to copy from
        VkDeviceSize bufferSize = (VkDeviceSize)(width * height * sizeof(Color8));

        Buffer stagingBuffer{bufferSize, 1, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};

        // Copy to the staging buffer
        stagingBuffer.Map();

        stagingBuffer.WriteToBuffer(pixels);
        stagingBuffer.Flush();

        stagingBuffer.Unmap();

        // Copy the buffer's contents to the image
        CopyBufferToImage(stagingBuffer.GetBuffer(), image, (uint32_t)width, (uint32_t)height, 1, commandBuffer);
        
        // Transition the image's layout again
        TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);

        // Delete the pixels
        delete[] pixels;

        // End single time commands
        EndSingleTimeCommands(commandBuffer);
    }
    void Image::SaveToFile(const string& fileLocation) {
        // Begin single time commands
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        // Save the image's layout and then transition the layout to copy to a staging buffer
        VkImageLayout oldLayout = imageLayout;
        TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, commandBuffer);

        // Create a staging buffer to copy to
        VkDeviceSize bufferSize = (VkDeviceSize)(width * height * sizeof(Color8));

        Buffer stagingBuffer{bufferSize, 1, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};

        // Copy the image's contents to the buffer
        CopyImageToBuffer(image, stagingBuffer.GetBuffer(), (uint32_t)width, (uint32_t)height, 1, commandBuffer);

        // Map the buffer and save its contents as an image
        stagingBuffer.Map();

        int32_t result = stbi_write_png(fileLocation.c_str(), (int32_t)width, (int32_t)height, sizeof(Color8), stagingBuffer.GetMappedMemory(), width * sizeof(Color8));

        if(!result)
            console::OutFatalError((string)"Failed to save image! Reason: " + stbi_failure_reason(), 1);
        
        stagingBuffer.Unmap();

        // Transition the image's layout to its old layout
        TransitionImageLayout(oldLayout, commandBuffer);

        // End single time commands
        EndSingleTimeCommands(commandBuffer);
    }

    Image::~Image() {
        vkDestroyImageView(GetDevice(), imageView, GetVulkanAllocator());
        vkDestroyImage(GetDevice(), image, GetVulkanAllocator());
        vkFreeMemory(GetDevice(), imageMemory, GetVulkanAllocator());
    }

    WFE_ASSET(Image)
}