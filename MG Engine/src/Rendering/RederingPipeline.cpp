#include "RenderingPipeline.h"
#include "RenderingPipelineInternal.h"

#include "VulkanManagerInternal.h"
#include "SwapChainInternal.h"

#include "VulkanManager.h"
#include "SwapChain.h"
#include "OSManager.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <array>

VkPipeline GraphicsPipeline;
VkShaderModule VertexShader;
VkShaderModule FragmentShader;

VkPipelineLayout PipelineLayout;
std::vector<VkCommandBuffer> CommandBuffers;

void RPipeline::DefaultPipelineConfigInfo(size_t Width, size_t Height, RPipeline::PipelineConfigInfo& ConfigInfo) {
	ConfigInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	ConfigInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	ConfigInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	
	ConfigInfo.Viewport.x = 0.0f;
	ConfigInfo.Viewport.y = 0.0f;
	ConfigInfo.Viewport.width = (float)Width;
	ConfigInfo.Viewport.height = (float)Height;
	ConfigInfo.Viewport.minDepth = 0.0f;
	ConfigInfo.Viewport.maxDepth = 1.0f;

	ConfigInfo.Scissor.offset = { 0, 0 };
	ConfigInfo.Scissor.extent = { (unsigned int)Width, (unsigned int)Height };

	ConfigInfo.ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ConfigInfo.ViewportInfo.viewportCount = 1;
	ConfigInfo.ViewportInfo.pViewports = &ConfigInfo.Viewport;
	ConfigInfo.ViewportInfo.scissorCount = 1;
	ConfigInfo.ViewportInfo.pScissors = &ConfigInfo.Scissor;

	ConfigInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	ConfigInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
	ConfigInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	ConfigInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	ConfigInfo.RasterizationInfo.lineWidth = 1.0f;
	ConfigInfo.RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	ConfigInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	ConfigInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
	ConfigInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;
	ConfigInfo.RasterizationInfo.depthBiasClamp = 0.0f;
	ConfigInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;

	ConfigInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	ConfigInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
	ConfigInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	ConfigInfo.MultisampleInfo.minSampleShading = 1.0f;
	ConfigInfo.MultisampleInfo.pSampleMask = nullptr;
	ConfigInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;
	ConfigInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;

	ConfigInfo.ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ConfigInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
	ConfigInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	ConfigInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	ConfigInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	ConfigInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	ConfigInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	ConfigInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	ConfigInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ConfigInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
	ConfigInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	ConfigInfo.ColorBlendInfo.attachmentCount = 1;
	ConfigInfo.ColorBlendInfo.pAttachments = &ConfigInfo.ColorBlendAttachment;
	ConfigInfo.ColorBlendInfo.blendConstants[0] = 0.0f;
	ConfigInfo.ColorBlendInfo.blendConstants[1] = 0.0f;
	ConfigInfo.ColorBlendInfo.blendConstants[2] = 0.0f;
	ConfigInfo.ColorBlendInfo.blendConstants[3] = 0.0f;

	ConfigInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	ConfigInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
	ConfigInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
	ConfigInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	ConfigInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	ConfigInfo.DepthStencilInfo.minDepthBounds = 0.0f;
	ConfigInfo.DepthStencilInfo.maxDepthBounds = 1.0f;
	ConfigInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
	ConfigInfo.DepthStencilInfo.front = {};
	ConfigInfo.DepthStencilInfo.back = {};
}

