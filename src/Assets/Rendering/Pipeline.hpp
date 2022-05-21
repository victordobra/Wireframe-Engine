#pragma once

#include "Base/Asset.hpp"
#include "Vulkan/VulkanInclude.hpp"
#include "Shader.hpp"
#include "Core.hpp"

namespace mge {
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
            vector<ShaderStage> shaderStages;
            VkPipelineViewportStateCreateInfo viewportInfo;
            VkPipelineVertexInputStateCreateInfo vertexInputInfo;
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
            VkPipelineRasterizationStateCreateInfo rasterizationInfo;
            VkPipelineMultisampleStateCreateInfo multisampleInfo;
            VkPipelineColorBlendAttachmentState colorBlendAttachment;
            VkPipelineColorBlendStateCreateInfo colorBlendInfo;
            VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
            VkDynamicState dynamicStateEnables[2];
            VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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

        void DrawFrame();

        ~Pipeline();
    private:
        void CreatePipelineLayout(const PipelineInfo& info);
        void CreatePipeline(const PipelineInfo& info);

        PipelineInfo pipelineInfo{};
        VkPipelineLayout pipelineLayout{};
	    VkPipeline graphicsPipeline{};
    };
}