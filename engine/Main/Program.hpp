#pragma once

#include "Core/Assets/AssetManager.hpp"
#include "Core/Debug/Logger.hpp"
#include "Core/Types/Defines.hpp"
#include "Platform/Window.hpp"
#include "Vulkan/VulkanRenderer.hpp"

namespace wfe {
	/// @brief A class containing an abstraction for the program and its components.
	class Program {
	public:
		/// @brief Creates the program and its components.
		/// @param argc The number for console arguments given. Defaulted to 0.
		/// @param args The console arguments given, or nullpre if none are present.
		Program(int32_t argc = 0, char** args = nullptr);
		Program(const Program&) = delete;
		Program(Program&&) noexcept = delete;

		Program& operator=(const Program&) = delete;
		Program& operator=(Program&&) = delete;

		/// @brief Runs the program.
		/// @return The program's return code.
		int32_t Run();

		/// @brief Gets the program's logger.
		/// @return The program's logger.
		Logger* GetLogger() const {
			return this->logger;
		}
		/// @brief Gets the program's window.
		/// @return The program's window.
		Window* GetWindow() const {
			return this->window;
		}
		/// @brief Gets the program's Vulkan renderer.
		/// @return The program's Vulkan renderer.
		VulkanRenderer* GetRenderer() const {
			return this->renderer;
		}
		/// @brief Gets the program's asset manager.
		/// @return The program's asset manager.
		AssetManager* GetAssetManager() const {
			return this->assetManager;
		}

		/// @brief Destroys the program and its components.
		~Program();
	private:
		static void* CloseEventListener(void* userData, void* params);

		atomic_uint32_t running = 1;

		Logger* logger;
		Window* window;
		VulkanRenderer* renderer;
		AssetManager* assetManager;
	};
}