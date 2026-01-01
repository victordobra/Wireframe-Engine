#include "RenderMesh.hpp"
#include <cstring>
#include <stdexcept>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Static record callbacks
	struct TransferUserData {
		VulkanBuffer* srcBuffer;
		VulkanBuffer* dstBuffer;
	};

	static void RecordBufferCopyCallback(void* userData, VkCommandBuffer commandBuffer) {
		// Get the buffers
		TransferUserData* transferData = (TransferUserData*)userData;
		VulkanBuffer* srcBuffer = transferData->srcBuffer;
		VulkanBuffer* dstBuffer = transferData->dstBuffer;

		// Set the buffer copy info
		VkBufferCopy bufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = srcBuffer->GetBufferSize()
		};

		// Record the transfer
		srcBuffer->GetDevice()->GetLoader()->vkCmdCopyBuffer(commandBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &bufferCopy);
	}

	// Public functions
	RenderMesh::RenderMesh(VulkanRenderer* renderer, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) : renderer(renderer), vertexCount(vertices.size()), indexCount(indices.size()) {
		// Create the vertex buffer
		VulkanDevice* device = renderer->GetDevice();

		vertexBuffer = new VulkanBuffer(
			device,
			(VkDeviceSize)sizeof(Vertex) * vertices.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_SHARING_MODE_CONCURRENT,
			VulkanDevice::QUEUE_TYPE_GRAPHICS | VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		// Create the index buffer
		indexBuffer = new VulkanBuffer(
			device,
			(VkDeviceSize)sizeof(uint32_t) * indices.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_SHARING_MODE_CONCURRENT,
			VulkanDevice::QUEUE_TYPE_GRAPHICS | VulkanDevice::QUEUE_TYPE_TRANSFER,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		// Create the vertex staging buffer
		VulkanBuffer vertexStagingBuffer {
			device,
			(VkDeviceSize)sizeof(Vertex) * vertices.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Set the vertex staging buffer's info
		std::memcpy(device->GetAllocator()->GetMappedMemory(vertexStagingBuffer.GetBufferMemory()), vertices.data(), sizeof(Vertex) * vertices.size());

		// Create the index staging buffer
		VulkanBuffer indexStagingBuffer {
			device,
			(VkDeviceSize)sizeof(uint32_t) * indices.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Set the index staging buffer's info
		std::memcpy(device->GetAllocator()->GetMappedMemory(indexStagingBuffer.GetBufferMemory()), indices.data(), sizeof(uint32_t) * indices.size());

		// Set the vertex transfer command stage info
		TransferUserData vertexTransferData {
			.srcBuffer = &vertexStagingBuffer,
			.dstBuffer = vertexBuffer
		};

		VulkanCommand::CommandStageInfo vertexTransferStageInfo {
			.name = "VertexCopyTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = &vertexStagingBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = vertexBuffer
					}
				}
			},
			.recordCallback = RecordBufferCopyCallback,
			.userData = &vertexTransferData
		};
		
		// Set the index transfer command stage info
		TransferUserData indexTransferData {
			.srcBuffer = &indexStagingBuffer,
			.dstBuffer = indexBuffer
		};

		VulkanCommand::CommandStageInfo indexTransferStageInfo {
			.name = "IndexCopyTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = &indexStagingBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = indexBuffer
					}
				}
			},
			.recordCallback = RecordBufferCopyCallback,
			.userData = &indexTransferData
		};

		// Create and record the command
		VulkanCommand transferCommand { device, VulkanCommand::COMMAND_TYPE_TRANSFER };

		transferCommand.AddCommandStage(std::move(vertexTransferStageInfo));
		transferCommand.AddCommandStage(std::move(indexTransferStageInfo));
		transferCommand.RecordCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		// Set the transfer fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the transfer fence
		VkFence transferFence;
		VkResult result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &transferFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for mesh vertex and index transfer! Error code: " + string_VkResult(result));

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = transferCommand.GetCommandBuffer(),
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		device->GetDeviceQueues().transferQueueMutex.lock();
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, transferFence);
		device->GetDeviceQueues().transferQueueMutex.unlock();
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for mesh vertex and index transfer! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &transferFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for mesh vertex and transfer! Error code: " + string_VkResult(result));
		
		// Destroy the transfer fence
		device->GetLoader()->vkDestroyFence(device->GetDevice(), transferFence, &VulkanRenderer::ALLOCATION_CALLBACKS);
	}

	void RenderMesh::GetMeshData(Vertex* vertices, uint32_t* indices) const {
		// Create the vertex staging buffer
		VulkanDevice* device = renderer->GetDevice();

		VulkanBuffer vertexStagingBuffer {
			device,
			(VkDeviceSize)sizeof(Vertex) * vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Create the index staging buffer
		VulkanBuffer indexStagingBuffer {
			device,
			(VkDeviceSize)sizeof(uint32_t) * indexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		// Set the vertex transfer command stage info
		TransferUserData vertexTransferData {
			.srcBuffer = vertexBuffer,
			.dstBuffer = &vertexStagingBuffer
		};

		VulkanCommand::CommandStageInfo vertexTransferStageInfo {
			.name = "VertexCopyTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = vertexBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = &vertexStagingBuffer
					}
				}
			},
			.recordCallback = RecordBufferCopyCallback,
			.userData = &vertexTransferData
		};
		
		// Set the index transfer command stage info
		TransferUserData indexTransferData {
			.srcBuffer = indexBuffer,
			.dstBuffer = &indexStagingBuffer
		};

		VulkanCommand::CommandStageInfo indexTransferStageInfo {
			.name = "IndexCopyTransfer",
			.dependencies = {},
			.resources = {
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_READ_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = indexBuffer
					}
				},
				VulkanCommand::ResourceAccessInfo {
					.type = VulkanCommand::RESOURCE_TYPE_BUFFER,
					.stageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR,
					.accessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT_KHR,
					.bufferAccessInfo = {
						.buffer = &indexStagingBuffer
					}
				}
			},
			.recordCallback = RecordBufferCopyCallback,
			.userData = &indexTransferData
		};

		// Create and record the command
		VulkanCommand transferCommand { device, VulkanCommand::COMMAND_TYPE_TRANSFER };

		transferCommand.AddCommandStage(std::move(vertexTransferStageInfo));
		transferCommand.AddCommandStage(std::move(indexTransferStageInfo));
		transferCommand.RecordCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		// Set the transfer fence create info
		VkFenceCreateInfo fenceInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0
		};

		// Create the transfer fence
		VkFence transferFence;
		VkResult result = device->GetLoader()->vkCreateFence(device->GetDevice(), &fenceInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &transferFence);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create Vulkan fence for mesh vertex and index transfer! Error code: " + string_VkResult(result));

		// Set the submit info
		VkCommandBufferSubmitInfo commandBufferSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			.commandBuffer = transferCommand.GetCommandBuffer(),
			.deviceMask = 0
		};

		VkSubmitInfo2KHR submitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr
		};

		// Submit the command buffer
		device->GetDeviceQueues().transferQueueMutex.lock();
		result = device->GetLoader()->vkQueueSubmit2KHR(device->GetDeviceQueues().transferQueue, 1, &submitInfo, transferFence);
		device->GetDeviceQueues().transferQueueMutex.unlock();
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to submit Vulkan command buffer for mesh vertex and index transfer! Error code: " + string_VkResult(result));
		
		// Wait for the copy to finish
		result = device->GetLoader()->vkWaitForFences(device->GetDevice(), 1, &transferFence, VK_TRUE, UINT64_MAX);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to wait for Vulkan fence for mesh vertex and transfer! Error code: " + string_VkResult(result));
		
		// Destroy the transfer fence
		device->GetLoader()->vkDestroyFence(device->GetDevice(), transferFence, &VulkanRenderer::ALLOCATION_CALLBACKS);

		// Get the vertices and indices from the staging buffers
		std::memcpy(vertices, device->GetAllocator()->GetMappedMemory(vertexStagingBuffer.GetBufferMemory()), sizeof(Vertex) * vertexCount);
		std::memcpy(indices, device->GetAllocator()->GetMappedMemory(indexStagingBuffer.GetBufferMemory()), sizeof(uint32_t) * indexCount);
	}

	RenderMesh::~RenderMesh() {
		// Destroy the vertex and index buffers
		delete vertexBuffer;
		delete indexBuffer;
	}
}