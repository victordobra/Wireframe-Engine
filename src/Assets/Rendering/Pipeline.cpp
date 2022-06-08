#include "Pipeline.hpp"
#include "Material.hpp"
#include "Vulkan/Device.hpp"

namespace mge {
	// Internal helper functions
	void Pipeline::CreateDescriptorPool() {
		if(pipelineInfo.globalBufferSize) {
			// The descriptor pool info isn't set, create it
			size_t materialCount = 0;

			for(const auto& shaderStage : pipelineInfo.shaderStages)
				materialCount += (size_t)(shaderStage.shader->materials.end() - shaderStage.shader->materials.begin());
			
			// Set descriptor pool sizes
			pipelineInfo.descriptorPoolInfo.descriptorPoolSizes.resize(1);

			VkDescriptorPoolSize uniformBufferSize;
			uniformBufferSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniformBufferSize.descriptorCount = materialCount * MAX_FRAMES_IN_FLIGHT /*<- Material buffers*/ + MAX_FRAMES_IN_FLIGHT /*<- Global buffer*/;

			pipelineInfo.descriptorPoolInfo.descriptorPoolSizes[0] = uniformBufferSize;
			
			// Set descriptor set layout bindings
			pipelineInfo.descriptorPoolInfo.descriptorSetLayouts.resize(materialCount /*<- Material buffers*/ + 1 /*<- Global buffer*/);

			VkDescriptorSetLayoutBinding materialBinding;
			materialBinding.binding = 0;
			materialBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			materialBinding.descriptorCount = 1;
			materialBinding.pImmutableSamplers = nullptr;
			materialBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			VkDescriptorSetLayoutBinding globalBinding;
			globalBinding.binding = 0;
			globalBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			globalBinding.descriptorCount = 1;
			globalBinding.pImmutableSamplers = nullptr;
			globalBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;

			pipelineInfo.descriptorPoolInfo.descriptorSetLayouts[0].bindings = { globalBinding };
			
			for(size_t i = 1; i < pipelineInfo.descriptorPoolInfo.descriptorSetLayouts.size(); ++i)
				pipelineInfo.descriptorPoolInfo.descriptorSetLayouts[i].bindings = { materialBinding };
		}

		descriptorPool = new DescriptorPool(pipelineInfo.descriptorPoolInfo);
		pipelineInfo.descriptorPoolInfo = descriptorPool->GetInfo();

		if(pipelineInfo.globalBufferSize) {
			// Create and write the global buffers
			for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
				// Create the buffer
				globalBuffers[i] = new Buffer(pipelineInfo.globalBufferSize, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

				// Get the descriptor info
				VkDescriptorBufferInfo bufferInfo = globalBuffers[i]->GetDescriptorInfo();

				// Set information about the write
				VkWriteDescriptorSet write{};

				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.dstBinding = 0;
				write.pBufferInfo = &bufferInfo;
				write.descriptorCount = 1;
				write.dstSet = descriptorPool->GetDescriptorSets()[descriptorPool->GetDescriptorSetIndex(0, i)];

				vkUpdateDescriptorSets(GetDevice(), 1, &write, 0, nullptr);
			}

			// Write the material buffers
			size_t descriptorIndex = MAX_FRAMES_IN_FLIGHT;
			vector<Material*> materials;

			for(auto& shaderStage : pipelineInfo.shaderStages)
				for(auto* material : shaderStage.shader->materials){
					materials.push_back(material);
				}

			for(size_t i = 0; i < materials.size(); ++i)
				for(size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; ++j) {
					// Get the descriptor info
					VkDescriptorBufferInfo bufferInfo = materials[i]->GetBuffers()[j]->GetDescriptorInfo();

					// Set information about the write
					VkWriteDescriptorSet write{};

					write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					write.dstBinding = 0;
					write.pBufferInfo = &bufferInfo;
					write.descriptorCount = 1;
					write.dstSet = descriptorPool->GetDescriptorSets()[descriptorIndex++];

					vkUpdateDescriptorSets(GetDevice(), 1, &write, 0, nullptr);
				}
		}
	}
	void Pipeline::CreatePipelineLayout() {
		// Add every descriptor set layout in a vector
		vector<VkDescriptorSetLayout> layouts(pipelineInfo.descriptorPoolInfo.descriptorSetLayouts.size());
		for(size_t i = 0; i < layouts.size(); ++i) {
			VkDescriptorSetLayout layout = pipelineInfo.descriptorPoolInfo.descriptorSetLayouts[i].layout;
			layouts[i] = pipelineInfo.descriptorPoolInfo.descriptorSetLayouts[i].layout;
		}

		// Pipeline layout info
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = (uint32_t)layouts.size();
		pipelineLayoutInfo.pSetLayouts = layouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = (uint32_t)pipelineInfo.pushConstantRanges.size();
		pipelineLayoutInfo.pPushConstantRanges = pipelineInfo.pushConstantRanges.data();
		pipelineLayoutInfo.flags = 0;
		pipelineLayoutInfo.pNext = nullptr;

		// Create the pipeline layout
		if (vkCreatePipelineLayout(GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			console::OutFatalError("Failed to create pipeline layout!", 1);
	}
	void Pipeline::CreatePipeline() {
		// Set vertex binding and attribute descriptions
		pipelineInfo.vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)pipelineInfo.vertexBindings.size();
		pipelineInfo.vertexInputInfo.pVertexBindingDescriptions = pipelineInfo.vertexBindings.data();
		pipelineInfo.vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)pipelineInfo.vertexAttributes.size();
		pipelineInfo.vertexInputInfo.pVertexAttributeDescriptions = pipelineInfo.vertexAttributes.data();

		// Add itself to every single shader
		for(const auto& shaderStage : pipelineInfo.shaderStages)
			shaderStage.shader->pipeline = this;

		// Shader stages
		vector<VkPipelineShaderStageCreateInfo> vkShaderStages(pipelineInfo.shaderStages.size());

        for(size_t i = 0; i < pipelineInfo.shaderStages.size(); i++) {
            vkShaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vkShaderStages[i].stage = pipelineInfo.shaderStages[i].shaderStage;
            vkShaderStages[i].module = pipelineInfo.shaderStages[i].shader->GetModule();
            vkShaderStages[i].pName = "main";
			vkShaderStages[i].flags = 0;
            vkShaderStages[i].pNext = nullptr;
            vkShaderStages[i].pSpecializationInfo = nullptr;
        }

		// Pipeline info
		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stageCount = (uint32_t)vkShaderStages.size();
		pipelineCreateInfo.pStages = vkShaderStages.data();
		pipelineCreateInfo.pVertexInputState = &pipelineInfo.vertexInputInfo;
		pipelineCreateInfo.pInputAssemblyState = &pipelineInfo.inputAssemblyInfo;
		pipelineCreateInfo.pViewportState = &pipelineInfo.viewportInfo;
		pipelineCreateInfo.pRasterizationState = &pipelineInfo.rasterizationInfo;
		pipelineCreateInfo.pMultisampleState = &pipelineInfo.multisampleInfo;
		pipelineCreateInfo.pColorBlendState = &pipelineInfo.colorBlendInfo;
		pipelineCreateInfo.pDepthStencilState = &pipelineInfo.depthStencilInfo;
		pipelineCreateInfo.pDynamicState = &pipelineInfo.dynamicStateInfo;
		
		pipelineCreateInfo.layout = pipelineLayout;
		pipelineCreateInfo.renderPass = GetRenderPass();
		pipelineCreateInfo.subpass = pipelineInfo.subpass;

		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		
		// Create the pipeline
		VkResult result = vkCreateGraphicsPipelines(GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
		if (result != VK_SUCCESS)
			console::OutFatalError("Failed to create graphics pipeline!", 1);
	}

	// External functions
    Pipeline::Pipeline(const PipelineInfo& info) {
		pipelineInfo = info;

		Create();
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

		// Read the push constant ranges
		uint64_t pushConstantRangeCount{};
		input.Get((char_t*)&pushConstantRangeCount, sizeof(uint64_t));
		pipelineInfo.pushConstantRanges.resize((size_t)pushConstantRangeCount);
		input.Get((char_t*)pipelineInfo.pushConstantRanges.data(), pushConstantRangeCount * sizeof(VkPushConstantRange));

		// Read the vertex bindings
		uint64_t vertexBindingCount{};
		input.Get((char_t*)&vertexBindingCount, sizeof(uint64_t));
		pipelineInfo.vertexBindings.resize((size_t)vertexBindingCount);
		input.Get((char_t*)pipelineInfo.vertexBindings.data(), vertexBindingCount * sizeof(VkVertexInputBindingDescription));

		// Read the vertex attributes
		uint64_t vertexAttributeCount{};
		input.Get((char_t*)&vertexAttributeCount, sizeof(uint64_t));
		pipelineInfo.vertexAttributes.resize((size_t)vertexAttributeCount);
		input.Get((char_t*)pipelineInfo.vertexAttributes.data(), vertexAttributeCount * sizeof(VkVertexInputAttributeDescription));

		// Read the rest of the pipeline info
		input.Get((char_t*)&pipelineInfo.viewportInfo, sizeof(VkPipelineViewportStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.vertexInputInfo, sizeof(VkPipelineVertexInputStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.inputAssemblyInfo, sizeof(VkPipelineInputAssemblyStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.rasterizationInfo, sizeof(VkPipelineRasterizationStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.multisampleInfo, sizeof(VkPipelineMultisampleStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.colorBlendAttachment, sizeof(VkPipelineColorBlendAttachmentState));
		input.Get((char_t*)&pipelineInfo.colorBlendInfo, sizeof(VkPipelineColorBlendStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.depthStencilInfo, sizeof(VkPipelineDepthStencilStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.dynamicStateEnables, 2 * sizeof(VkDynamicState));
		input.Get((char_t*)&pipelineInfo.dynamicStateInfo, sizeof(VkPipelineDynamicStateCreateInfo));
		input.Get((char_t*)&pipelineInfo.subpass, sizeof(uint32_t));

		input.Close();

		// Set some values
		pipelineInfo.colorBlendInfo.pAttachments = &pipelineInfo.colorBlendAttachment;
		pipelineInfo.dynamicStateInfo.pDynamicStates = pipelineInfo.dynamicStateEnables;

		// Create the pipeline
		Create();
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

		// Save the push constant ranges
		uint64_t pushConstantRangeCount = (uint64_t)pipelineInfo.pushConstantRanges.size();
		output.WriteBuffer((char_t*)&pushConstantRangeCount, sizeof(uint64_t));
		output.WriteBuffer((char_t*)pipelineInfo.pushConstantRanges.data(), pushConstantRangeCount * sizeof(VkPushConstantRange));

		// Save the descriptor set layouts
		uint64_t descriptorSetLayoutCount = (uint64_t)pipelineInfo.descriptorPoolInfo.descriptorSetLayouts.size();
		output.WriteBuffer((char_t*)&descriptorSetLayoutCount, sizeof(uint64_t));

		for(const auto& descriptorSetLayout : pipelineInfo.descriptorPoolInfo.descriptorSetLayouts) {
			uint64_t descriptorBindingCount = (uint64_t)descriptorSetLayout.bindings.size();
			output.WriteBuffer((char_t*)&descriptorBindingCount, sizeof(uint64_t));
		}

		// Save the vertex bindings
		uint64_t vertexBindingCount = (uint64_t)pipelineInfo.vertexBindings.size();
		output.WriteBuffer((char_t*)&vertexBindingCount, sizeof(uint64_t));
		output.WriteBuffer((char_t*)pipelineInfo.vertexBindings.data(), vertexBindingCount * sizeof(VkVertexInputBindingDescription));

		// Save the vertex attributes
		uint64_t vertexAttributeCount = (uint64_t)pipelineInfo.vertexAttributes.size();
		output.WriteBuffer((char_t*)&vertexAttributeCount, sizeof(uint64_t));
		output.WriteBuffer((char_t*)pipelineInfo.vertexAttributes.data(), vertexAttributeCount * sizeof(VkVertexInputAttributeDescription));

		// Write the rest of the pipeline info
		output.WriteBuffer((char_t*)&pipelineInfo.viewportInfo, sizeof(VkPipelineViewportStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.vertexInputInfo, sizeof(VkPipelineVertexInputStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.inputAssemblyInfo, sizeof(VkPipelineInputAssemblyStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.rasterizationInfo, sizeof(VkPipelineRasterizationStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.multisampleInfo, sizeof(VkPipelineMultisampleStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.colorBlendAttachment, sizeof(VkPipelineColorBlendAttachmentState));
		output.WriteBuffer((char_t*)&pipelineInfo.colorBlendInfo, sizeof(VkPipelineColorBlendStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.depthStencilInfo, sizeof(VkPipelineDepthStencilStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.dynamicStateEnables, 2 * sizeof(VkDynamicState));
		output.WriteBuffer((char_t*)&pipelineInfo.dynamicStateInfo, sizeof(VkPipelineDynamicStateCreateInfo));
		output.WriteBuffer((char_t*)&pipelineInfo.subpass, sizeof(uint32_t));

		output.Close();
	}

    void Pipeline::PopulatePipelineInfo(PipelineInfo& pipelineInfo) {
		// Viewport info
		pipelineInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipelineInfo.viewportInfo.viewportCount = 1;
		pipelineInfo.viewportInfo.pViewports = nullptr;
		pipelineInfo.viewportInfo.scissorCount = 1;
		pipelineInfo.viewportInfo.pScissors = nullptr;
		pipelineInfo.viewportInfo.flags = 0;
		pipelineInfo.viewportInfo.pNext = nullptr;

		// Vertex input info
		pipelineInfo.vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		pipelineInfo.vertexInputInfo.vertexAttributeDescriptionCount = 0;
		pipelineInfo.vertexInputInfo.vertexBindingDescriptionCount = 0;
		pipelineInfo.vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		pipelineInfo.vertexInputInfo.pVertexBindingDescriptions = nullptr;
		pipelineInfo.vertexInputInfo.flags = 0;
		pipelineInfo.vertexInputInfo.pNext = nullptr;

        // Input assembly info
		pipelineInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipelineInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		pipelineInfo.inputAssemblyInfo.flags = 0;
		pipelineInfo.inputAssemblyInfo.pNext = nullptr;

		// Rasterization info
		pipelineInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipelineInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		pipelineInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		pipelineInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		pipelineInfo.rasterizationInfo.lineWidth = 1.f;
		pipelineInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		pipelineInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		pipelineInfo.rasterizationInfo.depthBiasConstantFactor = 0.f;
		pipelineInfo.rasterizationInfo.depthBiasClamp = 0.f;
		pipelineInfo.rasterizationInfo.depthBiasSlopeFactor = 0.f;
		pipelineInfo.rasterizationInfo.flags = 0;
		pipelineInfo.rasterizationInfo.pNext = nullptr;

		// Multisample info
		pipelineInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipelineInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		pipelineInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		pipelineInfo.multisampleInfo.minSampleShading = 1.0f;
		pipelineInfo.multisampleInfo.pSampleMask = nullptr;
		pipelineInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		pipelineInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;
		pipelineInfo.multisampleInfo.flags = 0;
		pipelineInfo.multisampleInfo.pNext = nullptr;

		// Color blend attachmemt
		pipelineInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		pipelineInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		pipelineInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		pipelineInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		pipelineInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		pipelineInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		pipelineInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		pipelineInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Color blend info
		pipelineInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipelineInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		pipelineInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		pipelineInfo.colorBlendInfo.attachmentCount = 1;
		pipelineInfo.colorBlendInfo.pAttachments = &pipelineInfo.colorBlendAttachment;
		pipelineInfo.colorBlendInfo.blendConstants[0] = 0.0f;
		pipelineInfo.colorBlendInfo.blendConstants[1] = 0.0f;
		pipelineInfo.colorBlendInfo.blendConstants[2] = 0.0f;
		pipelineInfo.colorBlendInfo.blendConstants[3] = 0.0f;
		pipelineInfo.colorBlendInfo.flags = 0;
		pipelineInfo.colorBlendInfo.pNext = nullptr;

		// Depth stencil info
		pipelineInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pipelineInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		pipelineInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		pipelineInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		pipelineInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		pipelineInfo.depthStencilInfo.minDepthBounds = 0.0f;
		pipelineInfo.depthStencilInfo.maxDepthBounds = 1.0f;
		pipelineInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		pipelineInfo.depthStencilInfo.front = {};
		pipelineInfo.depthStencilInfo.back = {};
		pipelineInfo.depthStencilInfo.flags = 0;
		pipelineInfo.depthStencilInfo.pNext = nullptr;

		// Dynamic state info
		pipelineInfo.dynamicStateEnables[0] = VK_DYNAMIC_STATE_VIEWPORT;
		pipelineInfo.dynamicStateEnables[1] = VK_DYNAMIC_STATE_SCISSOR;
		pipelineInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipelineInfo.dynamicStateInfo.dynamicStateCount = 2;
		pipelineInfo.dynamicStateInfo.pDynamicStates = pipelineInfo.dynamicStateEnables;
		pipelineInfo.dynamicStateInfo.flags = 0;
		pipelineInfo.dynamicStateInfo.pNext = nullptr;
    }

    Pipeline::~Pipeline() {
        vkDeviceWaitIdle(GetDevice());

		// Delete the global buffers, if they exist
		if(pipelineInfo.globalBufferSize)
			for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
				delete globalBuffers[i];
		
		// Delete the descriptor pool
		delete descriptorPool;

		// Delete the pipeline
        vkDestroyPipeline(GetDevice(), graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(GetDevice(), pipelineLayout, nullptr);
    }

	MGE_ASSET(Pipeline)
}