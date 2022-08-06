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

        /// @brief Maps the buffer.
        /// @param size The size of the mapped region.
        /// @param offset The offset of the mapped region.
        /// @param handleErrors If the function should handle errors.
        /// @return VK_SUCCESS if handleErrors is set to true, otherwise the return code of the map function.
        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0, bool8_t handleErrors = true);
        /// @brief Unmaps the buffer.
        void Unmap();

        /// @brief Writes to the buffer.
        /// @param data A pointer to the data to write to the buffer.
        /// @param size The size of the data.
        /// @param offset The offset of the region to write to.
        void WriteToBuffer(const void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        /// @brief Writes to the buffer's specified index.
        /// @param data A pointer to the data to write to the buffer.
        /// @param index The index to write to.
        void WriteToIndex(const void* data, size_t index);
        /// @brief Reads from the buffer.
        /// @param data A pointer to the data to write to from the buffer.
        /// @param size The size of the data.
        /// @param offset The offset of the region to write from.
        void ReadFromBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        /// @brief Reads from the buffer's specified index.
        /// @param data A pointer to the data to write to from the buffer.
        /// @param index The index to read from.
        void ReadFromIndex(void* data, size_t index);
        /// @brief Flushes the buffer.
        /// @param size The size of the flushed region.
        /// @param offset The offset of the flushed region.
        /// @param handleErrors If the function should handle errors.
        /// @return VK_SUCCESS if handleErrors is set to true, otherwise the return code of the flush function.
        VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0, bool8_t handleErrors = true);
        /// @brief Flushes the buffer's specified index.
        /// @brief The index to flush.
        /// @param handleErrors If the function should handle errors.
        /// @return VK_SUCCESS if handleErrors is set to true, otherwise the return code of the flush function.
        VkResult FlushIndex(size_t index, bool8_t handleErrors = true);
        /// @brief Invalidates the buffer.
        /// @param size The size of the invalidated resion.
        /// @param handleErrors If the function should handle errors.
        /// @return VK_SUCCESS if handleErrors is set to true, otherwise the return code of the invalidate function.
        VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0, bool8_t handleErrors = true);
        /// @brief Invalidates the buffer's specified index.
        /// @param index THe index to invalidate.
        /// @param handleErrors If the function should handle errors.
        /// @return VK_SUCCESS if handleErrors is set to true, otherwise the return code of the invalidate function.
        VkResult InvalidateIndex(size_t index, bool8_t handleErrors = true);

        /// @brief Returns the buffer's descriptor info.
        /// @param size The size of the buffer region.
        /// @param offset The offset of the buffer's region.
        VkDescriptorBufferInfo GetDescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /// @brief Returns the Vulkan buffer.
        VkBuffer              GetBuffer()              const { 
            return buffer; 
        }
        /// @brief Returns a pointer to the mapped memory if it exists, otherwise a nullptr.
        void*                 GetMappedMemory()        const { 
            return mapped; 
        }
        /// @brief Returns the number of instances.
        size_t                GetInstanceCount()       const { 
            return instanceCount; 
        }
        /// @brief Returns the size of an instance.
        VkDeviceSize          GetInstanceSize()        const { 
            return instanceSize; 
        }
        /// @brief Returns the aligned size of an instance.
        VkDeviceSize          GetAlignmentSize()       const { 
            return instanceSize; 
        }
        /// @brief Returns the buffer's usage flags.
        VkBufferUsageFlags    GetUsageFlags()          const { 
            return usageFlags; 
        }
        /// @brief Returns the buffer's memory property flags.
        VkMemoryPropertyFlags GetMemoryPropertyFlags() const { 
            return memoryPropertyFlags; 
        }
        /// @brief Returns the buffer's size.
        VkDeviceSize          GetBufferSize()          const { 
            return bufferSize; 
        }

        ~Buffer();
    private:
        static VkDeviceSize GetAlignment(VkDeviceSize instanceSize);

        void* mapped = nullptr;
        VkBuffer buffer;
        VkDeviceMemory memory;

        VkDeviceSize bufferSize;
        size_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };
}