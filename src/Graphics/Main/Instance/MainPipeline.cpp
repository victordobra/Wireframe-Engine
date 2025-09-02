#include "MainPipeline.hpp"
#include "Core/Math/General/MatUtils.hpp"
#include "Graphics/Main/Components/MeshRenderer.hpp"
#include "Graphics/Main/Components/SceneLight.hpp"
#include "Graphics/EngineGraphics.hpp"
#include "Main/Program.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Constants
	const uint32_t MAX_LIGHT_COUNT = 64;

	// Structs
	struct alignas(16) SunLightInfo {
		alignas(16) Vec3f color;
		float intensity;
		alignas(16) Vec3f direction;
	};
	struct alignas(16) PointLightInfo {
		alignas(16) Vec3f color;
		float intensity;
		alignas(16) Vec3f position;
		float constantScaling;
		float linearScaling;
		float quadraticScaling;
	};
	struct alignas(16) SpotLightInfo {
		alignas(16) Vec3f color;
		float intensity;
		alignas(16) Vec3f position;
		alignas(16) Vec3f direction;
		float innerCutoff;
		float outerCutoff;
	};

	struct alignas(16) SceneInfo {
		Mat4x4f cameraTransform;
		alignas(16) Vec3f cameraPos;
		alignas(16) Vec3f cameraFwd;

		alignas(16) Vec3f ambientLightColor;

		uint32_t sunLightCount;
		uint32_t pointLightCount;
		uint32_t spotLightCount;

		SunLightInfo sunLights[MAX_LIGHT_COUNT];
		PointLightInfo pointLights[MAX_LIGHT_COUNT];
		SpotLightInfo spotLights[MAX_LIGHT_COUNT];
	};
	struct PushConstants {
		Mat4x4f objectTransform;
		Mat4x4f normalTransform;
	};

	// Shader sources
	static const uint32_t VERTEX_SHADER_SOURCE[] {
#include "Graphics/Main/Shaders/VertShader.vert.u32"
	};
	static const uint32_t FRAGMENT_SHADER_SOURCE[] {
#include "Graphics/Main/Shaders/FragShader.frag.u32"
	};

	// Internal helper functions
	void MainPipeline::CreateCommandBuffers() {
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
	}
	void MainPipeline::CreateSceneInfoBuffers() {
		// Set the scene info buffer create info
		VulkanDevice* device = program->GetRenderer()->GetDevice();
		uint32_t graphicsFamily = device->GetDeviceQueues().graphicsIndex;
		size_t maxFramesInFlight = program->GetGraphicsSystem()->GetMaxFramesInFlight();

		VkBufferCreateInfo sceneInfoBufferInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.size = sizeof(SceneInfo),
			.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &graphicsFamily
		};

		sceneInfoBuffers.resize(maxFramesInFlight);
		sceneInfoBufferMemories.resize(maxFramesInFlight);

		for(size_t i = 0; i != sceneInfoBuffers.size(); ++i) {
			// Create the scene info buffer
			VkResult result = device->GetLoader()->vkCreateBuffer(device->GetDevice(), &sceneInfoBufferInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &sceneInfoBuffers[i]);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to create Vulkan main graphics pipeline scene info buffer! Error code: " + string_VkResult(result));
			
			// Allocate the scene info buffer's memory
			try {
				sceneInfoBufferMemories[i] = device->GetAllocator()->AllocBufferMemory(sceneInfoBuffers[i], VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			} catch(const std::bad_alloc&) {
				throw std::runtime_error("Failed to allocate Vulkan main graphics pipeline scene info buffer memory!");
			}

			// Bind the buffer to its memory
			result = device->GetLoader()->vkBindBufferMemory(device->GetDevice(), sceneInfoBuffers[i], sceneInfoBufferMemories[i].memory, sceneInfoBufferMemories[i].offset);
			if(result != VK_SUCCESS)
				throw std::runtime_error((std::string)"Failed to bind Vulkan main graphics pipeline scene info buffer to its memory! Error code: " + string_VkResult(result));
		}
	}
	void MainPipeline::CreateDescriptors() {
		// Set the scene info descriptor set layout info
		VulkanDevice* device = program->GetRenderer()->GetDevice();
		size_t maxFramesInFlight = program->GetGraphicsSystem()->GetMaxFramesInFlight();

		VkDescriptorSetLayoutBinding sceneInfoDescriptorSetLayoutBinding {
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
			.pImmutableSamplers = nullptr
		};

		VkDescriptorSetLayoutCreateInfo sceneInfoDescriptorSetLayoutInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.bindingCount = 1,
			.pBindings = &sceneInfoDescriptorSetLayoutBinding
		};

		// Create the scene info descriptor set layout
		VkResult result = device->GetLoader()->vkCreateDescriptorSetLayout(device->GetDevice(), &sceneInfoDescriptorSetLayoutInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &sceneInfoDescriptorSetLayout);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan main graphics pipeline scene info descriptor set layout! Error code: " + string_VkResult(result));

		// Set the scene info descriptor pool create info
		VkDescriptorPoolSize sceneInfoDescriptorPoolSize {
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = (uint32_t)maxFramesInFlight
		};

		VkDescriptorPoolCreateInfo sceneInfoDescriptorPoolInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.maxSets = (uint32_t)maxFramesInFlight,
			.poolSizeCount = 1,
			.pPoolSizes = &sceneInfoDescriptorPoolSize
		};

		// Create the scene info descriptor pool
		result = device->GetLoader()->vkCreateDescriptorPool(device->GetDevice(), &sceneInfoDescriptorPoolInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &sceneInfoDescriptorPool);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan main graphics pipeline scene info descriptor pool! Error code: " + string_VkResult(result));
		
		// Set the scene info descriptor set alloc info
		std::vector<VkDescriptorSetLayout> setLayouts(maxFramesInFlight);
		for(size_t i = 0; i != setLayouts.size(); ++i)
			setLayouts[i] = sceneInfoDescriptorSetLayout;
		
		VkDescriptorSetAllocateInfo sceneInfoDescriptorSetInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = sceneInfoDescriptorPool,
			.descriptorSetCount = (uint32_t)maxFramesInFlight,
			.pSetLayouts = setLayouts.data()
		};

		// Allocate the scene info descriptor sets
		sceneInfoDescriptorSets.resize(maxFramesInFlight);
		result = device->GetLoader()->vkAllocateDescriptorSets(device->GetDevice(), &sceneInfoDescriptorSetInfo, sceneInfoDescriptorSets.data());
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan main graphics pipeline scene info descriptor sets! Error code: " + string_VkResult(result));
		
		// Set the scene info descriptor set writes
		std::vector<VkDescriptorBufferInfo> sceneInfoBufferInfos(maxFramesInFlight);
		for(size_t i = 0; i != sceneInfoBufferInfos.size(); ++i) {
			sceneInfoBufferInfos[i] = {
				.buffer = sceneInfoBuffers[i],
				.offset = 0,
				.range = VK_WHOLE_SIZE
			};
		}

		std::vector<VkWriteDescriptorSet> sceneInfoDescriptorSetWrites(maxFramesInFlight);
		for(size_t i = 0; i != sceneInfoDescriptorSetWrites.size(); ++i) {
			sceneInfoDescriptorSetWrites[i] = {
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = sceneInfoDescriptorSets[i],
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pImageInfo = nullptr,
				.pBufferInfo = &sceneInfoBufferInfos[i],
				.pTexelBufferView = nullptr
			};
		}

		// Update the scene info descriptor stes
		device->GetLoader()->vkUpdateDescriptorSets(device->GetDevice(), (uint32_t)sceneInfoDescriptorSetWrites.size(), sceneInfoDescriptorSetWrites.data(), 0, nullptr);
	}
	void MainPipeline::CreatePipeline(EngineGraphics* engineGraphics) {
		// Set the vertex shader module create info
		VulkanDevice* device = program->GetRenderer()->GetDevice();

		VkShaderModuleCreateInfo vertexShaderInfo {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = sizeof(VERTEX_SHADER_SOURCE),
			.pCode = VERTEX_SHADER_SOURCE
		};

		// Create the vertex shader module
		VkResult result = device->GetLoader()->vkCreateShaderModule(device->GetDevice(), &vertexShaderInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &vertexShader);
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
		VkDescriptorSetLayout setLayouts[] { sceneInfoDescriptorSetLayout, engineGraphics->GetMaterialManager()->GetMaterialSetLayout() };
		VkPushConstantRange pushConstantRange {
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset = 0,
			.size = sizeof(PushConstants)
		};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.setLayoutCount = 2,
			.pSetLayouts = setLayouts,
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
			},
			{
				.location = 3,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(RenderMesh::Vertex, tangent)
			},
			{
				.location = 4,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(RenderMesh::Vertex, bitangent)
			}
		};

		VkPipelineVertexInputStateCreateInfo vertexInputInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &vertexInputBinding,
			.vertexAttributeDescriptionCount = 5,
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
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
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

		// Get the current scene info struct
		SceneInfo* sceneInfo = (SceneInfo*)program->GetRenderer()->GetDevice()->GetAllocator()->GetMappedMemory(sceneInfoBufferMemories[frameIndex]);

		// Set the camera's info in the scene info
		Mat4x4f cameraProjection;
		float aspectRatio = (float)extent.width / extent.height;

		switch(cameraInfo.cameraType) {
		case CAMERA_TYPE_PERSPECTIVE:
			cameraProjection = Mat4x4PerspectiveProjection(cameraInfo.perspectiveInfo.fov, aspectRatio, cameraInfo.perspectiveInfo.nearPlane, cameraInfo.perspectiveInfo.farPlane);
			break;
		case CAMERA_TYPE_ORTOGRAPHIC:
			cameraProjection = Mat4x4OrtographicProjection(-cameraInfo.ortographicInfo.viewHeight * aspectRatio, cameraInfo.ortographicInfo.viewHeight * aspectRatio, -cameraInfo.ortographicInfo.viewHeight, cameraInfo.ortographicInfo.viewHeight, cameraInfo.ortographicInfo.nearPlane, cameraInfo.ortographicInfo.farPlane);
		}

		Mat4x4f cameraTransform = cameraProjection * Mat4x4Rotate(QuatConjugate(cameraInfo.rot)) * Mat4x4Translate(-cameraInfo.pos);

		sceneInfo->cameraTransform = MatTranspose(cameraTransform);
		sceneInfo->cameraPos = { cameraInfo.pos.x, cameraInfo.pos.y, cameraInfo.pos.z };
		sceneInfo->cameraFwd = (Vec3f)(Mat4x4Rotate(cameraInfo.rot) * Vec4f{ 0.0f, 0.0f, -1.0f, 1.0f });

		sceneInfo->ambientLightColor = { ambientLightColor.x, ambientLightColor.y, ambientLightColor.z };

		// Reset the light counters
		sceneInfo->sunLightCount = 0;
		sceneInfo->pointLightCount = 0;

		// Loop through all lights
		size_t sceneLightTypeIndex = program->GetEntityManager()->GetTypeIndex<SceneLight>();
		EntityManager::Signature sceneLightTypeSignature = 0;
		sceneLightTypeSignature.set(sceneLightTypeIndex, 1);

		for(Entity entity = program->GetEntityManager()->GetNextEntity(EntityManager::INVALID_ENTITY, sceneLightTypeSignature); entity != EntityManager::INVALID_ENTITY; entity = program->GetEntityManager()->GetNextEntity(entity, sceneLightTypeSignature)) {
			// Get the transform and the light component
			Transform transform = program->GetEntityManager()->GetEntityTransform(entity);
			SceneLight sceneLight = *(SceneLight*)(program->GetEntityManager()->GetComponentList(sceneLightTypeIndex)->GetComponent(entity));

			// Add the light to the scene info
			switch(sceneLight.lightType) {
			case SceneLight::LIGHT_TYPE_SUN:
				// Check if the max light count was already reached
				if(sceneInfo->sunLightCount == MAX_LIGHT_COUNT)
					throw std::length_error("Exceeded maximum sun light count!");
				
				// Add the current sun light to the scene info
				sceneInfo->sunLights[sceneInfo->sunLightCount++] = {
					.color = { sceneLight.sunLightInfo.lightColor.x, sceneLight.sunLightInfo.lightColor.y, sceneLight.sunLightInfo.lightColor.z },
					.intensity = sceneLight.sunLightInfo.lightIntensity,
					.direction = (Vec3f)(Mat4x4Rotate(transform.rot) * Vec4f{ 0.0f, 0.0f, -1.0f, 1.0f })
				};

				break;
			case SceneLight::LIGHT_TYPE_POINT:
				// Check if the max light count was already reached
				if(sceneInfo->pointLightCount == MAX_LIGHT_COUNT)
					throw std::length_error("Exceeded maximum point light count!");
				
				// Add the current point light to the scene info
				sceneInfo->pointLights[sceneInfo->pointLightCount++] = {
					.color = { sceneLight.pointLightInfo.lightColor.x, sceneLight.pointLightInfo.lightColor.y, sceneLight.pointLightInfo.lightColor.z },
					.intensity = sceneLight.pointLightInfo.lightIntensity,
					.position = { transform.pos.x, transform.pos.y, transform.pos.z },
					.constantScaling = sceneLight.pointLightInfo.constantScaling,
					.linearScaling = sceneLight.pointLightInfo.linearScaling,
					.quadraticScaling = sceneLight.pointLightInfo.quadraticScaling
				};

				break;
			case SceneLight::LIGHT_TYPE_SPOT:
				// Check if the max light count was already reached
				if(sceneInfo->spotLightCount == MAX_LIGHT_COUNT)
					throw std::length_error("Exceeded maximum spot light count!");
				
				// Add the current spot light to the scene info
				sceneInfo->spotLights[sceneInfo->spotLightCount++] = {
					.color = { sceneLight.spotLightInfo.lightColor.x, sceneLight.spotLightInfo.lightColor.y, sceneLight.spotLightInfo.lightColor.z },
					.intensity = sceneLight.spotLightInfo.lightIntensity,
					.position = { transform.pos.x, transform.pos.y, transform.pos.z },
					.direction = (Vec3f)(Mat4x4Rotate(transform.rot) * Vec4f{ 0.0f, 0.0f, -1.0f, 1.0f }),
					.innerCutoff = Cos(sceneLight.spotLightInfo.innerCutoff),
					.outerCutoff = Cos(sceneLight.spotLightInfo.outerCutoff)
				};

				break;
			}
		}

		// Loop through all renderers
		size_t meshRendererTypeIndex = program->GetEntityManager()->GetTypeIndex<MeshRenderer>();
		EntityManager::Signature meshRendererTypeSignature = 0;
		meshRendererTypeSignature.set(meshRendererTypeIndex, 1);

		for(Entity entity = program->GetEntityManager()->GetNextEntity(EntityManager::INVALID_ENTITY, meshRendererTypeSignature); entity != EntityManager::INVALID_ENTITY; entity = program->GetEntityManager()->GetNextEntity(entity, meshRendererTypeSignature)) {
			// Set the push constants
			PushConstants pushConstants {
				.objectTransform = MatTranspose(program->GetEntityManager()->GetGlobalTransform(entity)),
				.normalTransform = MatTranspose(program->GetEntityManager()->GetGlobalNormalTransform(entity))
			};

			// Get the renderer component
			MeshRenderer meshRenderer = *(MeshRenderer*)(program->GetEntityManager()->GetComponentList(meshRendererTypeIndex)->GetComponent(entity));

			// Bind the descriptor sets and set the push constants
			VkDescriptorSet descriptorSets[] { sceneInfoDescriptorSets[frameIndex], meshRenderer.material->GetDescriptorSet() };

			loader->vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 2, descriptorSets, 0, nullptr);
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
	MainPipeline::MainPipeline(EngineGraphics* engineGraphics, const CameraInfo& cameraInfo) : GraphicsPipeline(engineGraphics->GetProgram()->GetGraphicsSystem()), program(engineGraphics->GetProgram()), cameraInfo(cameraInfo) {
		// Create the pipeline's components
		CreateCommandBuffers();
		CreateSceneInfoBuffers();
		CreateDescriptors();
		CreatePipeline(engineGraphics);
	}

	MainPipeline::~MainPipeline() {
		// Destroy the pipeline and its layout
		VulkanDevice* device = program->GetRenderer()->GetDevice();

		device->GetLoader()->vkDestroyPipeline(device->GetDevice(), pipeline, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Destroy the shader modules
		device->GetLoader()->vkDestroyShaderModule(device->GetDevice(), vertexShader, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyShaderModule(device->GetDevice(), fragmentShader, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Destroy the scene info descriptor pool and the scene info descriptor set layout
		device->GetLoader()->vkDestroyDescriptorPool(device->GetDevice(), sceneInfoDescriptorPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
		device->GetLoader()->vkDestroyDescriptorSetLayout(device->GetDevice(), sceneInfoDescriptorSetLayout, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Destroy the scene info buffers
		for(size_t i = 0; i != sceneInfoBuffers.size(); ++i)
			device->GetLoader()->vkDestroyBuffer(device->GetDevice(), sceneInfoBuffers[i], &VulkanRenderer::ALLOCATION_CALLBACKS);
		for(size_t i = 0; i != sceneInfoBufferMemories.size(); ++i)
			device->GetAllocator()->FreeMemory(sceneInfoBufferMemories[i]);

		// Free all command buffers and destroy their command pool
		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), commandPool, (uint32_t)commandBuffers.size(), commandBuffers.data());
		device->GetLoader()->vkDestroyCommandPool(device->GetDevice(), commandPool, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}
}