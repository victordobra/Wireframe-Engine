#pragma once

#include "Core/Types/Defines.hpp"
#include "Vulkan/Instance/VulkanDevice.hpp"
#include "Vulkan/Instance/VulkanSwapChain.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanImage.hpp"
#include <string>
#include <vector>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	class VulkanCommand {
	public:
		/// @brief The callback called for each stage of the command when recording the command buffer.
		/// @param userData The data passed to the callback when recording. Specified in the stage info.
		/// @param commandBuffer The command buffer to record the current stage's commands to.
		typedef void(*RecordBufferCallback)(void* userData, VkCommandBuffer commandBuffer);

		/// @brief An enum containing all Vulkan command types.
		enum CommandType {
			/// @brief Graphics command type. Run on the Vulkan device's graphics queue.
			COMMAND_TYPE_GRAPHICS,
			/// @brief Present command type. Run on the Vulkan device's present queue.
			COMMAND_TYPE_PRESENT,
			/// @brief Transfer command type. Run on the Vulkan device's transfer queue.
			COMMAND_TYPE_TRANSFER,
			/// @brief Compute command type. Run on the Vulkan device's compute queue.
			COMMAND_TYPE_COMPUTE,
			/// @brief The number of command types.
			COMMAND_TYPE_COUNT
		};
		/// @brief An enum containing all Vulkan resource types.
		enum ResourceType {
			/// @brief Buffer resource type.
			RESOURCE_TYPE_BUFFER,
			/// @brief Image resource type.
			RESOURCE_TYPE_IMAGE,
			/// @brief Swap chain image resource type.
			RESOURCE_TYPE_SWAP_CHAIN_IMAGE
		};

		/// @brief A struct containing the dependency info for accessing a resource.
		struct ResourceAccessInfo {
			/// @brief The type of the current resource.
			ResourceType type;
			/// @brief A mask describing the pipeline stages in which the resource will be accessed.
			VkPipelineStageFlags2 stageMask;
			/// @brief A mask describing the types of access used on this resource. 
			VkAccessFlags2 accessMask;

			union {
				/// @brief A struct containing the access info for a buffer resource.
				struct {
					/// @brief A pointer to the buffer resource.
					VulkanBuffer* buffer;
				} bufferAccessInfo;
				/// @brief A struct containing the access info for an image resource.
				struct {
					/// @brief A pointer to the image resource.
					VulkanImage* image;
					/// @brief The layout the image will be accessed under.
					VkImageLayout layout;
				} imageAccessInfo;
				/// @brief A struct containing the access info for a swap chain image resource.
				struct {
					/// @brief A pointer to the swap chain image resource.
					VulkanSwapChain::SwapChainImage* swapChainImage;
					/// @brief The layout the swap chain image will be accessed under.
					VkImageLayout layout;
				} swapChainImageAccessInfo;
			};
		};
		/// @brief A struct containing information on a command stage.
		struct CommandStageInfo {
			/// @brief The name of the current command stage.
			std::string name;
			/// @brief A vector containing the names of all 
			std::vector<std::string> dependencies;

			/// @brief A vector containing all resource accesses done by the command stage.
			std::vector<ResourceAccessInfo> resources;

			/// @brief The callback used to record to the final command buffer.
			RecordBufferCallback recordCallback;
			/// @brief A pointer to the user data passed to the record callback.
			void* userData;
		};

		/// @brief Creates a Vulkan device command.
		/// @param device The Vulkan device the command is for.
		/// @param commandType The type of the Vulkan command.
		VulkanCommand(VulkanDevice* device, CommandType commandType);
		VulkanCommand(const VulkanCommand&) = delete;
		VulkanCommand(VulkanCommand&&) = delete;

		VulkanCommand& operator=(const VulkanCommand&) = delete;
		VulkanCommand& operator=(VulkanCommand&&) = delete;

		/// @brief Gets the Vulkan command buffer containing the recorded command.
		/// @return The Vulkan command buffer containing the recorded command.
		VkCommandBuffer GetCommandBuffer() const {
			return commandBuffer;
		}

		/// @brief Adds a command stage to the Vulkan command.
		/// @param commandStage A struct containing information regarding the command stage.
		void AddCommandStage(const CommandStageInfo& commandStage) {
			stages.push_back(commandStage);
		}
		/// @brief Adds a command stage to the Vulkan command.
		/// @param commandStage A struct containing information regarding the command stage.
		void AddCommandStage(CommandStageInfo&& commandStage) {
			stages.push_back(commandStage);
		}

		/// @brief Records the current command, including all declared stages.
		/// @param usageFlags A bitmask of Vulkan command buffer usage flags, with which the buffer will be recorded.
		void RecordCommand(VkCommandBufferUsageFlags usageFlags);
		/// @brief Resets the Vulkan command.
		void Reset();

		/// @brief Destroys the Vulkan device command.
		~VulkanCommand();
	private:
		void SortStages(size_t index, const std::unordered_map<std::string, size_t>& nameInds, std::vector<size_t>& stageHeights, std::vector<size_t>& sortedInds);

		VulkanDevice* device;
		CommandType commandType;

		std::vector<CommandStageInfo> stages;
		VkCommandBuffer commandBuffer;
	};
}