#pragma once

#include "Core/Types/Defines.hpp"
#include "Platform/Window.hpp"

#include "Loader/VulkanLoader.hpp"

namespace wfe {
    /// @brief A class containing all Vulkan base classes required for a renderer.
    class VulkanRenderer {
    public:
        /// @brief Creates the Vulkan renderer.
        /// @param window The window to render to.
        VulkanRenderer(Window* window);
        VulkanRenderer(const VulkanRenderer&) = delete;
        VulkanRenderer(VulkanRenderer&&) = delete;

        VulkanRenderer& operator=(const VulkanRenderer&) = delete;
        VulkanRenderer& operator=(VulkanRenderer&&) = delete;

        /// @brief Gets the window the Vulkan renderer is displaying to.
        /// @return The window the Vulkan renderer is displaying to.
        Window* GetWindow() const {
            return this->window;
        }

        /// @brief Gets the Vulkan function loader used for the renderer.
        /// @return The Vulkan function loader used for the renderer.
        VulkanLoader* GetLoader() const {
            return this->loader;
        }

        /// @brief Destroys the Vulkan renderer.
        ~VulkanRenderer();
    private:
        Window* window;

        VulkanLoader* loader;
    };
}