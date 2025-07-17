#include "MainPipeline.hpp"
#include "Core/Math/Matrix4x4.hpp"
#include "Graphics/Main/Components/MeshRenderer.hpp"
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Structs
	struct PushConstants {
		Matrix4x4 objectTransform;
		Matrix4x4 cameraTransform;
	};

	// Shader sources
	static const uint32_t VERTEX_SHADER_SOURCE[] {
#include "Graphics/Main/Shaders/VertShader.vert.u32"
	};
	static const uint32_t FRAGMENT_SHADER_SOURCE[] {
#include "Graphics/Main/Shaders/FragShader.frag.u32"
	};

	// Record commands function
	VkCommandBuffer MainPipeline::RecordCommands() {
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
		VkCommandBuffer commandBuffer = commandBuffers[program->GetGraphicsSystem()->GetFrameIndex()];
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

		// Calculate the camera's transform matrix		
		Matrix4x4 cameraProjection;
		float aspectRatio = (float)extent.width / extent.height;

		switch(cameraInfo.cameraType) {
		case CAMERA_TYPE_PERSPECTIVE:
			cameraProjection = Matrix4x4::PerspectiveProjection(cameraInfo.perspectiveInfo.fov, aspectRatio, cameraInfo.perspectiveInfo.nearPlane, cameraInfo.perspectiveInfo.farPlane);
			break;
		case CAMERA_TYPE_ORTOGRAPHIC:
			cameraProjection = Matrix4x4::OrtographicProjection(-cameraInfo.ortographicInfo.viewHeight * aspectRatio, cameraInfo.ortographicInfo.viewHeight * aspectRatio, -cameraInfo.ortographicInfo.viewHeight, cameraInfo.ortographicInfo.viewHeight, cameraInfo.ortographicInfo.nearPlane, cameraInfo.ortographicInfo.farPlane);
		}

		Matrix4x4 cameraTransform = Matrix4x4::Translation(-cameraInfo.pos) * Matrix4x4::Rotation(cameraInfo.rot.Inverted()) * cameraProjection;

		// Loop through all renderers
		size_t meshRendererTypeIndex = program->GetEntityManager()->GetTypeIndex<MeshRenderer>();
		EntityManager::Signature meshRendererTypeSignature = 0;
		meshRendererTypeSignature.set(meshRendererTypeIndex, 1);

		for(Entity entity = program->GetEntityManager()->GetNextEntity(EntityManager::INVALID_ENTITY, meshRendererTypeSignature); entity != EntityManager::INVALID_ENTITY; entity = program->GetEntityManager()->GetNextEntity(entity, meshRendererTypeSignature)) {
			// Get the transform and the renderer component
			Transform transform = program->GetEntityManager()->GetEntityTransform(entity);
			MeshRenderer meshRenderer = *(MeshRenderer*)(program->GetEntityManager()->GetComponentList(meshRendererTypeIndex)->GetComponent(entity));

			// Calculate the object's transformation matrix
			Matrix4x4 objectTransform = Matrix4x4::Transform(transform.pos, transform.rot, transform.scale);

			// Set the push constants
			PushConstants pushConstants {
				.objectTransform = objectTransform.Transposed(),
				.cameraTransform = cameraTransform.Transposed()
			};

			// Bind the descriptor sets and set the push constants
			VkDescriptorSet descriptorSet = meshRenderer.material->GetDescriptorSet();

			loader->vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
			loader->vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

			// Bind the vertex and index buffers
			VkBuffer vertexBuffer = meshRenderer.mesh->GetVertexBuffer();
			VkDeviceSize offset = 0;

			loader->vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);
			loader->vkCmdBindIndexBuffer(commandBuffer, meshRenderer.mesh->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

			// Draw the mesh
			loader->vkCmdDrawIndexed(commandBuffer, (uint32_t)meshRenderer.mesh->GetIndexCount(), 1, 0, 0, 0);
		}

		// End recording the command buffer
		result = loader->vkEndCommandBuffer(commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to end recording Vulkan main graphics pipeline command buffer! Error code: " + string_VkResult(result));
		
		return commandBuffer;
	}

	// Public functions
	MainPipeline::MainPipeline(Program* program, const CameraInfo& cameraInfo) : GraphicsPipeline(program->GetGraphicsSystem()), program(program), cameraInfo(cameraInfo) {
		// Set the command pool create info
		VulkanDevice* device = program->GetRenderer()->GetDevice();

		VkCommandPoolCreateInfo commandPoolInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = device->GetDeviceQueues().graphicsIndex
		};

		// Create the command pool
		VkResult result = device->GetLoader()->vkCreateCommandPool(device->GetDevice(), &commandPoolInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &commandPool);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan main graphics pipeline command pool! Error code: " + string_VkResult(result));
		
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
			throw std::runtime_error((std::string)"Failed to allocate Vulkan main graphics pipeline secondary command buffers! Error code: " + string_VkResult(result));
		
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
			throw std::runtime_error((std::string)"Failed to create Vulkan vertex shader module for main graphics pipeline! Error code: " + string_VkResult(result));

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
			throw std::runtime_error((std::string)"Failed to create Vulkan fragment shader module for main graphics pipeline! Error code: " + string_VkResult(result));

		// Set the pipeline layout create info
		VkDescriptorSetLayout setLayout = program->GetMaterialManager()->GetMaterialSetLayout();
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
			throw std::runtime_error((std::string)"Failed to create Vulkan main graphics pipeline layout! Error code: " + string_VkResult(result));
		
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
		VkVertexInputBindingDescription vertexInputBinding {
			.binding = 0,
			.stride = sizeof(RenderMesh::Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		};
		VkVertexInputAttributeDescription vertexInputAttributes[] {
			{
				.location = 0,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(RenderMesh::Vertex, position)
			},
			{
				.location = 1,
				.binding = 0,
				.format = VK_FORMAT_R32G32_SFLOAT,
				.offset = offsetof(RenderMesh::Vertex, uvCoord)
			},
			{
				.location = 2,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(RenderMesh::Vertex, normal)
			}
		};

		VkPipelineVertexInputStateCreateInfo vertexInputInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &vertexInputBinding,
			.vertexAttributeDescriptionCount = 3,
			.pVertexAttributeDescriptions = vertexInputAttributes
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
			.cullMode = VK_CULL_MODE_FRONT_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
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
			.depthCompareOp = VK_COMPARE_OP_LESS,
			.depthBoundsTestEnable = VK_TRUE,
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
			throw std::runtime_error((std::string)"Failed to create Vulkan main graphics pipeline! Error code: " + string_VkResult(result));
	}

	MainPipeline::~MainPipeline() {
		// Destroy the pipeline anad its layout
		VulkanDevice* device = program->GetRenderer()->GetDevice();

		device->GetLoader()->vkDestroyPipeline(device->GetDevice(), pipeline, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Destroy the shader modules
		device->GetLoader()->vkDestroyShaderModule(device->GetDevice(), vertexShader, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyShaderModule(device->GetDevice(), fragmentShader, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Free all command buffers and free their command pool
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), commandPool, (uint32_t)commandBuffers.size(), commandBuffers.data());
		device->GetLoader()->vkDestroyCommandPool(device->GetDevice(), commandPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}