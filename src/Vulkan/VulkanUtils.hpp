#pragma once

#ifndef MGE_VULKAN_UTILS_DONT_INCLUDE_VULKAN
#include "VulkanInclude.hpp"
#endif
#include "Core.hpp"

namespace mge {
    void LoadVkPushConstantRange                   (FileInput& stream, VkPushConstantRange*                    data, size_t count);
    void LoadVkDescriptorPoolSize                  (FileInput& stream, VkDescriptorPoolSize*                   data, size_t count);
    void LoadVkDescriptorSetLayoutBinding          (FileInput& stream, VkDescriptorSetLayoutBinding*           data, size_t count);
    void LoadVkVertexInputBindingDescription       (FileInput& stream, VkVertexInputBindingDescription*        data, size_t count);
    void LoadVkVertexInputAttributeDescription     (FileInput& stream, VkVertexInputAttributeDescription*      data, size_t count);
    void LoadVkViewport                            (FileInput& stream, VkViewport*                             data, size_t count);
    void LoadVkExtent2D                            (FileInput& stream, VkExtent2D*                             data, size_t count);
    void LoadVkExtent3D                            (FileInput& stream, VkExtent3D*                             data, size_t count);
    void LoadVkOffset2D                            (FileInput& stream, VkOffset2D*                             data, size_t count);
    void LoadVkOffset3D                            (FileInput& stream, VkOffset3D*                             data, size_t count);
    void LoadVkRect2D                              (FileInput& stream, VkRect2D*                               data, size_t count);
    void LoadVkPipelineViewportStateCreateInfo     (FileInput& stream, VkPipelineViewportStateCreateInfo*      data, size_t count, bool8_t loadViewports = false, bool8_t loadScissors = false);
    void LoadVkPipelineVertexInputStateCreateInfo  (FileInput& stream, VkPipelineVertexInputStateCreateInfo*   data, size_t count, bool8_t loadVertexInfo = false);
    void LoadVkPipelineInputAssemblyStateCreateInfo(FileInput& stream, VkPipelineInputAssemblyStateCreateInfo* data, size_t count);
    void LoadVkPipelineRasterizationStateCreateInfo(FileInput& stream, VkPipelineRasterizationStateCreateInfo* data, size_t count);
    void LoadVkPipelineMultisampleStateCreateInfo  (FileInput& stream, VkPipelineMultisampleStateCreateInfo*   data, size_t count, bool8_t loadSampleMask = false);
    void LoadVkPipelineColorBlendAttachmentState   (FileInput& stream, VkPipelineColorBlendAttachmentState*    data, size_t count);
    void LoadVkPipelineColorBlendStateCreateInfo   (FileInput& stream, VkPipelineColorBlendStateCreateInfo*    data, size_t count, bool8_t loadAttachments = false);
    void LoadVkPipelineDepthStencilStateCreateInfo (FileInput& stream, VkPipelineDepthStencilStateCreateInfo*  data, size_t count);
    void LoadVkPipelineDynamicStateCreateInfo      (FileInput& stream, VkPipelineDynamicStateCreateInfo*       data, size_t count, bool8_t loadDynamicStates = false);

    void SaveVkPushConstantRange                   (FileOutput& stream, const VkPushConstantRange*                    data, size_t count);
    void SaveVkDescriptorPoolSize                  (FileOutput& stream, const VkDescriptorPoolSize*                   data, size_t count);
    void SaveVkDescriptorSetLayoutBinding          (FileOutput& stream, const VkDescriptorSetLayoutBinding*           data, size_t count);
    void SaveVkVertexInputBindingDescription       (FileOutput& stream, const VkVertexInputBindingDescription*        data, size_t count);
    void SaveVkVertexInputAttributeDescription     (FileOutput& stream, const VkVertexInputAttributeDescription*      data, size_t count);
    void SaveVkViewport                            (FileOutput& stream, const VkViewport*                             data, size_t count);
    void SaveVkExtent2D                            (FileOutput& stream, const VkExtent2D*                             data, size_t count);
    void SaveVkExtent3D                            (FileOutput& stream, const VkExtent3D*                             data, size_t count);
    void SaveVkOffset2D                            (FileOutput& stream, const VkOffset2D*                             data, size_t count);
    void SaveVkOffset3D                            (FileOutput& stream, const VkOffset3D*                             data, size_t count);
    void SaveVkRect2D                              (FileOutput& stream, const VkRect2D*                               data, size_t count);
    void SaveVkPipelineViewportStateCreateInfo     (FileOutput& stream, const VkPipelineViewportStateCreateInfo*      data, size_t count, bool8_t loadViewports = false, bool8_t loadScissors = false);
    void SaveVkPipelineVertexInputStateCreateInfo  (FileOutput& stream, const VkPipelineVertexInputStateCreateInfo*   data, size_t count, bool8_t loadVertexInfo = false);
    void SaveVkPipelineInputAssemblyStateCreateInfo(FileOutput& stream, const VkPipelineInputAssemblyStateCreateInfo* data, size_t count);
    void SaveVkPipelineRasterizationStateCreateInfo(FileOutput& stream, const VkPipelineRasterizationStateCreateInfo* data, size_t count);
    void SaveVkPipelineMultisampleStateCreateInfo  (FileOutput& stream, const VkPipelineMultisampleStateCreateInfo*   data, size_t count, bool8_t loadSampleMask = false);
    void SaveVkPipelineColorBlendAttachmentState   (FileOutput& stream, const VkPipelineColorBlendAttachmentState*    data, size_t count);
    void SaveVkPipelineColorBlendStateCreateInfo   (FileOutput& stream, const VkPipelineColorBlendStateCreateInfo*    data, size_t count, bool8_t loadAttachments = false);
    void SaveVkPipelineDepthStencilStateCreateInfo (FileOutput& stream, const VkPipelineDepthStencilStateCreateInfo*  data, size_t count);
    void SaveVkPipelineDynamicStateCreateInfo      (FileOutput& stream, const VkPipelineDynamicStateCreateInfo*       data, size_t count, bool8_t loadDynamicStates = false);


    // Other functions
    /// @brief Converts the given VkResult to a string
    string VkResultToString(VkResult result);
}