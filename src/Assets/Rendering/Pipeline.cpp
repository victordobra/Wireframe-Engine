#include "Pipeline.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"

namespace mge {
	// Internal helper functions
	void Pipeline::CreatePipelineLayout(const PipelineInfo& info) {
		// Pipeline layout info
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		pipelineLayoutInfo.flags = 0;
		pipelineLayoutInfo.pNext = nullptr;

		// Create the pipeline layout
		if (vkCreatePipelineLayout(GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			console::OutFatalError("Failed to create pipeline layout!", 1);
	}
	void Pipeline::CreatePipeline(const PipelineInfo& info) {
		//Shader stages
		vector<VkPipelineShaderStageCreateInfo> vkShaderStages(info.shaderStages.size());

        for(size_t i = 0; i < info.shaderStages.size(); i++) {
            vkShaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vkShaderStages[i].stage = info.shaderStages[i].shaderStage;
            vkShaderStages[i].module = info.shaderStages[i].shader->GetModule();
            vkShaderStages[i].pName = "main";
			vkShaderStages[i].flags = 0;
            vkShaderStages[i].pNext = nullptr;
            vkShaderStages[i].pSpecializationInfo = nullptr;
        }

		//Pipeline info
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t)vkShaderStages.size();
		pipelineInfo.pStages = vkShaderStages.data();
		pipelineInfo.pVertexInputState = &info.vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &info.inputAssemblyInfo;
		pipelineInfo.pViewportState = &info.viewportInfo;
		pipelineInfo.pRasterizationState = &info.rasterizationInfo;
		pipelineInfo.pMultisampleState = &info.multisampleInfo;
		pipelineInfo.pColorBlendState = &info.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &info.depthStencilInfo;
		pipelineInfo.pDynamicState = &info.dynamicStateInfo;
		
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = GetRenderPass();
		pipelineInfo.subpass = info.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		
		//Creating the pipeline
		VkResult result = vkCreateGraphicsPipelines(GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
		if (result != VK_SUCCESS)
			console::OutFatalError("Failed to create graphics pipeline!", 1);
	}

	// External functions
    Pipeline::Pipeline(const PipelineInfo& info) {
		pipelineInfo = info;

		CreatePipelineLayout(pipelineInfo);
		CreatePipeline(pipelineInfo);
    }

	void Pipeline::LoadFromFile(const string& filePath) {
		FileInput input(filePath, StreamType::BINARY);

		// Read all of the shader stages
		uint64_t shaderStageCount{};
		input.Get((char_t*)&shaderStageCount, sizeof(uint64_t));

		pipelineInfo.shaderStages.resize(shaderStageCount);
		
		for(size_t i = 0; i < shaderStageCount; i++) {
			string location{};

			uint64_t stringLength{};
			input.Get((char_t*)&stringLength, sizeof(uint64_t));
			location.resize(stringLength);

			input.Get((char_t*)location.c_str(), stringLength * sizeof(char_t));
			location[stringLength] = 0;

			pipelineInfo.shaderStages[i].shader = dynamic_cast<Shader*>(Asset::LoadAssetFromFile<Shader>(location));
			input.Get((char_t*)&pipelineInfo.shaderStages[i].shaderStage, sizeof(VkShaderStageFlagBits));
		}

		// Read the rest of the pipeline
		input.Get((char_t*)&pipelineInfo.viewportInfo, sizeof(PipelineInfo) - sizeof(vector<ShaderStage>));

		input.Close();

		// Set some values
		pipelineInfo.colorBlendInfo.pAttachments = &pipelineInfo.colorBlendAttachment;
		pipelineInfo.dynamicStateInfo.pDynamicStates = pipelineInfo.dynamicStateEnables;

		// Create the pipeline layout and the pipeline
		CreatePipelineLayout(pipelineInfo);
		CreatePipeline(pipelineInfo);
	}
	void Pipeline::SaveToFile(const string& filePath) {
		FileOutput output(filePath, StreamType::BINARY);

		// Save all of the shader stages
		uint64_t shaderStageCount = (uint64_t)pipelineInfo.shaderStages.size();
		output.WriteBuffer((char_t*)&shaderStageCount, sizeof(uint64_t));

		for(size_t i = 0; i < shaderStageCount; i++) {
			const string& str = Asset::GetAssetLocation(pipelineInfo.shaderStages[i].shader);
			uint64_t stringLength = (uint64_t)str.length();

			output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
			output.WriteBuffer((char_t*)str.c_str(), stringLength * sizeof(char_t));
			output.WriteBuffer((char_t*)&pipelineInfo.shaderStages[i].shaderStage, sizeof(VkShaderStageFlagBits));
		}

		// Write the rest of the config info
		output.WriteBuffer((char_t*)&pipelineInfo.viewportInfo, sizeof(PipelineInfo) - sizeof(vector<ShaderStage>));

		output.Close();
	}

    void Pipeline::PopulatePipelineInfo(PipelineInfo& info) {
		// Viewport info
		info.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		info.viewportInfo.viewportCount = 1;
		info.viewportInfo.pViewports = nullptr;
		info.viewportInfo.scissorCount = 1;
		info.viewportInfo.pScissors = nullptr;
		info.viewportInfo.flags = 0;
		info.viewportInfo.pNext = nullptr;

		// Vertex input info
		info.vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		info.vertexInputInfo.vertexAttributeDescriptionCount = 0;
		info.vertexInputInfo.vertexBindingDescriptionCount = 0;
		info.vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		info.vertexInputInfo.pVertexBindingDescriptions = nullptr;
		info.vertexInputInfo.flags = 0;
		info.vertexInputInfo.pNext = nullptr;

        // Input assembly info
		info.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		info.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		info.inputAssemblyInfo.flags = 0;
		info.inputAssemblyInfo.pNext = nullptr;

		// Rasterization info
		info.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.rasterizationInfo.depthClampEnable = VK_FALSE;
		info.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		info.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		info.rasterizationInfo.lineWidth = 1.f;
		info.rasterizationInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		info.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.rasterizationInfo.depthBiasEnable = VK_FALSE;
		info.rasterizationInfo.depthBiasConstantFactor = 0.f;
		info.rasterizationInfo.depthBiasClamp = 0.f;
		info.rasterizationInfo.depthBiasSlopeFactor = 0.f;
		info.rasterizationInfo.flags = 0;
		info.rasterizationInfo.pNext = nullptr;

		// Multisample info
		info.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.multisampleInfo.sampleShadingEnable = VK_FALSE;
		info.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.multisampleInfo.minSampleShading = 1.0f;
		info.multisampleInfo.pSampleMask = nullptr;
		info.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		info.multisampleInfo.alphaToOneEnable = VK_FALSE;
		info.multisampleInfo.flags = 0;
		info.multisampleInfo.pNext = nullptr;

		// Color blend attachmemt
		info.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		info.colorBlendAttachment.blendEnable = VK_FALSE;
		info.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		info.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		info.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		info.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		info.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		info.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Color blend info
		info.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		info.colorBlendInfo.logicOpEnable = VK_FALSE;
		info.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		info.colorBlendInfo.attachmentCount = 1;
		info.colorBlendInfo.pAttachments = &info.colorBlendAttachment;
		info.colorBlendInfo.blendConstants[0] = 0.0f;
		info.colorBlendInfo.blendConstants[1] = 0.0f;
		info.colorBlendInfo.blendConstants[2] = 0.0f;
		info.colorBlendInfo.blendConstants[3] = 0.0f;
		info.colorBlendInfo.flags = 0;
		info.colorBlendInfo.pNext = nullptr;

		// Depth stencil info
		info.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		info.depthStencilInfo.depthTestEnable = VK_TRUE;
		info.depthStencilInfo.depthWriteEnable = VK_TRUE;
		info.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		info.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		info.depthStencilInfo.minDepthBounds = 0.0f;
		info.depthStencilInfo.maxDepthBounds = 1.0f;
		info.depthStencilInfo.stencilTestEnable = VK_FALSE;
		info.depthStencilInfo.front = {};
		info.depthStencilInfo.back = {};
		info.depthStencilInfo.flags = 0;
		info.depthStencilInfo.pNext = nullptr;

		// Dynamic state info
		info.dynamicStateEnables[0] = VK_DYNAMIC_STATE_VIEWPORT;
		info.dynamicStateEnables[1] = VK_DYNAMIC_STATE_SCISSOR;
		info.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		info.dynamicStateInfo.dynamicStateCount = 2;
		info.dynamicStateInfo.pDynamicStates = info.dynamicStateEnables;
		info.dynamicStateInfo.flags = 0;
		info.dynamicStateInfo.pNext = nullptr;
    }
	void Pipeline::DrawFrame() {
		VkCommandBuffer commandBuffer;

		uint32_t imageIndex;
		VkResult result = AcquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			console::OutFatalError("Failed to acquire swap chain image!", 1);

		//Allocate the command buffer
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GetCommandPool();
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
			console::OutFatalError("Failed to allocate command buffer!", 1);

		//Begin recording the command buffer
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			console::OutFatalError("Failed to begin recording command buffer!", 1);

		//Begin the render pass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = GetRenderPass();
		renderPassInfo.framebuffer = GetFrameBuffer((sint32_t)imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = GetSwapChainExtent();

		VkClearValue clearValues[2]{};

		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//Set the viewport and scissor
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float32_t)GetSwapChainExtent().width;
		viewport.height = (float32_t)GetSwapChainExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{ { 0, 0 }, GetSwapChainExtent() };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			console::OutFatalError("Failed to end recording command buffer!", 1);
		
		result = SubmitCommandBuffers(&commandBuffer, &imageIndex);
		if (result != VK_SUCCESS)
			console::OutFatalError("Failed to submit command buffers!", 1);
		
		vkDeviceWaitIdle(GetDevice());
		vkFreeCommandBuffers(GetDevice(), GetCommandPool(), 1, &commandBuffer);
	}

    Pipeline::~Pipeline() {
        vkDeviceWaitIdle(GetDevice());

        vkDestroyPipeline(GetDevice(), graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(GetDevice(), pipelineLayout, nullptr);
    }

	MGE_ASSET(Pipeline)
}