#include "RenderingPipeline.h"
#include "VulkanDevice.h"
#include "SwapChain.h"
#include "VulkanModel.h"
#include "DescriptorPool.h"
#include "Shader.h"
#include "Debugger.h"

#include "GraphicsInfo.h"
#include "EngineMath.h"
#include "ShaderData.h"

#include <vector>
#include <fstream>
#include <iostream>

namespace mge {
	struct PipelineConfigInfo {
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;

		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(PipelineConfigInfo&&) noexcept = delete;
		
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = (VkPipelineLayout)nullptr;
		VkRenderPass renderPass = (VkRenderPass)nullptr;
		uint32_t subpass = 0;

		~PipelineConfigInfo() = default;
	};

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	Shader* vertShader;
	Shader* fragShader;

	static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo) {
		//Input assembly info
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		//Viewport info
		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = nullptr;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = nullptr;

		//Rasterization info
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		//Multisample info
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		//Color blend attachmemt
		configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		//Color blend info
		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f;   // Optional
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;   // Optional
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;   // Optional
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;   // Optional

		//Depth stencil info
		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {};  // Optional
		configInfo.depthStencilInfo.back = {};   // Optional

		//Dynamic state info
		configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

#pragma warning( push )
#pragma warning( disable: 26812 )
		configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
#pragma warning( pop )

		configInfo.dynamicStateInfo.dynamicStateCount = (uint32_t)configInfo.dynamicStateEnables.size();
		configInfo.dynamicStateInfo.flags = 0;
	}

	static void CreatePipelineLayout(std::vector<VkDescriptorSetLayout> descriptorSetLayouts) {
		//Push constant info
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantData);

		//Pipeline layout info
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		//Creating the pipeline layout
		if (vkCreatePipelineLayout(GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			OutFatalError("Failed to create pipeline layout!");
	}
	static void CreateGraphicsPipeline(const char_t* vertPath, const char_t* fragPath) {
		//Create the pipeline config info
		PipelineConfigInfo pipelineConfigInfo{};
		DefaultPipelineConfigInfo(pipelineConfigInfo);
		pipelineConfigInfo.renderPass = GetRenderPass();
		pipelineConfigInfo.pipelineLayout = pipelineLayout;

		//Creating the shaders
		vertShader = new Shader(vertPath);
		fragShader = new Shader(fragPath);

		//Shader stages
		VkPipelineShaderStageCreateInfo shaderStages[2]{};

		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShader->GetShaderModule();
		shaderStages[0].pName = "main";
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShader->GetShaderModule();
		shaderStages[1].pName = "main";
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;
		
		//Vertex input info
		auto bindingDescriptions = VulkanModel::Vertex::GetBindingDescriptions();
		auto attributeDescriptions = VulkanModel::Vertex::GetAttributeDescriptions();
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
		vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)bindingDescriptions.size();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

		//Pipeline info
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &pipelineConfigInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &pipelineConfigInfo.viewportInfo;
		pipelineInfo.pRasterizationState = &pipelineConfigInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &pipelineConfigInfo.multisampleInfo;
		pipelineInfo.pColorBlendState = &pipelineConfigInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &pipelineConfigInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = &pipelineConfigInfo.dynamicStateInfo;

		pipelineInfo.layout = pipelineConfigInfo.pipelineLayout;
		pipelineInfo.renderPass = pipelineConfigInfo.renderPass;
		pipelineInfo.subpass = pipelineConfigInfo.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		//Creating the pipeline
		auto result = vkCreateGraphicsPipelines(GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
		if (result != VK_SUCCESS)
			OutFatalError("Failed to create graphics pipeline!");
	}

	void InitiatePipeline() {
		InitiateDevice();
		InitiateSwapChain({ (uint32_t)GetScreenWidth(), (uint32_t)GetScreenHeight() });
		InitiateDescriptorPool();
		CreatePipelineLayout(GetDescriptorLayouts());
		CreateGraphicsPipeline("Shaders\\VertShader.vert.spv", "Shaders\\FragShader.frag.spv");
	}
	void ClearPipeline() {
		vkDeviceWaitIdle(GetDevice());
		
		delete vertShader;
		delete fragShader;

		vkDestroyPipeline(GetDevice(), graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(GetDevice(), pipelineLayout, nullptr);

		ClearDescriptorPool();
		ClearSwapChain();
		ClearDevice();
	}

	void PipelineBind(VkCommandBuffer commandBuffer) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}

	VkPipelineLayout GetPipelineLayout() {
		return pipelineLayout;
	}
}