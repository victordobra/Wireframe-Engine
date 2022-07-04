#pragma once

#include "Device.hpp"

namespace wfe {
    class Buffer {
    public:
        Buffer() = delete;
        Buffer(const Buffer&) = delete;
        Buffer(Buffer&&) noexcept = delete;
        Buffer(VkDeviceSize instanceSize, size_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags);

        Buffer& operator=(const Buffer&) = delete;
        Buffer& operator=(Buffer&&) noexcept = delete;

        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void Unmap();

        void WriteToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void WriteToIndex(void* data, size_t index);
        void ReadFromBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void ReadFromIndex(void* data, size_t index);
        VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult FlushIndex(size_t index);
        VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult InvalidateIndex(size_t index);

        VkDescriptorBufferInfo GetDescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        VkBuffer              GetBuffer()              const { 
            return buffer; 
        }
        void*                 GetMappedMemory()        const { 
            return mapped; 
        }
        uint32_t              GetInstanceCount()       const { 
            return instanceCount; 
        }
        VkDeviceSize          GetInstanceSize()        const { 
            return instanceSize; 
        }
        VkDeviceSize          GetAlignmentSize()       const { 
            return instanceSize; 
        }
        VkBufferUsageFlags    GetUsageFlags()          const { 
            return usageFlags; 
        }
        VkMemoryPropertyFlags GetMemoryPropertyFlags() const { 
            return memoryPropertyFlags; 
        }
        VkDeviceSize          GetBufferSize()          const { 
            return bufferSize; 
        }

        ~Buffer();
    private:
        static VkDeviceSize GetAlignment(VkDeviceSize instanceSize);

        void* mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        size_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };
}