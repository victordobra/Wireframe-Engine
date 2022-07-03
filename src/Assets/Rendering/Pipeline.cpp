#include "Pipeline.hpp"
#include "Material.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Sampler.hpp"

namespace mge {
	// Internal helper functions
	void Pipeline::CreateDescriptorPool() {
		if(pipelineInfo.globalBufferSize) {
			// The descriptor pool info isn't set, create it
			struct ShaderCount {
				Shader* shader;
				VkShaderStageFlags shaderStageFlags;
				size_t count;
				size_t imageCount;
			};
			vector<ShaderCount> shaderCounts;
			
			// Extract every shader and how many times it appears
			for(auto& shaderStage : pipelineInfo.shaderStages) {
				size_t materialCount = shaderStage.shader->materials.size();
				if(!materialCount) {
					materialCount = 0;
					for(const auto& property : shaderStage.shader->properties)
						if(property.type != Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE) {
							materialCount = 1;
							break;
						}
				}
				shaderCounts.push_back({shaderStage.shader, (VkShaderStageFlags)shaderStage.shaderStage, materialCount, 0});
			}
			
			// Find the number of image properties
			size_t imageCount = 0;
			for(auto& shaderCount : shaderCounts)
				for(const auto& property : shaderCount.shader->properties)
					if(property.type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE) {
						imageCount += shaderCount.count;
						++shaderCount.imageCount;
					}
			
			// Find the total number of materials
			size_t materialCount = 0;
			for(const auto& shaderCount : shaderCounts)
				materialCount += shaderCount.count;
			
			// Set the descriptor pool sizes
			auto& poolSizes = pipelineInfo.descriptorPoolInfo.descriptorPoolSizes;

			poolSizes.push_back({ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, (uint32_t)(materialCount * MAX_FRAMES_IN_FLIGHT /*<- Material buffers*/ + MAX_FRAMES_IN_FLIGHT /*<- Global buffer*/) });
			if(imageCount)
				poolSizes.push_back({ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)(imageCount * MAX_FRAMES_IN_FLIGHT) });
			
			// Create the descriptor set layouts
			auto& setLayouts = pipelineInfo.descriptorPoolInfo.descriptorSetLayouts;
			setLayouts.clear();
			using SetLayout = DescriptorPool::DescriptorPoolInfo::DescriptorSetLayout;

			// Global buffer layout
			SetLayout globalBufferLayout;
			
			VkDescriptorSetLayoutBinding globalBufferBinding;
			
			globalBufferBinding.binding = 0;
			globalBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			globalBufferBinding.descriptorCount = 1;
			globalBufferBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
			globalBufferBinding.pImmutableSamplers = nullptr;

			globalBufferLayout.bindings.push_back(globalBufferBinding);
			setLayouts.push_back(globalBufferLayout);

			for(auto& shaderCount : shaderCounts) {
				if(!shaderCount.count)
					continue;
				
				SetLayout materialLayout;

				VkDescriptorSetLayoutBinding materialBufferBinding;

				materialBufferBinding.binding = 0;
				materialBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				materialBufferBinding.descriptorCount = 1;
				materialBufferBinding.stageFlags = shaderCount.shaderStageFlags;
				materialBufferBinding.pImmutableSamplers = nullptr;

				materialLayout.bindings.push_back(materialBufferBinding);

				for(size_t i = 0; i < shaderCount.imageCount; ++i) {
					VkDescriptorSetLayoutBinding materialImageBinding;

					materialImageBinding.binding = i + 1;
					materialImageBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					materialImageBinding.descriptorCount = 1;
					materialImageBinding.stageFlags = shaderCount.shaderStageFlags;
					materialImageBinding.pImmutableSamplers = nullptr;

					materialLayout.bindings.push_back(materialImageBinding);
				}

				setLayouts.push_back(materialLayout);
			}
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

			// Write the material buffers and images
			size_t descriptorIndex = MAX_FRAMES_IN_FLIGHT;
			vector<Material*> materials;

			for(auto& shaderStage : pipelineInfo.shaderStages)
				for(auto* material : shaderStage.shader->materials){
					materials.push_back(material);
				}

			for(size_t i = 0; i < materials.size(); ++i)
				for(size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; ++j) {
					// Get the descriptor infos
					VkDescriptorBufferInfo bufferInfo = materials[i]->GetBuffers()[j]->GetDescriptorInfo();

					vector<VkDescriptorImageInfo> imageInfos(materials[i]->GetImageVectors()[j].size());

					for(size_t k = 0; k < imageInfos.size(); ++k) {
						imageInfos[k].sampler = GetSampler();
						imageInfos[k].imageView = materials[i]->GetImageVectors()[j][k]->GetImageView();
						imageInfos[k].imageLayout = materials[i]->GetImageVectors()[j][k]->GetImageLayout();
					}

					// Set information about the writes
					vector<VkWriteDescriptorSet> writes(1 + imageInfos.size());

					writes[0] = {};

					writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					writes[0].dstBinding = 0;
					writes[0].pBufferInfo = &bufferInfo;
					writes[0].descriptorCount = 1;
					writes[0].dstSet = descriptorPool->GetDescriptorSets()[descriptorIndex];

					for(size_t k = 1; k < writes.size(); ++k) {
						writes[k] = {};

						writes[k].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						writes[k].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						writes[k].dstBinding = k;
						writes[k].pImageInfo = imageInfos.begin() + k - 1;
						writes[k].descriptorCount = 1;
						writes[k].dstSet = descriptorPool->GetDescriptorSets()[descriptorIndex];
					}
					++descriptorIndex;

					vkUpdateDescriptorSets(GetDevice(), (uint32_t)writes.size(), writes.data(), 0, nullptr);
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
		auto result = vkCreatePipelineLayout(GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout);
		if(result != VK_SUCCESS)
			console::OutFatalError((string)"Failed to create pipeline layout! Error code: " + VkResultToString(result), 1);
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
		auto result = vkCreateGraphicsPipelines(GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
		if (result != VK_SUCCESS)
			console::OutFatalError((string)"Failed to create graphics pipeline! Error code: " + VkResultToString(result), 1);
	}

	// External functions
    Pipeline::Pipeline(const PipelineInfo& info) {
		pipelineInfo = info;

		Create();
    }

	void Pipeline::LoadFromFile(const string& filePath) {
		FileInput input(filePath, StreamType::BINARY);

		// Load all of the shader stages
		uint64_t shaderStageCount{};
		input.ReadBuffer((char_t*)&shaderStageCount, sizeof(uint64_t));

		pipelineInfo.shaderStages.resize(shaderStageCount);
		
		for(uint64_t i = 0; i < shaderStageCount; i++) {
			string location{};

			uint64_t stringLength{};
			input.ReadBuffer((char_t*)&stringLength, sizeof(uint64_t));
			location.resize(stringLength);

			input.ReadBuffer((char_t*)location.c_str(), stringLength * sizeof(char_t));
			location[stringLength] = 0;

			Shader* shader = Asset::GetOrCreateAssetWithLocation<Shader>(location);
			shader->pipeline = this;
			pipelineInfo.shaderStages[i].shader = shader;
			input.ReadBuffer((char_t*)&pipelineInfo.shaderStages[i].shaderStage, sizeof(VkShaderStageFlagBits));
		}

		// Load the push constant ranges
		uint64_t pushConstantRangeCount{};
		input.ReadBuffer((char_t*)&pushConstantRangeCount, sizeof(uint64_t));
		pipelineInfo.pushConstantRanges.resize((size_t)pushConstantRangeCount);
		LoadVkPushConstantRange(input, pipelineInfo.pushConstantRanges.data(), pushConstantRangeCount);

		// Load the descriptor pool sizes
		uint64_t descriptorPoolSizeCount{};
		input.ReadBuffer((char_t*)&descriptorPoolSizeCount, sizeof(uint64_t));
		pipelineInfo.descriptorPoolInfo.descriptorPoolSizes.resize(descriptorPoolSizeCount);
		LoadVkDescriptorPoolSize(input, pipelineInfo.descriptorPoolInfo.descriptorPoolSizes.data(), descriptorPoolSizeCount);

		// Load the descriptor set layouts
		uint64_t descriptorSetLayoutCount{};
		input.ReadBuffer((char_t*)&descriptorSetLayoutCount, sizeof(uint64_t));
		pipelineInfo.descriptorPoolInfo.descriptorSetLayouts.resize(descriptorSetLayoutCount);

		for(uint64_t i = 0; i < descriptorSetLayoutCount; ++i) {
			uint64_t bindingCount{};
			input.ReadBuffer((char_t*)&bindingCount, sizeof(uint64_t));
			pipelineInfo.descriptorPoolInfo.descriptorSetLayouts[i].bindings.resize(bindingCount);
			LoadVkDescriptorSetLayoutBinding(input, pipelineInfo.descriptorPoolInfo.descriptorSetLayouts[i].bindings.data(), bindingCount);
		}

		// Load the descriptor pool create flags and the global buffer size
		input.ReadBuffer((char_t*)&pipelineInfo.descriptorPoolInfo.poolCreateFlags, sizeof(VkDescriptorPoolCreateFlags));
		input.ReadBuffer((char_t*)&pipelineInfo.globalBufferSize, sizeof(VkDeviceSize));

		// Load the vertex bindings
		uint64_t vertexBindingCount{};
		input.ReadBuffer((char_t*)&vertexBindingCount, sizeof(uint64_t));
		pipelineInfo.vertexBindings.resize(vertexBindingCount);
		LoadVkVertexInputBindingDescription(input, pipelineInfo.vertexBindings.data(), vertexBindingCount);

		// Load the vertex attributes
		uint64_t vertexAttributeCount{};
		input.ReadBuffer((char_t*)&vertexAttributeCount, sizeof(uint64_t));
		pipelineInfo.vertexAttributes.resize(vertexAttributeCount);
		LoadVkVertexInputAttributeDescription(input, pipelineInfo.vertexAttributes.data(), vertexAttributeCount);

		// Load the dynamic states
		uint64_t dynamicStateCount{};
		input.ReadBuffer((char_t*)&dynamicStateCount, sizeof(uint64_t));
		pipelineInfo.dynamicStateEnables.resize(dynamicStateCount);
		input.ReadBuffer((char_t*)pipelineInfo.dynamicStateEnables.data(), dynamicStateCount * sizeof(VkDynamicState));

		// Load the viewport and scissor
		LoadVkViewport(input, &pipelineInfo.viewport, 1);
		LoadVkRect2D(input, &pipelineInfo.scissor, 1);

		// Load the rest of the pipeline info
		LoadVkPipelineViewportStateCreateInfo(input, &pipelineInfo.viewportInfo, 1);
		LoadVkPipelineVertexInputStateCreateInfo(input, &pipelineInfo.vertexInputInfo, 1);
		LoadVkPipelineInputAssemblyStateCreateInfo(input, &pipelineInfo.inputAssemblyInfo, 1);
		LoadVkPipelineRasterizationStateCreateInfo(input, &pipelineInfo.rasterizationInfo, 1);
		LoadVkPipelineMultisampleStateCreateInfo(input, &pipelineInfo.multisampleInfo, 1);
		LoadVkPipelineColorBlendAttachmentState(input, &pipelineInfo.colorBlendAttachment, 1);
		LoadVkPipelineColorBlendStateCreateInfo(input, &pipelineInfo.colorBlendInfo, 1);
		LoadVkPipelineDepthStencilStateCreateInfo(input, &pipelineInfo.depthStencilInfo, 1);
		LoadVkPipelineDynamicStateCreateInfo(input, &pipelineInfo.dynamicStateInfo, 1);
		input.ReadBuffer((char_t*)&pipelineInfo.subpass, sizeof(uint32_t));

		input.Close();

		// Set some values
		pipelineInfo.dynamicStateEnables.clear();

		pipelineInfo.viewportInfo.viewportCount = 1;
		pipelineInfo.viewportInfo.pViewports = &pipelineInfo.viewport;
		pipelineInfo.viewportInfo.scissorCount = 1;
		pipelineInfo.viewportInfo.pScissors = &pipelineInfo.scissor;

		pipelineInfo.colorBlendInfo.attachmentCount = 1;
		pipelineInfo.colorBlendInfo.pAttachments = &pipelineInfo.colorBlendAttachment;

		pipelineInfo.dynamicStateInfo.dynamicStateCount = pipelineInfo.dynamicStateEnables.size();
		pipelineInfo.dynamicStateInfo.pDynamicStates = pipelineInfo.dynamicStateEnables.data();

		// Create the pipeline
		Create();
	}
	void Pipeline::SaveToFile(const string& filePath) {
		FileOutput output(filePath, StreamType::BINARY);

		// Save all of the shader stages
		uint64_t shaderStageCount = (uint64_t)pipelineInfo.shaderStages.size();
		output.WriteBuffer((char_t*)&shaderStageCount, sizeof(uint64_t));

		for(size_t i = 0; i < shaderStageCount; i++) {
			auto str = pipelineInfo.shaderStages[i].shader->location;
			uint64_t stringLength = (uint64_t)str.length();

			output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
			output.WriteBuffer((char_t*)str.c_str(), stringLength * sizeof(char_t));
			output.WriteBuffer((char_t*)&pipelineInfo.shaderStages[i].shaderStage, sizeof(VkShaderStageFlagBits));
		}

		// Save the push constant ranges
		uint64_t pushConstantRangeCount = (uint64_t)pipelineInfo.pushConstantRanges.size();
		output.WriteBuffer((char_t*)&pushConstantRangeCount, sizeof(uint64_t));
		SaveVkPushConstantRange(output, pipelineInfo.pushConstantRanges.data(), pushConstantRangeCount);

		// Save the descriptor pool sizes
		uint64_t descriptorPoolSizeCount = (uint64_t)pipelineInfo.descriptorPoolInfo.descriptorPoolSizes.size();
		output.WriteBuffer((char_t*)&descriptorPoolSizeCount, sizeof(uint64_t));
		SaveVkDescriptorPoolSize(output, pipelineInfo.descriptorPoolInfo.descriptorPoolSizes.data(), descriptorPoolSizeCount);

		// Save the descriptor set layouts
		uint64_t descriptorSetLayoutCount = (uint64_t)pipelineInfo.descriptorPoolInfo.descriptorSetLayouts.size();
		output.WriteBuffer((char_t*)&descriptorSetLayoutCount, sizeof(uint64_t));

		for(const auto& descriptorSetLayout : pipelineInfo.descriptorPoolInfo.descriptorSetLayouts) {
			uint64_t bindingCount = (uint64_t)descriptorSetLayout.bindings.size();
			output.WriteBuffer((char_t*)&bindingCount, sizeof(uint64_t));
			SaveVkDescriptorSetLayoutBinding(output, descriptorSetLayout.bindings.data(), bindingCount);
		}

		// Save the descriptor pool create flags and the global buffer size
		output.WriteBuffer((char_t*)&pipelineInfo.descriptorPoolInfo.poolCreateFlags, sizeof(VkDescriptorPoolCreateFlags));
		output.WriteBuffer((char_t*)&pipelineInfo.globalBufferSize, sizeof(VkDeviceSize));

		// Save the vertex bindings
		uint64_t vertexBindingCount = (uint64_t)pipelineInfo.vertexBindings.size();
		output.WriteBuffer((char_t*)&vertexBindingCount, sizeof(uint64_t));
		SaveVkVertexInputBindingDescription(output, pipelineInfo.vertexBindings.data(), vertexBindingCount);

		// Save the vertex attributes
		uint64_t vertexAttributeCount = (uint64_t)pipelineInfo.vertexAttributes.size();
		output.WriteBuffer((char_t*)&vertexAttributeCount, sizeof(uint64_t));
		SaveVkVertexInputAttributeDescription(output, pipelineInfo.vertexAttributes.data(), vertexAttributeCount);

		// Save the dynamic state enables
		uint64_t dynamicStateCount = pipelineInfo.dynamicStateEnables.size();
		output.WriteBuffer((char_t*)&dynamicStateCount, sizeof(uint64_t));
		output.WriteBuffer((char_t*)pipelineInfo.dynamicStateEnables.data(), dynamicStateCount * sizeof(VkDynamicState));

		// Save the viewport and the scissor
		SaveVkViewport(output, &pipelineInfo.viewport, 1);
		SaveVkRect2D(output, &pipelineInfo.scissor, 1);

		// Save the rest of the pipeline info
		SaveVkPipelineViewportStateCreateInfo(output, &pipelineInfo.viewportInfo, 1);
		SaveVkPipelineVertexInputStateCreateInfo(output, &pipelineInfo.vertexInputInfo, 1);
		SaveVkPipelineInputAssemblyStateCreateInfo(output, &pipelineInfo.inputAssemblyInfo, 1);
		SaveVkPipelineRasterizationStateCreateInfo(output, &pipelineInfo.rasterizationInfo, 1);
		SaveVkPipelineMultisampleStateCreateInfo(output, &pipelineInfo.multisampleInfo, 1);
		SaveVkPipelineColorBlendAttachmentState(output, &pipelineInfo.colorBlendAttachment, 1);
		SaveVkPipelineColorBlendStateCreateInfo(output, &pipelineInfo.colorBlendInfo, 1);
		SaveVkPipelineDepthStencilStateCreateInfo(output, &pipelineInfo.depthStencilInfo, 1);
		SaveVkPipelineDynamicStateCreateInfo(output, &pipelineInfo.dynamicStateInfo, 1);
		output.WriteBuffer((char_t*)&pipelineInfo.subpass, sizeof(uint32_t));

		output.Close();
	}

    void Pipeline::PopulatePipelineInfo(PipelineInfo& pipelineInfo) {
		// Viewport and scissor
		pipelineInfo.viewport = { 0, 0, (float32_t)GetSwapChainExtent().width, (float32_t)GetSwapChainExtent().height, 0.f, 1.f };
		pipelineInfo.scissor = { { 0, 0 }, GetSwapChainExtent() };

		// Viewport info
		pipelineInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipelineInfo.viewportInfo.viewportCount = 1;
		pipelineInfo.viewportInfo.pViewports = &pipelineInfo.viewport;
		pipelineInfo.viewportInfo.scissorCount = 1;
		pipelineInfo.viewportInfo.pScissors = &pipelineInfo.scissor;
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
		pipelineInfo.dynamicStateEnables.clear();

		pipelineInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipelineInfo.dynamicStateInfo.dynamicStateCount = pipelineInfo.dynamicStateEnables.size();
		pipelineInfo.dynamicStateInfo.pDynamicStates = pipelineInfo.dynamicStateEnables.data();
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