#include "Buffer.hpp"

namespace wfe {
    Buffer::Buffer(VkDeviceSize instanceSize, size_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
        // Set some values
        this->instanceSize = instanceSize;
        this->instanceCount = instanceCount;
        this->bufferSize = instanceSize * instanceCount;
        this->alignmentSize = PadUniformBufferSize(bufferSize);
        this->usageFlags = usageFlags;
        this->memoryPropertyFlags = memoryPropertyFlags;

        // Create the buffer
        CreateBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
    }

    VkResult Buffer::Map(VkDeviceSize size, VkDeviceSize offset, bool8_t handleErrors) {
        if(mapped)
            return;

        // Map the memory region
        auto result = vkMapMemory(GetDevice(), memory, offset, size, 0, &mapped);
        if(handleErrors && result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to map buffer! Error code: " + VkResultToString(result), 1);
        
        return result;
    }
    void Buffer::Unmap() {
        if(!mapped)
            return;

        // Unmap the memory region
        vkUnmapMemory(GetDevice(), memory);
        mapped = nullptr;
    }

    void Buffer::WriteToBuffer(const void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy to unmapped buffer!");

        if (size == VK_WHOLE_SIZE) {
            // Copy to the mapped memory
            memcpy(mapped, data, bufferSize);
        } else {
            // Add the offset to the memory
            char_t* mappedChar = (char_t*)mapped;
            mappedChar += offset;

            // Copy to the mapped memory
            memcpy(mappedChar, data, size);
        }
    }
    void Buffer::WriteToIndex(const void* data, size_t index) {
        WriteToBuffer(data, alignmentSize, index * alignmentSize);
    }
    void Buffer::ReadFromBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy from unmapped buffer!");

        if (size == VK_WHOLE_SIZE) {
            // Copy from the mapped memory
            memcpy(data, mapped, bufferSize);
        } else {
            // Add the offset to the memory
            char_t* mappedChar = (char_t*)mapped;
            mappedChar += offset;

            // Copy from the mapped memory
            memcpy(data, mappedChar, size);
        }
    }
    void Buffer::ReadFromIndex(void* data, size_t index) {
        ReadFromBuffer(data, alignmentSize, index * alignmentSize);
    }
    VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset, bool8_t handleErrors) {
        // Set the mapped memory range
        VkMappedMemoryRange memoryRange;

        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = nullptr;
        memoryRange.memory = memory;
        memoryRange.offset = offset;
        memoryRange.size = size;

        // Flush the memory range
        auto result = vkFlushMappedMemoryRanges(GetDevice(), 1, &memoryRange);
        if(handleErrors && result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to flush buffer! Error code: " + VkResultToString(result), 1);

        return result;
    }
    VkResult Buffer::FlushIndex(size_t index, bool8_t handleErrors) {
        return Flush(alignmentSize, index * alignmentSize, handleErrors);
    }
    VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset, bool8_t handleErrors) {
        // Set the mapped memory range
        VkMappedMemoryRange memoryRange;

        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = nullptr;
        memoryRange.memory = memory;
        memoryRange.offset = offset;
        memoryRange.size = size;

        // Invalidate the memory range
        auto result = vkInvalidateMappedMemoryRanges(GetDevice(), 1, &memoryRange);
        if(handleErrors && result != VK_SUCCESS)
            console::OutFatalError((string)"Failed to invalidate buffer! Error code: " + VkResultToString(result), 1);
        
        return result;
    }
    VkResult Buffer::InvalidateIndex(size_t index, bool8_t handleErrors) {
        return Invalidate(alignmentSize, index * alignmentSize);
    }

    VkDescriptorBufferInfo Buffer::GetDescriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        VkDescriptorBufferInfo info;

        info.buffer = buffer;
        info.offset = offset;
        info.range = bufferSize;

        return info;
    }

    Buffer::~Buffer() {
        Unmap();
        vkDestroyBuffer(GetDevice(), buffer, GetVulkanAllocator());
        vkFreeMemory(GetDevice(), memory, GetVulkanAllocator());
    }
}