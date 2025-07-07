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
			return renderingFences.size();
		}
		/// @brief Gets the graphics pipelines registered to this system.
		/// @return A vector containing the graphics pipelines registered to this system.
		const std::vector<GraphicsPipeline*> GetPipelines() const {
			return pipelines;
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
		size_t GetFrameIndex() const {
			return frameIndex;
		}

		/// @brief Renders the current frame.
		void Render();

		/// @brief Destroys the graphics system.
		~GraphicsSystem();
	private:
		friend GraphicsPipeline;

		void RegisterPipeline(GraphicsPipeline* pipeline);
		void UnregisterPipeline(GraphicsPipeline* pipeline);

		VulkanRenderer* renderer;
		std::vector<GraphicsPipeline*> pipelines;
		std::vector<VkCommandBuffer> renderingCommandBuffers;
		std::vector<VkFence> renderingFences;
		std::vector<VkSemaphore> renderingSemaphores;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		size_t frameIndex = 0;
	};

	/// @brief An abstract class representing a graphics pipeline, used for rendering.
	class GraphicsPipeline {
	public:
		/// @brief Creates a new graphics pipeline.
		/// @param graphicsSystem The graphics system to register the pipeline in.
		GraphicsPipeline(GraphicsSystem* graphicsSystem) : graphicsSystem(graphicsSystem) {
			// Register the pipeline
			graphicsSystem->RegisterPipeline(this);
		}
		GraphicsPipeline(const GraphicsPipeline&) = delete;
		GraphicsPipeline(GraphicsPipeline&&) = delete;

		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
		GraphicsPipeline& operator=(GraphicsPipeline&&) = delete;

		/// @brief Records the current pipeline's render commands.
		/// @return A Vulkan secondary command buffer in which are recorded the rendering commands.
		virtual VkCommandBuffer RecordCommands() = 0;

		/// @brief Gets the graphics system the pipeline is registered in.
		/// @return The graphics system the pipeline is registered in.
		GraphicsSystem* GetGraphicsSystem() const {
			return graphicsSystem;
		}

		/// @brief Destroys the graphics pipeline
		virtual ~GraphicsPipeline() {
			// Unregister the pipeline
			graphicsSystem->UnregisterPipeline(this);
		}
	private:
		GraphicsSystem* graphicsSystem;
	};
}