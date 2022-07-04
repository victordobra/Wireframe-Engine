#include "VulkanUtils.hpp"

namespace wfe {
    // Load functions
    void LoadVkPushConstantRange                   (FileInput& stream, VkPushConstantRange*                    data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->stageFlags, sizeof(VkShaderStageFlagBits));
            stream.ReadBuffer((char_t*)&ptr->offset, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->size, sizeof(uint32_t));

            ++ptr;
        }
    }
    void LoadVkDescriptorPoolSize                  (FileInput& stream, VkDescriptorPoolSize*                   data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->type, sizeof(VkDescriptorType));
            stream.ReadBuffer((char_t*)&ptr->descriptorCount, sizeof(uint32_t));

            ++ptr;
        }
    }
    void LoadVkDescriptorSetLayoutBinding          (FileInput& stream, VkDescriptorSetLayoutBinding*           data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->binding, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->descriptorType, sizeof(VkDescriptorType));
            stream.ReadBuffer((char_t*)&ptr->descriptorCount, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->stageFlags, sizeof(VkShaderStageFlags));

            ++ptr;
        }
    }
    void LoadVkVertexInputBindingDescription       (FileInput& stream, VkVertexInputBindingDescription*        data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->binding, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->stride, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->inputRate, sizeof(VkVertexInputRate));

            ++ptr;
        }
    }
    void LoadVkVertexInputAttributeDescription     (FileInput& stream, VkVertexInputAttributeDescription*      data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->location, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->binding, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->format, sizeof(VkFormat));
            stream.ReadBuffer((char_t*)&ptr->offset, sizeof(uint32_t));

            ++ptr;
        }
    }
    void LoadVkViewport                            (FileInput& stream, VkViewport*                             data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->x, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->y, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->width, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->height, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->minDepth, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->maxDepth, sizeof(float32_t));

            ++ptr;
        }
    }
    void LoadVkExtent2D                            (FileInput& stream, VkExtent2D*                             data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->width, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->height, sizeof(uint32_t));

            ++ptr;
        }
    }
    void LoadVkExtent3D                            (FileInput& stream, VkExtent3D*                             data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->width, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->height, sizeof(uint32_t));
            stream.ReadBuffer((char_t*)&ptr->depth, sizeof(uint32_t));

            ++ptr;
        }
    }
    void LoadVkOffset2D                            (FileInput& stream, VkOffset2D*                             data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->x, sizeof(sint32_t));
            stream.ReadBuffer((char_t*)&ptr->y, sizeof(sint32_t));

            ++ptr;
        }
    }
    void LoadVkOffset3D                            (FileInput& stream, VkOffset3D*                             data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->x, sizeof(sint32_t));
            stream.ReadBuffer((char_t*)&ptr->y, sizeof(sint32_t));
            stream.ReadBuffer((char_t*)&ptr->z, sizeof(sint32_t));

            ++ptr;
        }
    }
    void LoadVkRect2D                              (FileInput& stream, VkRect2D*                               data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            LoadVkOffset2D(stream, &ptr->offset, 1);
            LoadVkExtent2D(stream, &ptr->extent, 1);

            ++ptr;
        }
    }
    void LoadVkPipelineViewportStateCreateInfo     (FileInput& stream, VkPipelineViewportStateCreateInfo*      data, size_t count, bool8_t loadViewports, bool8_t loadScissors) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineViewportStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->viewportCount, sizeof(uint32_t));

            if(loadViewports) {
                ptr->pViewports = new VkViewport[ptr->viewportCount];
                LoadVkViewport(stream, (VkViewport*)ptr->pViewports, ptr->viewportCount);
            } else
                ptr->pViewports = nullptr;
            
            stream.ReadBuffer((char_t*)&ptr->scissorCount, sizeof(uint32_t));
            
            if(loadScissors) {
                ptr->pScissors = new VkRect2D[ptr->scissorCount];
                LoadVkRect2D(stream, (VkRect2D*)ptr->pScissors, ptr->scissorCount);
            } else
                ptr->pScissors = nullptr;
            
            ++ptr;
        }
    }
    void LoadVkPipelineVertexInputStateCreateInfo  (FileInput& stream, VkPipelineVertexInputStateCreateInfo*   data, size_t count, bool8_t loadVertexInfo) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineVertexInputStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->vertexBindingDescriptionCount, sizeof(uint32_t));

            if(loadVertexInfo) {
                ptr->pVertexBindingDescriptions = new VkVertexInputBindingDescription[ptr->vertexBindingDescriptionCount];
                LoadVkVertexInputBindingDescription(stream, (VkVertexInputBindingDescription*)ptr->pVertexBindingDescriptions, ptr->vertexBindingDescriptionCount);
            } else
                ptr->pVertexBindingDescriptions = nullptr;
            
            stream.ReadBuffer((char_t*)&ptr->vertexAttributeDescriptionCount, sizeof(uint32_t));

            if(loadVertexInfo) {
                ptr->pVertexAttributeDescriptions = new VkVertexInputAttributeDescription[ptr->vertexAttributeDescriptionCount];
                LoadVkVertexInputAttributeDescription(stream, (VkVertexInputAttributeDescription*)ptr->pVertexAttributeDescriptions, ptr->vertexAttributeDescriptionCount);
            } else
                ptr->pVertexAttributeDescriptions = nullptr;
            
            ++ptr;
        }
    }
    void LoadVkPipelineInputAssemblyStateCreateInfo(FileInput& stream, VkPipelineInputAssemblyStateCreateInfo* data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineInputAssemblyStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->topology, sizeof(VkPrimitiveTopology));
            stream.ReadBuffer((char_t*)&ptr->primitiveRestartEnable, sizeof(VkBool32));

            ++ptr;
        }
    }
    void LoadVkPipelineRasterizationStateCreateInfo(FileInput& stream, VkPipelineRasterizationStateCreateInfo* data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineRasterizationStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->depthClampEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->rasterizerDiscardEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->polygonMode, sizeof(VkPolygonMode));
            stream.ReadBuffer((char_t*)&ptr->cullMode, sizeof(VkCullModeFlags));
            stream.ReadBuffer((char_t*)&ptr->frontFace, sizeof(VkFrontFace));
            stream.ReadBuffer((char_t*)&ptr->depthBiasEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->depthBiasConstantFactor, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->depthBiasClamp, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->depthBiasSlopeFactor, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->lineWidth, sizeof(float32_t));

            ++ptr;
        }
    }
    void LoadVkPipelineMultisampleStateCreateInfo  (FileInput& stream, VkPipelineMultisampleStateCreateInfo*   data, size_t count, bool8_t loadSampleMask) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineMultisampleStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->rasterizationSamples, sizeof(VkSampleCountFlagBits));
            stream.ReadBuffer((char_t*)&ptr->sampleShadingEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->minSampleShading, sizeof(float32_t));

            if(loadSampleMask) {
                ptr->pSampleMask = new VkSampleMask();
                stream.ReadBuffer((char_t*)ptr->pSampleMask, sizeof(VkSampleMask));
            } else
                ptr->pSampleMask = nullptr;
            
            stream.ReadBuffer((char_t*)&ptr->alphaToCoverageEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->alphaToOneEnable, sizeof(VkBool32));

            ++ptr;
        }
    }
    void LoadVkPipelineColorBlendAttachmentState   (FileInput& stream, VkPipelineColorBlendAttachmentState*    data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.ReadBuffer((char_t*)&ptr->blendEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->srcColorBlendFactor, sizeof(VkBlendFactor));
            stream.ReadBuffer((char_t*)&ptr->dstColorBlendFactor, sizeof(VkBlendFactor));
            stream.ReadBuffer((char_t*)&ptr->colorBlendOp, sizeof(VkBlendOp));
            stream.ReadBuffer((char_t*)&ptr->srcAlphaBlendFactor, sizeof(VkBlendFactor));
            stream.ReadBuffer((char_t*)&ptr->dstAlphaBlendFactor, sizeof(VkBlendFactor));
            stream.ReadBuffer((char_t*)&ptr->alphaBlendOp, sizeof(VkBlendOp));
            stream.ReadBuffer((char_t*)&ptr->colorWriteMask, sizeof(VkColorComponentFlags));

            ++ptr;
        }
    }
    void LoadVkPipelineColorBlendStateCreateInfo   (FileInput& stream, VkPipelineColorBlendStateCreateInfo*    data, size_t count, bool8_t loadAttachments) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineColorBlendStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->logicOpEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->logicOp, sizeof(VkLogicOp));
            stream.ReadBuffer((char_t*)&ptr->attachmentCount, sizeof(uint32_t));

            if(loadAttachments) {
                ptr->pAttachments = new VkPipelineColorBlendAttachmentState[ptr->attachmentCount];
                LoadVkPipelineColorBlendAttachmentState(stream, (VkPipelineColorBlendAttachmentState*)ptr->pAttachments, ptr->attachmentCount);
            } else
                ptr->pAttachments = nullptr;
        
            stream.ReadBuffer((char_t*)&ptr->blendConstants, 4 * sizeof(float32_t));

            ++ptr;
        }
    }
    void LoadVkPipelineDepthStencilStateCreateInfo (FileInput& stream, VkPipelineDepthStencilStateCreateInfo*  data, size_t count) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineDepthStencilStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->depthTestEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->depthWriteEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->depthCompareOp, sizeof(VkCompareOp));
            stream.ReadBuffer((char_t*)&ptr->depthBoundsTestEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->stencilTestEnable, sizeof(VkBool32));
            stream.ReadBuffer((char_t*)&ptr->front, sizeof(VkStencilOpState));
            stream.ReadBuffer((char_t*)&ptr->back, sizeof(VkStencilOpState));
            stream.ReadBuffer((char_t*)&ptr->minDepthBounds, sizeof(float32_t));
            stream.ReadBuffer((char_t*)&ptr->maxDepthBounds, sizeof(float32_t));

            ++ptr;
        }
    }
    void LoadVkPipelineDynamicStateCreateInfo      (FileInput& stream, VkPipelineDynamicStateCreateInfo*       data, size_t count, bool8_t loadDynamicStates) {
        auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            ptr->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            ptr->pNext = nullptr;

            stream.ReadBuffer((char_t*)&ptr->flags, sizeof(VkPipelineDynamicStateCreateFlags));
            stream.ReadBuffer((char_t*)&ptr->dynamicStateCount, sizeof(uint32_t));

            if(loadDynamicStates) {
                ptr->pDynamicStates = new VkDynamicState[ptr->dynamicStateCount];
                stream.ReadBuffer((char_t*)ptr->pDynamicStates, ptr->dynamicStateCount * sizeof(VkDynamicState));
            } else 
                ptr->pDynamicStates = nullptr;
            
            ++ptr;
        }
    }

    // Save functions
    void SaveVkPushConstantRange                   (FileOutput& stream, const VkPushConstantRange*                    data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->stageFlags, sizeof(VkShaderStageFlagBits));
            stream.WriteBuffer((char_t*)&ptr->offset, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->size, sizeof(uint32_t));

            ++ptr;
        }
    }
    void SaveVkDescriptorPoolSize                  (FileOutput& stream, const VkDescriptorPoolSize*                   data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->type, sizeof(VkDescriptorType));
            stream.WriteBuffer((char_t*)&ptr->descriptorCount, sizeof(uint32_t));

            ++ptr;
        }
    }
    void SaveVkDescriptorSetLayoutBinding          (FileOutput& stream, const VkDescriptorSetLayoutBinding*           data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->binding, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->descriptorType, sizeof(VkDescriptorType));
            stream.WriteBuffer((char_t*)&ptr->descriptorCount, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->stageFlags, sizeof(VkShaderStageFlags));

            ++ptr;
        }
    }
    void SaveVkVertexInputBindingDescription       (FileOutput& stream, const VkVertexInputBindingDescription*        data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->binding, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->stride, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->inputRate, sizeof(VkVertexInputRate));

            ++ptr;
        }
    }
    void SaveVkVertexInputAttributeDescription     (FileOutput& stream, const VkVertexInputAttributeDescription*      data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->location, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->binding, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->format, sizeof(VkFormat));
            stream.WriteBuffer((char_t*)&ptr->offset, sizeof(uint32_t));

            ++ptr;
        }
    }
    void SaveVkViewport                            (FileOutput& stream, const VkViewport*                             data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->x, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->y, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->width, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->height, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->minDepth, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->maxDepth, sizeof(float32_t));

            ++ptr;
        }
    }
    void SaveVkExtent2D                            (FileOutput& stream, const VkExtent2D*                             data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->width, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->height, sizeof(uint32_t));

            ++ptr;
        }
    }
    void SaveVkExtent3D                            (FileOutput& stream, const VkExtent3D*                             data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->width, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->height, sizeof(uint32_t));
            stream.WriteBuffer((char_t*)&ptr->depth, sizeof(uint32_t));

            ++ptr;
        }
    }
    void SaveVkOffset2D                            (FileOutput& stream, const VkOffset2D*                             data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->x, sizeof(sint32_t));
            stream.WriteBuffer((char_t*)&ptr->y, sizeof(sint32_t));

            ++ptr;
        }
    }
    void SaveVkOffset3D                            (FileOutput& stream, const VkOffset3D*                             data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->x, sizeof(sint32_t));
            stream.WriteBuffer((char_t*)&ptr->y, sizeof(sint32_t));
            stream.WriteBuffer((char_t*)&ptr->z, sizeof(sint32_t));

            ++ptr;
        }
    }
    void SaveVkRect2D                              (FileOutput& stream, const VkRect2D*                               data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            SaveVkOffset2D(stream, &ptr->offset, 1);
            SaveVkExtent2D(stream, &ptr->extent, 1);

            ++ptr;
        }
    }
    void SaveVkPipelineViewportStateCreateInfo     (FileOutput& stream, const VkPipelineViewportStateCreateInfo*      data, size_t count, bool8_t saveViewports, bool8_t saveScissors) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineViewportStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->viewportCount, sizeof(uint32_t));

            if(saveViewports)
                SaveVkViewport(stream, ptr->pViewports, ptr->viewportCount);
            
            stream.WriteBuffer((char_t*)&ptr->scissorCount, sizeof(uint32_t));

            if(saveScissors)
                SaveVkRect2D(stream, ptr->pScissors, ptr->scissorCount);
            
            ++ptr;
        }
    }
    void SaveVkPipelineVertexInputStateCreateInfo  (FileOutput& stream, const VkPipelineVertexInputStateCreateInfo*   data, size_t count, bool8_t saveVertexInfo) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineVertexInputStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->vertexBindingDescriptionCount, sizeof(uint32_t));

            if(saveVertexInfo) 
                SaveVkVertexInputBindingDescription(stream, ptr->pVertexBindingDescriptions, ptr->vertexBindingDescriptionCount);
            
            stream.WriteBuffer((char_t*)&ptr->vertexAttributeDescriptionCount, sizeof(uint32_t));

            if(saveVertexInfo) 
                SaveVkVertexInputAttributeDescription(stream, ptr->pVertexAttributeDescriptions, ptr->vertexAttributeDescriptionCount);
            
            ++ptr;
        }
    }
    void SaveVkPipelineInputAssemblyStateCreateInfo(FileOutput& stream, const VkPipelineInputAssemblyStateCreateInfo* data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineInputAssemblyStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->topology, sizeof(VkPrimitiveTopology));
            stream.WriteBuffer((char_t*)&ptr->primitiveRestartEnable, sizeof(VkBool32));

            ++ptr;
        }
    }
    void SaveVkPipelineRasterizationStateCreateInfo(FileOutput& stream, const VkPipelineRasterizationStateCreateInfo* data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineRasterizationStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->depthClampEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->rasterizerDiscardEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->polygonMode, sizeof(VkPolygonMode));
            stream.WriteBuffer((char_t*)&ptr->cullMode, sizeof(VkCullModeFlags));
            stream.WriteBuffer((char_t*)&ptr->frontFace, sizeof(VkFrontFace));
            stream.WriteBuffer((char_t*)&ptr->depthBiasEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->depthBiasConstantFactor, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->depthBiasClamp, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->depthBiasSlopeFactor, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->lineWidth, sizeof(float32_t));

            ++ptr;
        }
    }
    void SaveVkPipelineMultisampleStateCreateInfo  (FileOutput& stream, const VkPipelineMultisampleStateCreateInfo*   data, size_t count, bool8_t saveSampleMask) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineMultisampleStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->rasterizationSamples, sizeof(VkSampleCountFlagBits));
            stream.WriteBuffer((char_t*)&ptr->sampleShadingEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->minSampleShading, sizeof(float32_t));

            if(saveSampleMask) 
                stream.WriteBuffer((char_t*)ptr->pSampleMask, sizeof(VkSampleMask));
            
            stream.WriteBuffer((char_t*)&ptr->alphaToCoverageEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->alphaToOneEnable, sizeof(VkBool32));

            ++ptr;
        }
    }
    void SaveVkPipelineColorBlendAttachmentState   (FileOutput& stream, const VkPipelineColorBlendAttachmentState*    data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->blendEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->srcColorBlendFactor, sizeof(VkBlendFactor));
            stream.WriteBuffer((char_t*)&ptr->dstColorBlendFactor, sizeof(VkBlendFactor));
            stream.WriteBuffer((char_t*)&ptr->colorBlendOp, sizeof(VkBlendOp));
            stream.WriteBuffer((char_t*)&ptr->srcAlphaBlendFactor, sizeof(VkBlendFactor));
            stream.WriteBuffer((char_t*)&ptr->dstAlphaBlendFactor, sizeof(VkBlendFactor));
            stream.WriteBuffer((char_t*)&ptr->alphaBlendOp, sizeof(VkBlendOp));
            stream.WriteBuffer((char_t*)&ptr->colorWriteMask, sizeof(VkColorComponentFlags));

            ++ptr;
        }
    }
    void SaveVkPipelineColorBlendStateCreateInfo   (FileOutput& stream, const VkPipelineColorBlendStateCreateInfo*    data, size_t count, bool8_t saveAttachments) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineColorBlendStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->logicOpEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->logicOp, sizeof(VkLogicOp));
            stream.WriteBuffer((char_t*)&ptr->attachmentCount, sizeof(uint32_t));

            if(saveAttachments)
                SaveVkPipelineColorBlendAttachmentState(stream, ptr->pAttachments, ptr->attachmentCount);
        
            stream.WriteBuffer((char_t*)&ptr->blendConstants, 4 * sizeof(float32_t));

            ++ptr;
        }
    }
    void SaveVkPipelineDepthStencilStateCreateInfo (FileOutput& stream, const VkPipelineDepthStencilStateCreateInfo*  data, size_t count) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineDepthStencilStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->depthTestEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->depthWriteEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->depthCompareOp, sizeof(VkCompareOp));
            stream.WriteBuffer((char_t*)&ptr->depthBoundsTestEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->stencilTestEnable, sizeof(VkBool32));
            stream.WriteBuffer((char_t*)&ptr->front, sizeof(VkStencilOpState));
            stream.WriteBuffer((char_t*)&ptr->back, sizeof(VkStencilOpState));
            stream.WriteBuffer((char_t*)&ptr->minDepthBounds, sizeof(float32_t));
            stream.WriteBuffer((char_t*)&ptr->maxDepthBounds, sizeof(float32_t));

            ++ptr;
        }
    }
    void SaveVkPipelineDynamicStateCreateInfo      (FileOutput& stream, const VkPipelineDynamicStateCreateInfo*       data, size_t count, bool8_t saveDynamicStates) {
        const auto* ptr = data;

        for(size_t i = 0; i < count; ++i) {
            stream.WriteBuffer((char_t*)&ptr->flags, sizeof(VkPipelineDynamicStateCreateFlags));
            stream.WriteBuffer((char_t*)&ptr->dynamicStateCount, sizeof(uint32_t));

            if(saveDynamicStates)
                stream.WriteBuffer((char_t*)ptr->pDynamicStates, ptr->dynamicStateCount * sizeof(VkDynamicState));
            
            ++ptr;
        }
    }

    // Other functions
    string VkResultToString(VkResult result) {
        switch(result) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:
            return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR:
            return "VK_SUBOPTIMAL_KH";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "VK_ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV:
            return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_EXT:
            return "VK_ERROR_NOT_PERMITTED_EXT";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR:
            return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR:
            return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR:
            return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_PIPELINE_COMPILE_REQUIRED_EXT:
            return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
        case VK_RESULT_MAX_ENUM:
            return "VK_RESULT_MAX_ENUM";
        default:
            return (string)"Unknown" + ToString((uint64_t)result);
        }
    }
}