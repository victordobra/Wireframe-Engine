#include "VulkanRenderer.hpp"
#include "ProjectInfo.hpp"

namespace wfe {
    // Public functions
    VulkanRenderer::VulkanRenderer(Program* program) : program(program) {
        // Create the loader
        this->loader = new VulkanLoader();

        // Set the application info
        VkApplicationInfo appInfo {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = WFE_PROJECT_NAME,
            .applicationVersion = VK_MAKE_API_VERSION(0, WFE_PROJECT_VERSION_MAJOR, WFE_PROJECT_VERSION_MINOR, WFE_PROJECT_VERSION_PATCH),
            .pEngineName = WFE_ENGINE_NAME,
            .engineVersion = VK_MAKE_API_VERSION(0, WFE_ENGINE_VERSION_MAJOR, WFE_ENGINE_VERSION_MINOR, WFE_ENGINE_VERSION_PATCH),
            .apiVersion = VK_API_VERSION_1_0
        };

        // Create the instance
        this->instance = new VulkanInstance(this->loader, appInfo, program->GetLogger());
    }

    VulkanRenderer::~VulkanRenderer() {
        // Destroy all renderer components
        delete this->instance;
        delete this->loader;
    }
}