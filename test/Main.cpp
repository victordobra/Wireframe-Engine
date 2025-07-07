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

	// Load the test image
	wfe::ImageTexture* surfaceTex = new wfe::ImageTexture(program);
	surfaceTex->Import("assets/TestImage.png");

	// Create the example material
	wfe::Material* material = new wfe::Material(program->GetMaterialManager(), { wfe::Vector4::ONE }, { surfaceTex });

	// Create the example mesh
	std::vector<wfe::RenderMesh::Vertex> vertices {
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }
	};
	std::vector<wfe::uint32_t> indices {
		0, 1, 2,
		1, 3, 2
	};

	wfe::RenderMesh* mesh = new wfe::RenderMesh(program->GetRenderer(), vertices, indices);

	// Create the rendered entity and add a render mesh component
	wfe::Entity entity = program->GetEntityManager()->CreateEntity();
	program->GetEntityManager()->GetEntityTransform(entity).pos = wfe::Vector3(0.0f, 0.0f, -3.0f);

	wfe::size_t typeIndex = program->GetEntityManager()->GetTypeIndex<wfe::MeshRenderer>();
	wfe::MeshRenderer* meshRenderer = (wfe::MeshRenderer*)program->GetEntityManager()->GetComponentList(typeIndex)->CreateComponent(entity);

	meshRenderer->mesh = mesh;
	meshRenderer->material = material;

	// Run the program
	wfe::int32_t returnCode = program->Run();

	// Destroy the created entity and all other resources
	program->GetEntityManager()->DestroyEntity(entity);

	delete mesh;
	delete material;
	delete surfaceTex;

	// Destroy the program and exit
	delete program;

	return returnCode;
}