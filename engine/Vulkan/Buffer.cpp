#include "Buffer.hpp"

namespace wfe {
    Buffer::Buffer(VkDeviceSize instanceSize, size_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
        this->instanceSize = instanceSize;
        this->instanceCount = instanceCount;
        this->bufferSize = instanceSize * instanceCount;
        this->alignmentSize = PadUniformBufferSize(bufferSize);
        this->usageFlags = usageFlags;
        this->memoryPropertyFlags = memoryPropertyFlags;

        CreateBuffer(this->bufferSize, usageFlags, memoryPropertyFlags, this->buffer, this->memory);
    }

    VkResult Buffer::Map(VkDeviceSize size, VkDeviceSize offset) {
        assert(buffer && memory && "Called map on buffer before create!");
        return vkMapMemory(GetDevice(), memory, offset, size, 0, &mapped);
    }
    void Buffer::Unmap() {
        if(!mapped)
            return;

        vkUnmapMemory(GetDevice(), memory);
        mapped = nullptr;
    }

    void Buffer::WriteToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy to unmapped buffer!");

        if (size == VK_WHOLE_SIZE)
            memcpy(mapped, data, bufferSize);
        else {
            char_t* mappedChar = (char_t*)mapped;
            mappedChar += offset;
            memcpy(mappedChar, data, size);
        }
    }
    void Buffer::WriteToIndex(void* data, size_t index) {
        WriteToBuffer(data, alignmentSize, index * alignmentSize);
    }
    void Buffer::ReadFromBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy from unmapped buffer!");

        if (size == VK_WHOLE_SIZE)
            memcpy(data, mapped, bufferSize);
        else {
            char_t* mappedChar = (char_t*)mapped;
            mappedChar += offset;
            memcpy(data, mappedChar, size);
        }
    }
    void Buffer::ReadFromIndex(void* data, size_t index) {
        ReadFromBuffer(data, alignmentSize, index * alignmentSize);
    }
    VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.memory = memory;
        memoryRange.size = size;
        memoryRange.offset = offset;
        memoryRange.pNext = nullptr;

        return vkFlushMappedMemoryRanges(GetDevice(), 1, &memoryRange);
    }
    VkResult Buffer::FlushIndex(size_t index) {
        return Flush(alignmentSize, index * alignmentSize);
    }
    VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.memory = memory;
        memoryRange.size = size;
        memoryRange.offset = offset;
        memoryRange.pNext = nullptr;
        
        return vkInvalidateMappedMemoryRanges(GetDevice(), 1, &memoryRange);
    }
    VkResult Buffer::InvalidateIndex(size_t index) {
        return Invalidate(alignmentSize, index * alignmentSize);
    }

    VkDescriptorBufferInfo Buffer::GetDescriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        VkDescriptorBufferInfo info;

        info.buffer = buffer;
        info.offset = 0;
        info.range = bufferSize;

        return info;
    }

    Buffer::~Buffer() {
        Unmap();
        vkDestroyBuffer(GetDevice(), buffer, nullptr);
        vkFreeMemory(GetDevice(), memory, nullptr);
    }
}