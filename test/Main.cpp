#include <WireframeEngine.hpp>
#include "Info/ProjectInfo.hpp"
#include <math.h>

struct ArcballCameraInfo {
	wfe::Program* program;

	float xAngle = 0.0f;
	float yAngle = 0.0f;

	wfe::Vector3 center = wfe::Vector3::ZERO;
	float radius = 5.0f;
	float cameraVel = 0.004f;
};

static void* FrameEventCallback(void* userData, void* params) {
	// Get the arcball camera info
	ArcballCameraInfo& arcballInfo = *(ArcballCameraInfo*)userData;

	// Update the camera angle, if the mouse is pressed
	wfe::InputManager* inputManager = arcballInfo.program->GetWindow()->GetInputManager();

	if(inputManager->IsKeyDown(wfe::InputManager::KEY_LEFT_MBUTTON)) {
		// Get the mouse movement and apply it to the camera
		wfe::InputManager::MouseMovement movement = inputManager->GetMouseMovement();

		arcballInfo.xAngle += movement.y * arcballInfo.cameraVel;
		arcballInfo.yAngle += -movement.x * arcballInfo.cameraVel;

		// Clamp the X angle, to avoid the camera turning over
		if(arcballInfo.xAngle > M_PI_2) {
			arcballInfo.xAngle = (float)M_PI_2;
		} else if(arcballInfo.xAngle < -M_PI_2) {
			arcballInfo.xAngle = (float)-M_PI_2;
		}

		// Loop the Y angle
		while(arcballInfo.yAngle > M_PI)
			arcballInfo.yAngle -= (float)M_PI * 2;
		while(arcballInfo.yAngle < -M_PI)
			arcballInfo.yAngle += (float)M_PI * 2;
	}

	// Calculate the camera's position
	wfe::Vector3 position {
		sinf(arcballInfo.yAngle) * cosf(arcballInfo.xAngle),
		sinf(arcballInfo.xAngle),
		cosf(arcballInfo.yAngle) * cosf(arcballInfo.xAngle)
	};
	position = (position * arcballInfo.radius) + arcballInfo.center;

	// Calculate the camera's rotation
	wfe::Quaternion rotation = wfe::Quaternion::AroundAxis(wfe::Vector3::UP, arcballInfo.yAngle) * wfe::Quaternion::AroundAxis(wfe::Vector3::RIGHT, -arcballInfo.xAngle);

	// Set the camera's info
	wfe::MainPipeline* mainPipeline = arcballInfo.program->GetMainPipeline();
	wfe::MainPipeline::CameraInfo cameraInfo = mainPipeline->GetCameraInfo();

	cameraInfo.pos = position;
	cameraInfo.rot = rotation;

	mainPipeline->SetCameraInfo(cameraInfo);

	return nullptr;
}

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
		.windowWidth = 1024,
		.windowHeight = 1024,
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

	// Add the update listener
	ArcballCameraInfo arcballInfo;
	arcballInfo.program = program;
	program->GetFrameEvent().AddListener({ FrameEventCallback, &arcballInfo });

	// Import the asset directory and get all assets
	program->GetAssetManager()->ImportDirectory("assets/");

	// Get the render object from the directory
	wfe::RenderObject* renderObject = (wfe::RenderObject*)program->GetAssetManager()->GetAsset(2);

	// Get the mesh and its material
	wfe::RenderMesh* mesh = renderObject->GetItems()[0].mesh;
	wfe::Material* material = renderObject->GetItems()[0].material;

	// Create the rendered entity and add a render mesh component
	wfe::Entity entity = program->GetEntityManager()->CreateEntity();

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