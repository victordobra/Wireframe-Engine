#pragma once

#include "Core/Types/Defines.hpp"
#include "Graphics/Skybox/Resources/Skybox.hpp"
#include "Graphics/GraphicsSystem.hpp"

namespace wfe {
    class EngineGraphics;
    class Program;

	/// @brief A class implementing the skybox graphics pipeline, used for skybox rendering.
    class SkyboxPipeline {
    public:
        /// @brief Creates a skybox graphics pipeline.
		/// @param engineGraphics The engine graphics component to register the pipeline in.
        SkyboxPipeline(EngineGraphics* engineGraphics);
        SkyboxPipeline(const SkyboxPipeline&) = delete;
        SkyboxPipeline(SkyboxPipeline&&) = delete;

        SkyboxPipeline& operator=(const SkyboxPipeline&) = delete;
        SkyboxPipeline& operator=(SkyboxPipeline&&) = delete;

		/// @brief Gets the program that owns the pipeline.
		/// @return The program that owns the pipeline.
		Program* GetProgram() const {
			return program;
		}
		/// @brief Gets the rendered skybox.
		/// @return The rendered skybox.
		Skybox* GetSkybox() const {
			return skybox;
		}
		/// @brief Gets the Vulkan vertex shader module.
		/// @return The Vulkan vertex shader module's handle.
		VkShaderModule GetVertexShader() const {
			return vertexShader;
		}
		/// @brief Gets the Vulkan fragment shader module.
		/// @return The Vulkan fragment shader module's handle.
		VkShaderModule GetFragmentShader() const {
			return fragmentShader;
		}
		/// @brief Gets the Vulkan pipeline layout of the graphics pipeline.
		/// @return The handle of the Vulkan pipeline layout of the graphics pipeline.
		VkPipelineLayout GetPipelineLayout() const {
			return pipelineLayout;
		}
		/// @brief Gets the Vulkan graphics pipeline.
		/// @return The Vulkan graphics pipeline's handle
		VkPipeline GetPipeline() const {
			return pipeline;
		}

		/// @brief Sets the skybox to be rendered by the pipeline.
		/// @param newSkybox The new skybox to be rendered by the pipeline.
		void SetSkybox(Skybox* newSkybox) {
			skybox = newSkybox;
		}

		/// @brief Gets the command stage info of the skybox graphics pipeline.
		/// @return The command stage info of the skybox graphics pipeline.
		VulkanCommand::CommandStageInfo GetStageInfo();
		/// @brief Records the main pipeline's render commands.
		/// @param commandBuffer The Vulkan secondary command buffer in which are recorded the rendering commands.
		void RecordCommands(VkCommandBuffer commandBuffer);

        /// @brief Destroys the skybox graphics pipeline.
        ~SkyboxPipeline();
    private:
		Program* program;
        Skybox* skybox;

		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
    };
}