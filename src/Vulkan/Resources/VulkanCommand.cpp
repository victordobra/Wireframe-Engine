#include "VulkanCommand.hpp"
#include <stdexcept>
#include <unordered_map>
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Internal helper functions
	void VulkanCommand::SortStages(size_t index, const std::unordered_map<std::string, size_t>& nameInds, std::vector<size_t>& stageHeights, std::vector<size_t>& sortedInds) {
		// Exit the function if the current height was already generated
		if(stageHeights[index] != SIZE_T_MAX) {
#if defined(WFE_BUILD_MODE_DEBUG)
			// Check if the current stage's height is greater than the heights of all of its dependencies
			for(const std::string& depName : stages[index].dependencies) {
				// Find the dependency's index
				size_t depIndex = nameInds.at(depName);

				if(stageHeights[depIndex] >= stageHeights[index])
					throw std::invalid_argument("Detected circular dependency in Vulkan command!");
			}
#endif

			return;
		}

		// Set the default stage height
		stageHeights[index] = 0;

		// Make sure that the current height is higher than all of the dependencies' heights
		for(const std::string& depName : stages[index].dependencies) {
			// Find the dependency's index
			size_t depIndex = nameInds.at(depName);

			// Make sure to include the stage's dependency 
			SortStages(depIndex, nameInds, stageHeights, sortedInds);

			// Update the current stage's height, if needed
			if(stageHeights[depIndex] >= stageHeights[index])
				stageHeights[index] = stageHeights[depIndex] + 1;
		}

		// Add the current stage to the sorted vector
		sortedInds.push_back(index);
	}

	// Public functions
	VulkanCommand::VulkanCommand(VulkanDevice* device, CommandType commandType) : device(device), commandType(commandType) {
		// Set the command buffer alloc info
		VkCommandPool commandPools[] {
			device->GetDeviceCommandPools().graphicsCommandPool->GetCommandPool(),
			device->GetDeviceCommandPools().presentCommandPool->GetCommandPool(),
			device->GetDeviceCommandPools().transferCommandPool->GetCommandPool(),
			device->GetDeviceCommandPools().computeCommandPool->GetCommandPool()
		};

		VkCommandBufferAllocateInfo allocInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = commandPools[commandType],
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		// Create the command buffer
		VkResult result = device->GetLoader()->vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, &commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to allocate Vulkan command buffer! Error code: " + string_VkResult(result));
	}

	void VulkanCommand::RecordCommand(VkCommandBufferUsageFlags usageFlags) {
		if(stages.empty())
			return;

		// Find all accessed resources
		std::unordered_map<VulkanBuffer*, size_t> buffers;
		std::unordered_map<VulkanImage*, size_t> images;
		std::unordered_map<VulkanSwapChain::SwapChainImage*, size_t> swapChainImages;

		for(const CommandStageInfo& stage : stages) {
			for(const ResourceAccessInfo& resource : stage.resources) {
				switch(resource.type) {
				case RESOURCE_TYPE_BUFFER:
					buffers.insert({ resource.bufferAccessInfo.buffer, buffers.size() });
					break;
				case RESOURCE_TYPE_IMAGE:
					images.insert({ resource.imageAccessInfo.image, images.size() + swapChainImages.size() });
					break;
				case RESOURCE_TYPE_SWAP_CHAIN_IMAGE:
					swapChainImages.insert({ resource.swapChainImageAccessInfo.swapChainImage, images.size() + swapChainImages.size() });
					break;
				}
			}
		}

		// Map the stage names to their corresponding indices
		std::unordered_map<std::string, size_t> nameInds;

		for(size_t i = 0; i != stages.size(); ++i)
			nameInds.insert({ stages[i].name, i });
		
		// Sort the command stges to validate all dependencies
		std::vector<size_t> stageHeights(stages.size(), SIZE_T_MAX);
		std::vector<size_t> sortedInds;
		sortedInds.reserve(stages.size());

		for(size_t i = 0; i != stages.size(); ++i)
			SortStages(i, nameInds, stageHeights, sortedInds);

		// Initialize the buffer memory barriers
		std::vector<std::vector<VkBufferMemoryBarrier2KHR>> bufferBarriers(stages.size());
		for(size_t i = 0; i != stages.size(); ++i) {
			// Fill the buffer memory barrier structs with default info
			bufferBarriers[i].resize(buffers.size(), VkBufferMemoryBarrier2KHR {
				.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR,
				.pNext = nullptr,
				.srcStageMask = 0,
				.srcAccessMask = 0,
				.dstStageMask = 0,
				.dstAccessMask = 0,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.offset = 0,
				.size = VK_WHOLE_SIZE
			});

			// Assign the buffers to every barrier
			for(const std::pair<VulkanBuffer const*, size_t>& bufferInd : buffers)
				bufferBarriers[i][bufferInd.second].buffer = bufferInd.first->GetBuffer();
		}

		// Initialize the image memory barriers
		std::vector<std::vector<VkImageMemoryBarrier2KHR>> imageBarriers(stages.size());
		for(size_t i = 0; i != stages.size(); ++i) {
			// Fill the image memory barrier structs with default info
			imageBarriers[i].resize(images.size() + swapChainImages.size(), VkImageMemoryBarrier2KHR {
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR,
				.pNext = nullptr,
				.srcStageMask = 0,
				.srcAccessMask = 0,
				.dstStageMask = 0,
				.dstAccessMask = 0,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_UNDEFINED
			});

			// Assign the images to every barrier
			for(const std::pair<VulkanImage const*, size_t>& imageInd : images) {
				imageBarriers[i][imageInd.second].image = imageInd.first->GetImage();
				imageBarriers[i][imageInd.second].subresourceRange = {
					.aspectMask = imageInd.first->GetImageAspectFlags(),
					.baseMipLevel = 0,
					.levelCount = imageInd.first->GetImageMipLevelCount(),
					.baseArrayLayer = 0,
					.layerCount = imageInd.first->GetImageLayerCount()
				};
			}
			for(const std::pair<VulkanSwapChain::SwapChainImage const*, size_t>& swapChainImageInd : swapChainImages) {
				imageBarriers[i][swapChainImageInd.second].image = swapChainImageInd.first->image;
				imageBarriers[i][swapChainImageInd.second].subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				};
			}
		}

		// Assign the access infos
		for(size_t i = 0; i != sortedInds.size(); ++i) {
			size_t stageInd = sortedInds[i];

			for(const ResourceAccessInfo& resource : stages[stageInd].resources) {
				// Find the barrier index
				size_t index;
				switch(resource.type) {
				case RESOURCE_TYPE_BUFFER:
					index = buffers.at(resource.bufferAccessInfo.buffer);
					break;
				case RESOURCE_TYPE_IMAGE:
					index = images.at(resource.imageAccessInfo.image);
					break;
				case RESOURCE_TYPE_SWAP_CHAIN_IMAGE:
					index = swapChainImages.at(resource.swapChainImageAccessInfo.swapChainImage);
					break;
				}

				// Assign the stage and access masks for the current stage
				if(resource.type == RESOURCE_TYPE_BUFFER) {
					bufferBarriers[i][index].dstStageMask |= resource.stageMask;
					bufferBarriers[i][index].dstAccessMask |= resource.accessMask;
					if(i != sortedInds.size() - 1) {
						bufferBarriers[i + 1][index].srcStageMask |= resource.stageMask;
						bufferBarriers[i + 1][index].srcAccessMask |= resource.accessMask;
					}
				} else {
					imageBarriers[i][index].dstStageMask |= resource.stageMask;
					imageBarriers[i][index].dstAccessMask |= resource.accessMask;
					if(i != sortedInds.size() - 1) {
						imageBarriers[i + 1][index].srcStageMask |= resource.stageMask;
						imageBarriers[i + 1][index].srcAccessMask |= resource.accessMask;
					}
				}
			}
		}

		// Propagate the source access masks forward
		for(size_t i = 1; i != stages.size(); ++i) {
			for(size_t j = 0; j != bufferBarriers[i].size(); ++j) {
				bufferBarriers[i][j].srcStageMask |= bufferBarriers[i - 1][j].srcStageMask;
				bufferBarriers[i][j].srcAccessMask |= bufferBarriers[i - 1][j].srcAccessMask;
			}
			for(size_t j = 0; j != imageBarriers[i].size(); ++j) {
				imageBarriers[i][j].srcStageMask |= imageBarriers[i - 1][j].srcStageMask;
				imageBarriers[i][j].srcAccessMask |= imageBarriers[i - 1][j].srcAccessMask;
			}
		}

		// Propagate the destination access masks back
		for(size_t i = stages.size() - 2; i != SIZE_T_MAX; --i) {
			for(size_t j = 0; j != bufferBarriers[i].size(); ++j) {
				bufferBarriers[i][j].dstStageMask |= bufferBarriers[i + 1][j].dstStageMask;
				bufferBarriers[i][j].dstAccessMask |= bufferBarriers[i + 1][j].dstAccessMask;
			}
			for(size_t j = 0; j != imageBarriers[i].size(); ++j) {
				imageBarriers[i][j].dstStageMask |= imageBarriers[i + 1][j].dstStageMask;
				imageBarriers[i][j].dstAccessMask |= imageBarriers[i + 1][j].dstAccessMask;
			}
		}

		// Create the dependecy info structs
		std::vector<VkDependencyInfoKHR> dependencyInfos(stages.size());

		for(size_t i = 0; i != stages.size(); ++i) {
			dependencyInfos[i] = VkDependencyInfoKHR {
				.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR,
				.pNext = nullptr,
				.dependencyFlags = 0,
				.memoryBarrierCount = 0,
				.pMemoryBarriers = nullptr,
				.bufferMemoryBarrierCount = (uint32_t)bufferBarriers[i].size(),
				.pBufferMemoryBarriers = bufferBarriers[i].data(),
				.imageMemoryBarrierCount = (uint32_t)imageBarriers[i].size(),
				.pImageMemoryBarriers = imageBarriers[i].data()
			};
		}

		// Set the command buffer begin info
		VkCommandBufferBeginInfo commandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = usageFlags,
			.pInheritanceInfo = nullptr
		};

		// Begin recording the command buffer
		VkResult result = device->GetLoader()->vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to begin recording Vulkan command buffer! Error code: " + string_VkResult(result));
		
		// Record all command stages, ordered based on their heights
		for(size_t i = 0; i != stages.size(); ++i) {
			// Set the initial image layouts in the image barriers
			for(const std::pair<VulkanImage const*, size_t>& imageInd : images)
				imageBarriers[i][imageInd.second].oldLayout = imageInd.first->GetImageLayout();
			for(const std::pair<VulkanSwapChain::SwapChainImage const*, size_t>& swapChainImageInd : swapChainImages)
				imageBarriers[i][swapChainImageInd.second].oldLayout = swapChainImageInd.first->imageLayout;
			
			// Update the image layout values
			size_t stageInd = sortedInds[i];
		
			for(const ResourceAccessInfo& resource : stages[stageInd].resources) {
				switch(resource.type) {
				case RESOURCE_TYPE_IMAGE:
					resource.imageAccessInfo.image->SetImageLayout(resource.imageAccessInfo.layout);
					break;
				case RESOURCE_TYPE_SWAP_CHAIN_IMAGE:
					resource.swapChainImageAccessInfo.swapChainImage->imageLayout = resource.swapChainImageAccessInfo.layout;
					break;
				}
			}

			// Set the final image layouts in the image barriers
			for(const std::pair<VulkanImage const*, size_t>& imageInd : images)
				imageBarriers[i][imageInd.second].newLayout = imageInd.first->GetImageLayout();
			for(const std::pair<VulkanSwapChain::SwapChainImage const*, size_t>& swapChainImageInd : swapChainImages)
				imageBarriers[i][swapChainImageInd.second].newLayout = swapChainImageInd.first->imageLayout;
			
			// Add the pipeline barrier
			device->GetLoader()->vkCmdPipelineBarrier2KHR(commandBuffer, dependencyInfos.data() + i);

			// Record the current stage
			stages[stageInd].recordCallback(stages[stageInd].userData, commandBuffer);
		}

		// End recording the command buffer
		result = device->GetLoader()->vkEndCommandBuffer(commandBuffer);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to end recording Vulkan command buffer! Error code: " + string_VkResult(result));
	}
	void VulkanCommand::Reset() {
		// Clear the stage vector
		stages.clear();

		// Reset the command buffer
		device->GetLoader()->vkResetCommandBuffer(commandBuffer, 0);
	}

	VulkanCommand::~VulkanCommand() {
		// Free the command buffer
		VkCommandPool commandPools[] {
			device->GetDeviceCommandPools().graphicsCommandPool->GetCommandPool(),
			device->GetDeviceCommandPools().presentCommandPool->GetCommandPool(),
			device->GetDeviceCommandPools().transferCommandPool->GetCommandPool(),
			device->GetDeviceCommandPools().computeCommandPool->GetCommandPool()
		};

		device->GetLoader()->vkFreeCommandBuffers(device->GetDevice(), commandPools[commandType], 1, &commandBuffer);
	}
}