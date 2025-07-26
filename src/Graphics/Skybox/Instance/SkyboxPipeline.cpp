#include "SkyboxPipeline.hpp"
#include "Core/Math/Matrix4x4.hpp"
#include "Graphics/EngineGraphics.hpp"
#include "Main/Program.hpp"
#include <math.h>
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Structs
	struct PushConstants {
		Vector4 skyboxPoints[4];
	};

	// Shader sources
	static const uint32_t VERTEX_SHADER_SOURCE[] {
#include "Graphics/Skybox/Shaders/VertShader.vert.u32"
	};
	static const uint32_t FRAGMENT_SHADER_SOURCE[] {
#include "Graphics/Skybox/Shaders/FragShader.frag.u32"
	};

	// Public functions
	VkCommandBuffer SkyboxPipeline::RecordCommands() {
		// Set the inheritence info
		VkFormat colorFormat = program->GetRenderer()->GetSwapChain()->GetSurfaceFormat().format;

		VkCommandBufferInheritanceRenderingInfoKHR inheritanceRenderingInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO_KHR,
			.pNext = nullptr,
			.flags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT_KHR,
			.viewMask = 0,
			.colorAttachmentCount = 1,
			.pColorAttachmentFormats = &colorFormat,
			.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT,
			.stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
		};
		VkCommandBufferInheritanceInfo inheritanceInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
			.pNext = &inheritanceRenderingInfo,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.framebuffer = VK_NULL_HANDLE,
			.occlusionQueryEnable = VK_FALSE,
			.queryFlags = 0,
			.pipelineStatistics = 0
		};

		// Set the command buffer begin info
		VkCommandBufferBeginInfo beginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
			.pInheritanceInfo = &inheritanceInfo
		};

		// Begin recording the command buffer
		size_t frameIndex = program->GetGraphicsSystem()->GetFrameIndex();
		VkCommandBuffer commandBuffer = commandBuffers[frameIndex];
		VulkanLoader* loader = program->GetRenderer()->GetLoader();

		VkResult result = loader->vkBeginCommandBuffer(commandBuffer, &beginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan main graphics pipeline command buffer! Error code: " + string_VkResult(result));
		
		// Bind the pipeline
		loader->vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		// Set the viewport and scissor
		VkExtent2D extent = program->GetRenderer()->GetSwapChain()->GetExtent();

		VkViewport viewport {
			.x = 0.0f,
			.y = 0.0f,
			.width = (float)extent.width,
			.height = (float)extent.height,
			.minDepth = 0.0f,
			.maxDepth = 1.0f
		};
		loader->vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor {
			.offset = { 0, 0 },
			.extent = extent
		};
		loader->vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		// Get the camera's info
		MainPipeline::CameraInfo cameraInfo = program->GetEngineGraphics()->GetMainPipeline()->GetCameraInfo();

		// Set the skybox points
		PushConstants pushConstants;
	
		switch(cameraInfo.cameraType) {
		case MainPipeline::CAMERA_TYPE_PERSPECTIVE: {
			// Calculate the vertical and horizontal dimensions
			float verticalSize = tanf(cameraInfo.perspectiveInfo.fov * 0.5f);
			float horizontalSize = verticalSize * (viewport.width / viewport.height);

			// Set the top-right corner's coordinates
			pushConstants.skyboxPoints[3] = { horizontalSize, verticalSize, -1.0f, 1.0f };

			break;
		}
		case MainPipeline::CAMERA_TYPE_ORTOGRAPHIC:
			// Set the top-right corner's coordinates
			pushConstants.skyboxPoints[3] = { 0.0f, 0.0f, -1.0f, 1.0f };

			break;
		}

		// Get all other skybox points by mirroring the top-right corner
		pushConstants.skyboxPoints[0] = { -pushConstants.skyboxPoints[3].x, -pushConstants.skyboxPoints[3].y, -1.0f, 1.0f };
		pushConstants.skyboxPoints[1] = {  pushConstants.skyboxPoints[3].x, -pushConstants.skyboxPoints[3].y, -1.0f, 1.0f };
		pushConstants.skyboxPoints[2] = { -pushConstants.skyboxPoints[3].x,  pushConstants.skyboxPoints[3].y, -1.0f, 1.0f };

		// Apply the camera's rotation vector to all three points
		Matrix4x4 cameraRotTransform = Matrix4x4::Rotation(cameraInfo.rot);
		for(size_t i = 0; i != 4; ++i)
			pushConstants.skyboxPoints[i] = pushConstants.skyboxPoints[i] * cameraRotTransform;

		// Push the skybox points and far plane to the shader
		loader->vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

		// Bind the skybox
		VkDescriptorSet descriptorSet = skybox->GetDescriptorSet();
		loader->vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

		// Draw the skybox
		loader->vkCmdDraw(commandBuffer, 6, 1, 0, 0);

		// End recording the command buffer
		result = loader->vkEndCommandBuffer(commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to end recording Vulkan main graphics pipeline command buffer! Error code: " + string_VkResult(result));
		
		return commandBuffer;
	}

	// Public functions
	SkyboxPipeline::SkyboxPipeline(EngineGraphics* engineGraphics) : GraphicsPipeline(engineGraphics->GetProgram()->GetGraphicsSystem()), program(engineGraphics->GetProgram()), skybox(nullptr) {
		// Set the command pool create info
		VulkanDevice* device = program->GetRenderer()->GetDevice();
		uint32_t graphicsFamily = device->GetDeviceQueues().graphicsIndex;

		VkCommandPoolCreateInfo commandPoolInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = graphicsFamily
		};

		// Create the command pool
		VkResult result = device->GetLoader()->vkCreateCommandPool(device->GetDevice(), &commandPoolInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &commandPool);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan skybox graphics pipeline command pool! Error code: " + string_VkResult(result));
		
		// Set the command buffer alloc info
		VkCommandBufferAllocateInfo commandBufferInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
			.commandBufferCount = (uint32_t)program->GetGraphicsSystem()->GetMaxFramesInFlight()
		};

		// Allocate the command buffers
		commandBuffers.resize(program->GetGraphicsSystem()->GetMaxFramesInFlight());
		result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &commandBufferInfo, commandBuffers.data());
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan skybox graphics pipeline secondary command buffers! Error code: " + string_VkResult(result));

		// Set the vertex shader module create info
		VkShaderModuleCreateInfo vertexShaderInfo {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = sizeof(VERTEX_SHADER_SOURCE),
			.pCode = VERTEX_SHADER_SOURCE
		};

		// Create the vertex shader module
		result = device->GetLoader()->vkCreateShaderModule(device->GetDevice(), &vertexShaderInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &vertexShader);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan vertex shader module for skybox graphics pipeline! Error code: " + string_VkResult(result));

		// Set the fragment shader module create info
		VkShaderModuleCreateInfo fragmentShaderInfo {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = sizeof(FRAGMENT_SHADER_SOURCE),
			.pCode = FRAGMENT_SHADER_SOURCE
		};

		// Create the fragment shader module
		result = device->GetLoader()->vkCreateShaderModule(device->GetDevice(), &fragmentShaderInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &fragmentShader);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fragment shader module for skybox graphics pipeline! Error code: " + string_VkResult(result));

		// Set the pipeline layout create info
		VkDescriptorSetLayout setLayout = engineGraphics->GetSkyboxManager()->GetSkyboxSetLayout();
		VkPushConstantRange pushConstantRange {
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset = 0,
			.size = sizeof(PushConstants)
		};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.setLayoutCount = 1,
			.pSetLayouts = &setLayout,
			.pushConstantRangeCount = 1,
			.pPushConstantRanges = &pushConstantRange
		};

		// Create the pipeline layout
		result = device->GetLoader()->vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &pipelineLayout);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan skybox graphics pipeline layout! Error code: " + string_VkResult(result));
		
		// Set the shader stage infos
		VkPipelineShaderStageCreateInfo shaderStageInfos[] {
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.stage = VK_SHADER_STAGE_VERTEX_BIT,
				.module = vertexShader,
				.pName = "main",
				.pSpecializationInfo = nullptr
			},
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = fragmentShader,
				.pName = "main",
				.pSpecializationInfo = nullptr
			}
		};

		// Set the vertex input info
		VkPipelineVertexInputStateCreateInfo vertexInputInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.vertexBindingDescriptionCount = 0,
			.pVertexBindingDescriptions = nullptr,
			.vertexAttributeDescriptionCount = 0,
			.pVertexAttributeDescriptions = nullptr
		};

		// Set the input assembly info
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE
		};

		// Set the viewport info
		VkPipelineViewportStateCreateInfo viewportInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.viewportCount = 1,
			.pViewports = nullptr,
			.scissorCount = 1,
			.pScissors = nullptr
		};

		// Set the rasterization info
		VkPipelineRasterizationStateCreateInfo rasterizationInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.depthClampEnable = VK_TRUE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.depthBiasEnable = VK_TRUE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f
		};

		// Set the multisample info
		VkPipelineMultisampleStateCreateInfo multisampleInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 0.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};

		// Set the depth stencil info
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.depthTestEnable = VK_TRUE,
			.depthWriteEnable = VK_TRUE,
			.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable = VK_FALSE,
			.front = VK_STENCIL_OP_KEEP,
			.back = VK_STENCIL_OP_KEEP,
			.minDepthBounds = 0.0f,
			.maxDepthBounds = 1.0f
		};

		// Set the color blend info
		VkPipelineColorBlendAttachmentState colorBlendAttachment {
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo colorBlendInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment,
			.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f } 
		};

		// Set the dynamic states
		VkDynamicState dynamaicStates[] { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.dynamicStateCount = 2,
			.pDynamicStates = dynamaicStates
		};

		// Set the pipeline rendering info
		VkFormat colorFormat = program->GetRenderer()->GetSwapChain()->GetSurfaceFormat().format;

		VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
			.pNext = nullptr,
			.viewMask = 0,
			.colorAttachmentCount = 1,
			.pColorAttachmentFormats = &colorFormat,
			.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT,
			.stencilAttachmentFormat = VK_FORMAT_UNDEFINED
		};

		// Set the graphics pipeline create info
		VkGraphicsPipelineCreateInfo pipelineInfo {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &pipelineRenderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfos,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = &inputAssemblyInfo,
			.pTessellationState = nullptr,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizationInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = &colorBlendInfo,
			.pDynamicState = &dynamicInfo,
			.layout = pipelineLayout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};

		// Create the graphics pipeline
		result = device->GetLoader()->vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &pipeline);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan skybox graphics pipeline! Error code: " + string_VkResult(result));
	}

	SkyboxPipeline::~SkyboxPipeline() {
		// Destroy the pipeline and its layout
		VulkanDevice* device = program->GetRenderer()->GetDevice();

		device->GetLoader()->vkDestroyPipeline(device->GetDevice(), pipeline, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Destroy the shader modules
		device->GetLoader()->vkDestroyShaderModule(device->GetDevice(), vertexShader, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyShaderModule(device->GetDevice(), fragmentShader, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Free all command buffers and destroy their command pool
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), commandPool, (uint32_t)commandBuffers.size(), commandBuffers.data());
		device->GetLoader()->vkDestroyCommandPool(device->GetDevice(), commandPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}