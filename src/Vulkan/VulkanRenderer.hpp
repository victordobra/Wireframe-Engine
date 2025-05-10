#pragma once

#include "Core/Types/Defines.hpp"

#include "Instance/VulkanInstance.hpp"
#include "Instance/VulkanSurface.hpp"
#include "Loader/VulkanLoader.hpp"

namespace wfe {
	class Program;

	/// @brief A class containing all Vulkan base classes required for a renderer.
	class VulkanRenderer {
	public:
		/// @brief Creates the Vulkan renderer.
		/// @param program The program owning this renderer.
		VulkanRenderer(Program* program);
		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;

		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

		/// @brief Gets the program which owns this Vulkan renderer.
		/// @return The program which owns this Vulkan renderer.
		Program* GetProgram() const {
			return this->program;
		}

		/// @brief Gets the Vulkan function loader used for the renderer.
		/// @return The Vulkan function loader used for the renderer.
		VulkanLoader* GetLoader() const {
			return this->loader;
		}
		/// @brief Gets the Vulkan instance of the renderer.
		/// @return The Vulkan instance of the renderer.
		VulkanInstance* GetInstance() const {
			return this->instance;
		}
		/// @brief Gets the Vulkan surface of the renderer.
		/// @return The Vulkan surface of the renderer.
		VulkanSurface* GetSurface() const {
			return this->surface;
		}

		/// @brief Destroys the Vulkan renderer.
		~VulkanRenderer();
	private:
		Program* program;

		VulkanLoader* loader;
		VulkanInstance* instance;
		VulkanSurface* surface;
	};
}