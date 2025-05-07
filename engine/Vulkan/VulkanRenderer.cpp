#include "VulkanRenderer.hpp"

namespace wfe {
    // Public functions
    VulkanRenderer::VulkanRenderer(Window* window) : window(window) {
        // Create the loader
        this->loader = new VulkanLoader();
    }

    VulkanRenderer::~VulkanRenderer() {
        // Destroy all renderer components
        delete this->loader;
    }
}