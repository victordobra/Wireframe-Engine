#pragma once

#include "Core/Types/Defines.hpp"
#include "Vulkan/VulkanRenderer.hpp"
#include <vector>

namespace wfe {
	class GraphicsPipeline;

	/// @brief A graphics system which manages all rendering using different rendering pipelines.
	class GraphicsSystem {
	public:
		/// @brief Creates a graphics system.
		/// @param renderer The Vulkan renderer to use.
		/// @param maxFramesInFlight The maximum number of frames in flight at a given moment.
		GraphicsSystem(VulkanRenderer* renderer, size_t maxFramesInFlight);
		GraphicsSystem(const GraphicsSystem&) = delete;
		GraphicsSystem(GraphicsSystem&&) = delete;

		GraphicsSystem& operator=(const GraphicsSystem&) = delete;
		GraphicsSystem& operator=(GraphicsSystem&&) = delete;

		/// @brief Gets the Vulkan renderer used by the graphics system.
		/// @return The Vulkan renderer used by the graphics system.
		VulkanRenderer* GetVulkanRenderer() const {
			return renderer;
		}
		/// @brief Gets the maximum number of frames in flight at a given moment.
		/// @return The maximum number of frames in flight at a given moment.
		size_t GetMaxFramesInFlight() const {
			return renderingCommands.size();
		}

		/// @brief Gets the command structures used to hold the rendering commands for all frames in flight.
		/// @return 
		const std::vector<VulkanCommand*> GetRenderingCommands() const {
			return renderingCommands;
		}
		/// @brief Gets the fences used to synchronize rendering. Each fence is signaled when the last frame with its index finished rendering.
		/// @return A vector containing the fences used to synchronize rendering.
		const std::vector<VkFence>& GetRenderingFences() const {
			return renderingFences;
		}
		/// @brief Gets the semaphores used to synchronize rendering. Each semaphore is signaled when the last frame rendering to the image with its index finished rendering.
		/// @return A vector containing the semaphores used to synchronize rendering.
		const std::vector<VkSemaphore>& GetRenderingSemaphores() const {
			return renderingSemaphores;
		}
		/// @brief Gets the semaphores used to synchronize swap chaian access. Each semaphore is signaled when the corresponding frame's image is available.
		/// @return A vector containing the semaphores used to synchronize swap chaian access
		const std::vector<VkSemaphore>& GetImageAvailableSemaphores() const {
			return imageAvailableSemaphores;
		}
		/// @brief Gets the current frame index. Used to access the current frame command pool and syncronization objects.
		/// @return The current frame index.
		uint32_t GetFrameIndex() const {
			return frameIndex;
		}
		/// @brief Gets the index of the currently acquired swap chain image.
		/// @return The index of the currently acquired swap chain image.
		uint32_t GetImageIndex() const {
			return imageIndex;
		}

		/// @brief Gets the command stage info for transitioning the image layout of the current swap chain image. Must be set to depend on every other rendering command.
		/// @param imageIndex The index of the swap chain image whose layout to transition.
		/// @return The command stage info for transitioning the image layout of the current swap chain image. 
		VulkanCommand::CommandStageInfo GetPresentLayoutTransitionStageInfo();

		/// @brief Initiates recording the current frame's rendering command.
		/// @return True if rendering should procees, otherwise false.
		bool InitCommand();
		/// @brief Submits the current frame's rendering command.
		void SubmitCommand();

		/// @brief Destroys the graphics system.
		~GraphicsSystem();
	private:
		VulkanRenderer* renderer;

		std::vector<VulkanCommand*> renderingCommands;
		std::vector<VkFence> renderingFences;
		std::vector<VkSemaphore> renderingSemaphores;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		uint32_t frameIndex = 0, imageIndex = 0;
	};
}