std::vector<char> ReadFile(const std::string& FilePath) {
	std::ifstream FileInput(FilePath, std::ios::binary | std::ios::ate);

	size_t FileSize = (size_t)FileInput.tellg();
	std::vector<char> FileData(FileSize);

	FileInput.read(FileData.data(), FileSize);

	return FileData;
}
void CreateShaderModule(const std::vector<char>& Code, VkShaderModule* Module) {
	VkShaderModuleCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	CreateInfo.codeSize = Code.size();
	CreateInfo.pCode = (unsigned int*)Code.data();

	if (vkCreateShaderModule(Vulkan::GetDevice(), &CreateInfo, nullptr, Module) != VK_SUCCESS)
		throw std::runtime_error("Failed to create shader module!");
}
void InitiatePipeline() {
	RPipeline::PipelineConfigInfo ConfInfo{};
	DefaultPipelineConfigInfo(SwapChain::Width(), SwapChain::Height(), ConfInfo);
	ConfInfo.RenderPass = SwapChain::GetRenderPass();
	ConfInfo.PipelineLayout = PipelineLayout;

	auto VertCode = ReadFile("Shaders\\TestVertex.vert.spv");
	auto FragCode = ReadFile("Shaders\\TestFragment.frag.spv");

	CreateShaderModule(VertCode, &VertexShader);
	CreateShaderModule(FragCode, &FragmentShader);

	VkPipelineShaderStageCreateInfo ShaderStages[2]{};
	ShaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	ShaderStages[0].module = VertexShader;
	ShaderStages[0].pName = "main";
	ShaderStages[0].flags = 0;
	ShaderStages[0].pNext = nullptr;
	ShaderStages[0].pSpecializationInfo = nullptr;
	ShaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	ShaderStages[1].module = FragmentShader;
	ShaderStages[1].pName = "main";
	ShaderStages[1].flags = 0;
	ShaderStages[1].pNext = nullptr;
	ShaderStages[1].pSpecializationInfo = nullptr;

	VkPipelineVertexInputStateCreateInfo VertexInputInfo{};
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputInfo.vertexAttributeDescriptionCount = 0;
	VertexInputInfo.vertexBindingDescriptionCount = 0;
	VertexInputInfo.pVertexAttributeDescriptions = nullptr;
	VertexInputInfo.pVertexBindingDescriptions = nullptr;

	VkGraphicsPipelineCreateInfo PipelineInfo{};
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.stageCount = 2;
	PipelineInfo.pStages = ShaderStages;
	PipelineInfo.pVertexInputState = &VertexInputInfo;
	PipelineInfo.pInputAssemblyState = &ConfInfo.InputAssemblyInfo;
	PipelineInfo.pViewportState = &ConfInfo.ViewportInfo;
	PipelineInfo.pRasterizationState = &ConfInfo.RasterizationInfo;
	PipelineInfo.pMultisampleState = &ConfInfo.MultisampleInfo;
	PipelineInfo.pColorBlendState = &ConfInfo.ColorBlendInfo;
	PipelineInfo.pDepthStencilState = &ConfInfo.DepthStencilInfo;
	PipelineInfo.pDynamicState = nullptr;

	PipelineInfo.layout = ConfInfo.PipelineLayout;
	PipelineInfo.renderPass = ConfInfo.RenderPass;
	PipelineInfo.subpass = ConfInfo.Subpass;

	PipelineInfo.basePipelineIndex = -1;
	PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	auto Result = vkCreateGraphicsPipelines(Vulkan::GetDevice(), VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &GraphicsPipeline);
	if (Result != VK_SUCCESS)
		throw std::runtime_error("Failed to create graphics pipeline!");
}

void CreatePipelineLayout() {
	VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = 0;
	PipelineLayoutInfo.pSetLayouts = nullptr;
	PipelineLayoutInfo.pushConstantRangeCount = 0;
	PipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(Vulkan::GetDevice(), &PipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("Failed to create pipeline layout!");
}
void CreateCommandBuffers() {
	CommandBuffers.resize(SwapChain::ImageCount());

	VkCommandBufferAllocateInfo AllocInfo{};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandPool = Vulkan::GetCommandPool();
	AllocInfo.commandBufferCount = (unsigned int)CommandBuffers.size();

	if (vkAllocateCommandBuffers(Vulkan::GetDevice(), &AllocInfo, CommandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffers!");

	for (size_t i = 0; i < CommandBuffers.size(); i++) {
		VkCommandBufferBeginInfo BeginInfo{};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(CommandBuffers[i], &BeginInfo) != VK_SUCCESS)
			throw std::runtime_error("Failed to begin reccording the command buffer!");

		VkRenderPassBeginInfo RenderPassInfo{};
		RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassInfo.renderPass = SwapChain::GetRenderPass();
		RenderPassInfo.framebuffer = SwapChain::GetFramebuffer(i);
		RenderPassInfo.renderArea.offset = {0, 0};
		RenderPassInfo.renderArea.extent = SwapChain::GetSwapChainExtent();

		std::array<VkClearValue, 2> ClearValues{};
		ClearValues[0].color = {1.0f, 1.0f, 1.0f, 1.0f};
		ClearValues[1].depthStencil = {1.0f, 0};
		
		RenderPassInfo.clearValueCount = (unsigned int)ClearValues.size();
		RenderPassInfo.pClearValues = ClearValues.data();

		vkCmdBeginRenderPass(CommandBuffers[i], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);

		vkCmdDraw(CommandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(CommandBuffers[i]);
		if (vkEndCommandBuffer(CommandBuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("Failed to record command buffer!");
	}
}

void RPipeline::DrawFrame() {
	size_t ImageIndex{};
	auto Result = SwapChain::AcquireNextImage(&ImageIndex);

	if (Result != VK_SUCCESS && Result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("Failed to acquire swap chain image!");

	Result = SwapChain::SubmitCommandBuffers(&CommandBuffers[ImageIndex], &ImageIndex);
	if (Result != VK_SUCCESS)
		throw std::runtime_error("Failed to present swap chain image!");
}

void RPipeline::InitPipeline() {
	Vulkan::InitVulkan();
	SwapChain::InitSwapChain();
	CreatePipelineLayout();
	InitiatePipeline();

	CreateCommandBuffers();
}

void RPipeline::ClearPipeline() {
	vkDestroyShaderModule(Vulkan::GetDevice(), VertexShader, nullptr);
	vkDestroyShaderModule(Vulkan::GetDevice(), FragmentShader, nullptr);
	vkDestroyPipelineLayout(Vulkan::GetDevice(), PipelineLayout, nullptr);
	vkDestroyPipeline(Vulkan::GetDevice(), GraphicsPipeline, nullptr);

	SwapChain::ClearSwapChain();
	Vulkan::ClearVulkan();
}