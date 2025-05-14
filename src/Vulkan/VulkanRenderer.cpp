#include "VulkanRenderer.hpp"
#include "Info/EngineInfo.hpp"

namespace wfe {
	// Public functions
	VulkanRenderer::VulkanRenderer(Program* program) : program(program) {
		// Create the loader
		loader = new VulkanLoader();

		// Set the application info
		VkApplicationInfo appInfo {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = program->GetProgramInfo().programName,
			.applicationVersion = VK_MAKE_API_VERSION(0, program->GetProgramInfo().programVersionMajor, program->GetProgramInfo().programVersionMinor, program->GetProgramInfo().programVersionPatch),
			.pEngineName = WFE_ENGINE_NAME,
			.engineVersion = VK_MAKE_API_VERSION(0, WFE_ENGINE_VERSION_MAJOR, WFE_ENGINE_VERSION_MINOR, WFE_ENGINE_VERSION_PATCH),
			.apiVersion = VulkanInstance::DEFAULT_REQUIRED_INSTANCE_API_VERSION
		};

		// Create the instance
		instance = new VulkanInstance(loader, appInfo, program->GetLogger());

		// Create the surface
		surface = new VulkanSurface(instance, program->GetWindow());

		// Create the device
		device = new VulkanDevice(instance, surface);
	}

	VulkanRenderer::~VulkanRenderer() {
		// Destroy all renderer components
		delete device;
		delete surface;
		delete instance;
		delete loader;
	}
}