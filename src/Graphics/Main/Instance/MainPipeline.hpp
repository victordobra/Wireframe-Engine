#pragma once

#include "Core/Math/General/VecUtils.hpp"
#include "Core/Math/General/QuatUtils.hpp"
#include "Core/Types/Defines.hpp"
#include "Graphics/GraphicsSystem.hpp"

namespace wfe {
	class EngineGraphics;
	class Program;

	/// @brief A class implementing the main graphics pipeline, used for base 3D graphics.
	class MainPipeline {
	public:
		/// @brief An enum containing all camera projection types.
		enum CameraType {
			/// @brief The camera type using perspective projection.
			CAMERA_TYPE_PERSPECTIVE,
			/// @brief The camera type using ortographic projection.
			CAMERA_TYPE_ORTOGRAPHIC
		};
		/// @brief The projection information for a perspective camera.
		struct PerspectiveProjectionInfo {
			/// @brief The camera's vertical field of view, in radians.
			float fov;
			/// @brief The distance from the camera's position to the near clipping plane.
			float nearPlane;
			/// @brief The distance from the camera's position to the far clipping plane.
			float farPlane;
		};
		/// @brief The projection information for an ortographic camera.
		struct OrtographicProjectionInfo {
			/// @brief The distance from the camera's position to the top and bottom clipping planes.
			float viewHeight;
			/// @brief The distance from the camera's position to the near clipping plane.
			float nearPlane;
			/// @brief The distance from the camera's position to the far clipping plane.
			float farPlane;
		};
		/// @brief The camera's information.
		struct CameraInfo {
			/// @brief The camera's position.
			Vec3f pos;
			/// @brief The camera's rotation.
			Quatf rot;
			/// @brief The camera's projection type.
			CameraType cameraType;

			union {
				/// @brief The projection information, if the camera uses perspective projection.
				PerspectiveProjectionInfo perspectiveInfo;
				/// @brief The projection information, if the camera uses ortographic projection.
				OrtographicProjectionInfo ortographicInfo;
			};
		};

		/// @brief Creates a main graphics pipeline.
		/// @param engineGraphics The engine graphics component to register the pipeline in.
		/// @param cameraInfo The camera's starting info.
		MainPipeline(EngineGraphics* engineGraphics, const CameraInfo& cameraInfo);
		MainPipeline(const MainPipeline&) = delete;
		MainPipeline(MainPipeline&&) = delete;

		MainPipeline& operator=(const MainPipeline&) = delete;
		MainPipeline& operator=(MainPipeline&&) = delete;

		/// @brief Gets the program that owns the pipeline.
		/// @return The program that owns the pipeline.
		Program* GetProgram() const {
			return program;
		}
		/// @brief Gets the color of the ambient light.
		/// @return The color of the ambient light.
		Vec3f GetAmbientLightColor() const {
			return ambientLightColor;
		}
		/// @brief Gets the camera's information.
		/// @return The camera's information.
		const CameraInfo& GetCameraInfo() const {
			return cameraInfo;
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

		/// @brief Sets the camera's information.
		/// @param newCameraInfo The camera's new information.
		void SetCameraInfo(const CameraInfo& newCameraInfo) {
			cameraInfo = newCameraInfo;
		}
		/// @brief Sets the color of the ambient light.
		/// @param newAmbientLightColor The new color of the ambient light.
		void SetAmbientLightColor(const Vec3f& newAmbientLightColor) {
			ambientLightColor = newAmbientLightColor;
		}

		/// @brief Gets the command stage info of the main graphics pipeline.
		/// @return The command stage info of the main graphics pipeline.
		VulkanCommand::CommandStageInfo GetStageInfo();
		/// @brief Records the main pipeline's render commands.
		/// @param commandBuffer The Vulkan secondary command buffer in which are recorded the rendering commands.
		void RecordCommands(VkCommandBuffer commandBuffer);

		/// @brief Destroys the main graphics pipeline.
		~MainPipeline();
	private:
		void CreateSceneInfoBuffers();
		void CreateDescriptors();
		void CreatePipeline(EngineGraphics* engineGraphics);

		Program* program;
		CameraInfo cameraInfo;
		Vec3f ambientLightColor;

		std::vector<VulkanBuffer*> sceneInfoBuffers;

		VkDescriptorPool sceneInfoDescriptorPool;
		VkDescriptorSetLayout sceneInfoDescriptorSetLayout;
		std::vector<VkDescriptorSet> sceneInfoDescriptorSets;

		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
	};
}