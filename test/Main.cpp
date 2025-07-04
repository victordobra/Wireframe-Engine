#include <WireframeEngine.hpp>
#include "Info/ProjectInfo.hpp"

int main(int argc, char** args) {
	// Set the program info
	wfe::Program::ProgramInfo programInfo {
		.programName = WFE_PROJECT_NAME,
		.programVersionMajor = WFE_PROJECT_VERSION_MAJOR,
		.programVersionMinor = WFE_PROJECT_VERSION_MINOR,
		.programVersionPatch = WFE_PROJECT_VERSION_PATCH
	};

	// Set the program settings
	wfe::Program::ProgramSettings programSettings {
		.enableDebug = true,
		.enableConsoleLog = true,
		.logFilePath = "log.txt",
		.logMessageLevels = wfe::Logger::MESSAGE_LEVEL_ALL,
		.enableVsync = true,
		.startMaximized = true,
		.startFullscreen = false,
		.windowX = 128,
		.windowY = 128,
		.windowWidth = 512,
		.windowHeight = 512,
		.requiredVulkanAPIVersion = wfe::VulkanInstance::DEFAULT_REQUIRED_INSTANCE_API_VERSION,
		.requiredVulkanInstanceExtensions = wfe::VulkanInstance::DEFAULT_REQUIRED_INSTANCE_EXTENSIONS,
		.optionalVulkanInstanceExtensions = wfe::VulkanInstance::DEFAULT_OPTIONAL_INSTANCE_EXTENSIONS,
		.vulkanValidationLayers = wfe::VulkanInstance::DEFAULT_VALIDATION_LAYERS,
		.requiredVulkanDeviceFeatures = wfe::VulkanDevice::DEFAULT_REQUIRED_DEVICE_FEATURES,
		.optionalVulkanDeviceFeatures = wfe::VulkanDevice::DEFAULT_OPTIONAL_DEVICE_FEATURES,
		.requiredVulkanDeviceExtensions = wfe::VulkanDevice::DEFAULT_REQUIRED_DEVICE_EXTENSIONS,
		.optionalVulkanDeviceExtensions = wfe::VulkanDevice::DEFAULT_OPTIONAL_DEVICE_EXTENSIONS,
		.maxEntityCount = 256,
		.maxFramesInFlight = 2,
		.maxMaterialCount = 256
	};

	// Create and run the program
	wfe::Program* program = new wfe::Program(programInfo, programSettings);

	wfe::int32_t returnCode = program->Run();

	// Destroy the program and exit
	delete program;

	return returnCode;
}