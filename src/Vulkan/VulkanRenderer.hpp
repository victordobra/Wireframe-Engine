#pragma once

#include "Core/Types/Defines.hpp"
#include "API/VulkanStructSize.hpp"
#include "Instance/VulkanAllocator.hpp"
#include "Instance/VulkanCommandPool.hpp"
#include "Instance/VulkanDevice.hpp"
#include "Instance/VulkanInstance.hpp"
#include "Instance/VulkanSurface.hpp"
#include "Instance/VulkanSwapChain.hpp"
#include "Loader/VulkanLoader.hpp"
#include "Resources/VulkanBuffer.hpp"
#include "Resources/VulkanCommand.hpp"
#include "Resources/VulkanImage.hpp"

namespace wfe {
	class Program;

	/// @brief A class containing all Vulkan base classes required for a renderer.
	class VulkanRenderer {
	public:
		/// @brief The Vulkan allocation callbacks used by the renderer.
		static const VkAllocationCallbacks ALLOCATION_CALLBACKS;

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
			return program;
		}

		/// @brief Gets the Vulkan function loader used for the renderer.
		/// @return The Vulkan function loader used for the renderer.
		VulkanLoader* GetLoader() const {
			return loader;
		}
		/// @brief Gets the Vulkan instance of the renderer.
		/// @return The Vulkan instance of the renderer.
		VulkanInstance* GetInstance() const {
			return instance;
		}
		/// @brief Gets the Vulkan surface of the renderer.
		/// @return The Vulkan surface of the renderer.
		VulkanSurface* GetSurface() const {
			return surface;
		}
		/// @brief Gets the Vulkan device of the renderer.
		/// @return The Vulkan device of the renderer.
		VulkanDevice* GetDevice() const {
			return device;
		}
		/// @brief Gets the Vulkan swap chain of the renderer.
		/// @return The Vulkan swap chain of the renderer.
		VulkanSwapChain* GetSwapChain() const {
			return swapChain;
		}

		/// @brief Destroys the Vulkan renderer.
		~VulkanRenderer();
	private:
		Program* program;

		VulkanLoader* loader;
		VulkanInstance* instance;
		VulkanSurface* surface;
		VulkanDevice* device;
		VulkanSwapChain* swapChain;
	};
}