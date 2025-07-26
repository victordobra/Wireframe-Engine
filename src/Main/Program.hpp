#pragma once

#include "Core/Assets/AssetManager.hpp"
#include "Core/ECS/EntityManager.hpp"
#include "Core/Types/Defines.hpp"
#include "Core/Utils/Logger.hpp"
#include "Graphics/EngineGraphics.hpp"
#include "Graphics/GraphicsSystem.hpp"
#include "Platform/Window.hpp"
#include "Vulkan/VulkanRenderer.hpp"

namespace wfe {
	/// @brief A class containing an abstraction for the program and its components.
	class Program {
	public:
		/// @brief A struct containing the program's general info.
		struct ProgramInfo {
			/// @brief The program's name.
			const char* programName;
			/// @brief The major component of the program's version.
			uint32_t programVersionMajor;
			/// @brief The minor component of the program's version.
			uint32_t programVersionMinor;
			/// @brief The patch component of the program's version.
			uint32_t programVersionPatch;
		};
		/// @brief A struct containing the program's general settings, used for startup.
		struct ProgramSettings {
			/// @brief True if debugging should be enabled, otherwise false.
			bool enableDebug = true;
			/// @brief True if the program should log to the console, otherwise false.
			bool enableConsoleLog = true;
			/// @brief The path to the log file to be used by the program.
			const char* logFilePath = "log.txt";
			/// @brief A bitmask containing the message levels that should be logged by the program's logger.
			Logger::MessageLevelMask logMessageLevels = Logger::MESSAGE_LEVEL_ALL;

			/// @brief True if vertical synchronization (vsync) should be enabled, otherwise false.
			bool enableVsync = true;
			/// @brief True if the window should start maximized, otherwise false.
			bool startMaximized = false;
			/// @brief True if the window should start fullscreened, otherwise false.
			bool startFullscreen = false;
			
			/// @brief The initial X coordinate of the window. Ignored if the window is maximized or fullscreened.
			int32_t windowX = 128;
			/// @brief The initial Y coordinate of the window. Ignored if the window is maximized or fullscreened.
			int32_t windowY = 128;
			/// @brief The initial width of the window. Ignored if the window is maximized or fullscreened.
			uint32_t windowWidth = 512;
			/// @brief The initial height of the window. Ignored if the window is maximized or fullscreened.
			uint32_t windowHeight = 512;

			/// @brief The required Vulkan API version to be used by the program.
			uint32_t requiredVulkanAPIVersion = VulkanInstance::DEFAULT_REQUIRED_INSTANCE_API_VERSION;
			/// @brief A vector containing the required Vulkan instance extensions.
			std::vector<const char*> requiredVulkanInstanceExtensions = VulkanInstance::DEFAULT_REQUIRED_INSTANCE_EXTENSIONS;
			/// @brief A vector containing the optional Vulkan instance extensions.
			std::vector<const char*> optionalVulkanInstanceExtensions = VulkanInstance::DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS;
			/// @brief A vector containing the Vulkana validation layers to enable if debugging is enabled.
			std::vector<const char*> vulkanValidationLayers = VulkanInstance::DEFAULT_VALIDATION_LAYERS;
			/// @brief A struct containing the required Vulkan device features.
			VkPhysicalDeviceFeatures2 requiredVulkanDeviceFeatures = VulkanDevice::DEFAULT_REQUIRED_DEVICE_FEATURES;
			/// @brief A struct containing the optional Vulkan device features.
			VkPhysicalDeviceFeatures2 optionalVulkanDeviceFeatures = VulkanDevice::DEFAULT_OPTIONAL_DEVICE_FEATURES;
			/// @brief A vector containing the required Vulkan device extensions.
			std::vector<const char*> requiredVulkanDeviceExtensions = VulkanDevice::DEFAULT_REQUIRED_DEVICE_EXTENSIONS;
			/// @brief A vector containing the optional Vulkan device extensions.
			std::vector<const char*> optionalVulkanDeviceExtensions = VulkanDevice::DEFAULT_OPTIONAL_DEVICE_EXTENSIONS;
			
			/// @brief The maximum number of frames in flight at a given moment.
			size_t maxFramesInFlight = 2;
			/// @brief The maximum number of entities that can be active at once.
			size_t maxEntityCount = 256;
			/// @brief The maximum number of materials that the renderer can handle.
			size_t maxMaterialCount = 256;
			/// @brief The maximum number of skyboxes that the renderer can handle.
			size_t maxSkyboxCount = 8;
		};

		/// @brief Creates the program and its components.
		/// @param info The program's general information.
		/// @param settings The program's settings, used for startup.
		Program(const ProgramInfo& info, const ProgramSettings& settings);
		Program(const Program&) = delete;
		Program(Program&&) noexcept = delete;

		Program& operator=(const Program&) = delete;
		Program& operator=(Program&&) = delete;

		/// @brief Runs the program.
		/// @return The program's return code.
		int32_t Run();

		/// @brief Gets the program's general information.
		/// @return The program's general information.
		const ProgramInfo& GetProgramInfo() const {
			return info;
		}
		/// @brief Gets the program's settings. They should not be used to query the programa's current status, but rather at startup to configure the program.
		/// @return The program's settings.
		const ProgramSettings& GetProgramSettings() const {
			return settings;
		}

		/// @brief Gets the program's logger.
		/// @return The program's logger.
		Logger* GetLogger() const {
			return logger;
		}
		/// @brief Gets the program's window.
		/// @return The program's window.
		Window* GetWindow() const {
			return window;
		}
		/// @brief Gets the program's Vulkan renderer.
		/// @return The program's Vulkan renderer.
		VulkanRenderer* GetRenderer() const {
			return renderer;
		}
		/// @brief Gets the program's asset manager.
		/// @return The program's asset manager.
		AssetManager* GetAssetManager() const {
			return assetManager;
		}
		/// @brief Gets the program's entity manager.
		/// @return The program's entity manager.
		EntityManager* GetEntityManager() const {
			return entityManager;
		}
		/// @brief Gets the program's graphics system.
		/// @return The program's graphics system.
		GraphicsSystem* GetGraphicsSystem() const {
			return graphicsSystem;
		}
		/// @brief Gets the program's engine graphics components.
		/// @return The program's engine graphics components.
		EngineGraphics* GetEngineGraphics() const {
			return engineGraphics;
		}

		/// @brief Gets the event triggered on every frame update.
		/// @return A reference to the event triggered on every frame update.
		Event& GetFrameEvent() const {
			return frameEvent;
		}

		/// @brief Destroys the program and its components.
		~Program();
	private:
		static void* CloseEventListener(void* userData, void* params);

		ProgramInfo info;
		ProgramSettings settings;
		atomic_uint32_t running = 1;

		Logger* logger;
		Window* window;
		VulkanRenderer* renderer;
		AssetManager* assetManager;
		EntityManager* entityManager;
		GraphicsSystem* graphicsSystem;
		EngineGraphics* engineGraphics;

		mutable Event frameEvent;
	};
}