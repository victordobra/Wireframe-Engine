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

	// Create the program
	wfe::Program* program = new wfe::Program(programInfo, programSettings);

	// Import the asset directory and get all assets
	program->GetAssetManager()->ImportDirectory("assets/");

	// Get the render object from the directory
	wfe::RenderObject* renderObject = (wfe::RenderObject*)program->GetAssetManager()->GetAsset(2);

	// Get the mesh and its material
	wfe::RenderMesh* mesh = renderObject->GetItems()[0].mesh;
	wfe::Material* material = renderObject->GetItems()[0].material;

	// Create the rendered entity and add a render mesh component
	wfe::Entity entity = program->GetEntityManager()->CreateEntity();
	program->GetEntityManager()->GetEntityTransform(entity).pos = wfe::Vector3(0.0f, 0.0f, -3.0f);

	wfe::size_t typeIndex = program->GetEntityManager()->GetTypeIndex<wfe::MeshRenderer>();
	wfe::MeshRenderer* meshRenderer = (wfe::MeshRenderer*)program->GetEntityManager()->GetComponentList(typeIndex)->CreateComponent(entity);

	meshRenderer->mesh = mesh;
	meshRenderer->material = material;

	// Run the program
	wfe::int32_t returnCode = program->Run();

	// Destroy the created entity
	program->GetEntityManager()->DestroyEntity(entity);

	// Unload the asset directory
	program->GetAssetManager()->UnloadDirectory("assets/");

	// Destroy the program and exit
	delete program;

	return returnCode;
}