#include "VulkanRenderer.hpp"
#include "Info/EngineInfo.hpp"

namespace wfe {
	// Public functions
	VulkanRenderer::VulkanRenderer(Program* program) : program(program) {
		// Create the loader
		this->loader = new VulkanLoader();

		// Set the application info
		VkApplicationInfo appInfo {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = program->GetProgramInfo().programName,
			.applicationVersion = VK_MAKE_API_VERSION(0, program->GetProgramInfo().programVersionMajor, program->GetProgramInfo().programVersionMinor, program->GetProgramInfo().programVersionPatch),
			.pEngineName = WFE_ENGINE_NAME,
			.engineVersion = VK_MAKE_API_VERSION(0, WFE_ENGINE_VERSION_MAJOR, WFE_ENGINE_VERSION_MINOR, WFE_ENGINE_VERSION_PATCH),
			.apiVersion = VK_API_VERSION_1_0
		};

		// Create the instance
		this->instance = new VulkanInstance(this->loader, appInfo, program->GetLogger());

		// Create the surface
		this->surface = new VulkanSurface(this->instance, program->GetWindow());
	}

	VulkanRenderer::~VulkanRenderer() {
		// Destroy all renderer components
		delete this->surface;
		delete this->instance;
		delete this->loader;
	}
}