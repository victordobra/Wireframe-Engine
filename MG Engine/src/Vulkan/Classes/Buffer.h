#pragma once
#include "VulkanInclude.h"
#include "Defines.h"

namespace mge {
    class Buffer {
    public:
        Buffer(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);

        Buffer(const Buffer&) = delete;
        Buffer(Buffer&&) = delete;

        Buffer& operator=(const Buffer&) = delete;
        Buffer& operator=(Buffer&&) = delete;

        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void Unmap();

        void WriteToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void WriteToIndex(void* data, int index);
        VkResult FlushIndex(int index);
        VkDescriptorBufferInfo DescriptorInfoForIndex(int index);
        VkResult InvalidateIndex(int index);

        VkBuffer              GetBuffer()              const { return buffer; }
        void*                 GetMappedMemory()        const { return mapped; }
        uint32_t              GetInstanceCount()       const { return instanceCount; }
        VkDeviceSize          GetInstanceSize()        const { return instanceSize; }
        VkDeviceSize          GetAlignmentSize()       const { return instanceSize; }
        VkBufferUsageFlags    GetUsageFlags()          const { return usageFlags; }
        VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return memoryPropertyFlags; }
        VkDeviceSize          GetBufferSize()          const { return bufferSize; }

        ~Buffer();
    private:
        static VkDeviceSize GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        void* mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };
}

