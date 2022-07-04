#pragma once

#include "Base/Asset.hpp"
#include "Vulkan/VulkanInclude.hpp"
#include "Vulkan/DescriptorPool.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Buffer.hpp"
#include "Shader.hpp"
#include "Core.hpp"

namespace wfe {
    /// @brief A graphics pipeline.
    class Pipeline : public Asset {
    public:
        /// @brief A struct that holds a shader stage.
        struct ShaderStage {
            Shader* shader;
            VkShaderStageFlagBits shaderStage;
        };
        /// @brief A struct that holds information about the pipeline. Used to initialize the pipeline.
        struct PipelineInfo {
            vector<ShaderStage> shaderStages{};
            vector<VkPushConstantRange> pushConstantRanges{};

            DescriptorPool::DescriptorPoolInfo descriptorPoolInfo{};
            VkDeviceSize globalBufferSize = 0;

            vector<VkVertexInputBindingDescription> vertexBindings{};
            vector<VkVertexInputAttributeDescription> vertexAttributes{};

            vector<VkDynamicState> dynamicStateEnables{};

            VkViewport viewport{};
            VkRect2D scissor{};

            VkPipelineViewportStateCreateInfo viewportInfo{};
            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
            VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
            VkPipelineMultisampleStateCreateInfo multisampleInfo{};
            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
            VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
            VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
            uint32_t subpass = 0;
        };

        Pipeline() = default;
        Pipeline(const Pipeline&) = delete;
        Pipeline(Pipeline&&) noexcept = delete;
        Pipeline(const PipelineInfo& info);

        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline& operator=(Pipeline&&) noexcept = delete;

        void LoadFromFile(const string& filePath) override;
        void SaveToFile(const string& filePath) override;

        /// @brief Populates the pipeline info with the default values.
        static void PopulatePipelineInfo(PipelineInfo& info);

        /// @brief Returns the Vulkan graphics pipeline.
        VkPipeline GetPipeline() const { return graphicsPipeline; }
        /// @brief Returns the Vulkan pipeline layout.
        VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
        /// @brief Returns the pipeline info.
        PipelineInfo GetPipelineInfo() const { return pipelineInfo; }
        /// @brief Returns the pipeline's descriptor pool.
        DescriptorPool* GetDescriptorPool() const { return descriptorPool; }
        /// @brief Returns the descriptor pool's current global buffer.
        Buffer* GetGlobalBuffer() const { 
            return globalBuffers[GetCurrentFrame()]; 
        }
        /// @brief Returns the descriptor pool's global buffers as an array of size MAX_FRAMES_IN_FLIGHT
        Buffer** GetGlobalBuffers() const {
            return (Buffer**)globalBuffers;
        }

        ~Pipeline();
    private:
        void Create() {
            CreateDescriptorPool();
            CreatePipelineLayout();
            CreatePipeline();
        }

        void CreateDescriptorPool();
        void CreatePipelineLayout();
        void CreatePipeline();

        PipelineInfo pipelineInfo{};
        VkPipelineLayout pipelineLayout{};
	    VkPipeline graphicsPipeline{};

        DescriptorPool* descriptorPool;
        Buffer* globalBuffers[MAX_FRAMES_IN_FLIGHT];
    };
}