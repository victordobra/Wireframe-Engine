#include <WireframeEngine.hpp>
#include "Info/ProjectInfo.hpp"
#include <math.h>

struct ArcballCameraInfo {
	wfe::Program* program;

	float xAngle = 0.0f;
	float yAngle = 0.0f;

	wfe::Vec3f center = wfe::VEC3F_ZERO;
	float radius = 6.0f;
	float cameraVel = 0.002f;
};
struct ObjectRotateInfo {
	wfe::Program* program;
	wfe::Entity rotationEntity;
	float rotationSpeed = wfe::HALF_PI_F;
};

static void* ArcballCameraFrameCallback(void* userData, void* params) {
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
		if(arcballInfo.xAngle > wfe::HALF_PI_F) {
			arcballInfo.xAngle = wfe::HALF_PI_F;
		} else if(arcballInfo.xAngle < -wfe::HALF_PI_F) {
			arcballInfo.xAngle = -wfe::HALF_PI_F;
		}

		// Loop the Y angle
		while(arcballInfo.yAngle > wfe::PI_F)
			arcballInfo.yAngle -= wfe::PI_F * 2.0f;
		while(arcballInfo.yAngle < -wfe::PI_F)
			arcballInfo.yAngle += wfe::PI_F * 2.0f;
	}

	// Calculate the camera's position
	wfe::Vec3f position {
		wfe::Sin(arcballInfo.yAngle) * wfe::Cos(arcballInfo.xAngle),
		wfe::Sin(arcballInfo.xAngle),
		wfe::Cos(arcballInfo.yAngle) * wfe::Cos(arcballInfo.xAngle)
	};
	position = (position * arcballInfo.radius) + arcballInfo.center;

	// Calculate the camera's rotation
	wfe::Quatf rotation = wfe::QuatRotAroundAxis(arcballInfo.yAngle, wfe::VEC3F_UP) * wfe::QuatRotAroundAxis(-arcballInfo.xAngle, wfe::VEC3F_RIGHT);

	// Set the camera's info
	wfe::MainPipeline* mainPipeline = arcballInfo.program->GetEngineGraphics()->GetMainPipeline();
	wfe::MainPipeline::CameraInfo cameraInfo = mainPipeline->GetCameraInfo();

	cameraInfo.pos = position;
	cameraInfo.rot = rotation;

	mainPipeline->SetCameraInfo(cameraInfo);

	return nullptr;
}
static void* ObjectRotateFrameCallback(void* userData, void* params) {
	// Get the object rotate info
	ObjectRotateInfo& rotateInfo = *(ObjectRotateInfo*)userData;

	// Update the given entity's transform by rotating it
	float deltaTime = rotateInfo.program->GetFrameClock().GetPrevTickDuration();
	rotateInfo.program->GetEntityManager()->GetEntityTransform(rotateInfo.rotationEntity).rot *= wfe::QuatRotAroundAxis(rotateInfo.rotationSpeed * deltaTime, wfe::VEC3F_UP);

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
		.maxFramesInFlight = 2,
		.maxEntityCount = 256,
		.maxMaterialCount = 256,
		.maxSkyboxCount = 8
	};

	// Create the program
	wfe::Program* program = new wfe::Program(programInfo, programSettings);

	// Store the start time for test component creation
	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	// Add the arcball camera listener
	ArcballCameraInfo arcballInfo;
	arcballInfo.program = program;
	program->GetFrameStartEvent().AddListener({ ArcballCameraFrameCallback, &arcballInfo });

	// Import the asset directory
	wfe::AssetDirectory* assetDir = new wfe::AssetDirectory(program, "assets/");
	assetDir->Import();

	// Create and set the skybox
	wfe::Skybox* skybox = new wfe::Skybox(program->GetEngineGraphics()->GetSkyboxManager(), (wfe::ImageCubemap*)program->GetAssetManager()->GetAsset(4));
	program->GetEngineGraphics()->GetSkyboxPipeline()->SetSkybox(skybox);

	// Get the render object from the directory
	wfe::RenderObject* renderObject = (wfe::RenderObject*)program->GetAssetManager()->GetAsset(3);

	// Get the mesh and its material
	wfe::RenderMesh* mesh = renderObject->GetItems()[0].mesh;
	wfe::Material* material = renderObject->GetItems()[0].material;

	// Create the renderer parent entity
	wfe::Entity rendererParentEntity = program->GetEntityManager()->CreateEntity();

	program->GetEntityManager()->GetEntityTransform(rendererParentEntity).rot = wfe::QuatRotAroundAxis(wfe::QUARTER_PI_F, wfe::VEC3F_UP);

	// Add the object rotate listener
	ObjectRotateInfo rotateInfo;
	rotateInfo.program = program;
	rotateInfo.rotationEntity = rendererParentEntity;
	program->GetFrameStartEvent().AddListener({ ObjectRotateFrameCallback, &rotateInfo });

	// Create the rendered entities and add their render mesh components
	wfe::size_t meshRendererTypeIndex = program->GetEntityManager()->GetTypeIndex<wfe::MeshRenderer>();

	wfe::Entity meshRendererEntity1 = program->GetEntityManager()->CreateEntity();
	wfe::MeshRenderer* meshRenderer1 = (wfe::MeshRenderer*)program->GetEntityManager()->GetComponentList(meshRendererTypeIndex)->CreateComponent(meshRendererEntity1);

	meshRenderer1->mesh = mesh;
	meshRenderer1->material = material;

	program->GetEntityManager()->GetEntityTransform(meshRendererEntity1).pos = { 1.5f, 0.0f, 0.0f };
	program->GetEntityManager()->SetParent(meshRendererEntity1, rendererParentEntity);

	wfe::Entity meshRendererEntity2 = program->GetEntityManager()->CreateEntity();
	wfe::MeshRenderer* meshRenderer2 = (wfe::MeshRenderer*)program->GetEntityManager()->GetComponentList(meshRendererTypeIndex)->CreateComponent(meshRendererEntity2);

	meshRenderer2->mesh = mesh;
	meshRenderer2->material = material;

	program->GetEntityManager()->GetEntityTransform(meshRendererEntity2).pos = { -1.5f, 0.0f, 0.0f };
	program->GetEntityManager()->SetParent(meshRendererEntity2, rendererParentEntity);

	// Create the sun light entity
	wfe::Entity sunLightEntity = program->GetEntityManager()->CreateEntity();

	wfe::size_t sceneLightTypeIndex = program->GetEntityManager()->GetTypeIndex<wfe::SceneLight>();
	wfe::SceneLight* sunLight = (wfe::SceneLight*)program->GetEntityManager()->GetComponentList(sceneLightTypeIndex)->CreateComponent(sunLightEntity);

	sunLight->lightType = wfe::SceneLight::LIGHT_TYPE_SUN;
	sunLight->sunLightInfo = {
		.lightColor = { 1.0f, 1.0f, 1.0f },
		.lightIntensity = 0.8f
	};

	program->GetEntityManager()->GetEntityTransform(sunLightEntity).rot = wfe::QuatRotAroundAxis(-wfe::QUARTER_PI_F, wfe::VEC3F_UP) * wfe::QuatRotAroundAxis(-wfe::QUARTER_PI_F, wfe::VEC3F_RIGHT);

	// Create the point light entity
	wfe::Entity pointLightEntity = program->GetEntityManager()->CreateEntity();
	wfe::SceneLight* pointLight = (wfe::SceneLight*)program->GetEntityManager()->GetComponentList(sceneLightTypeIndex)->CreateComponent(pointLightEntity);

	pointLight->lightType = wfe::SceneLight::LIGHT_TYPE_POINT;
	pointLight->pointLightInfo = {
		.lightColor = { 0.0f, 1.0f, 0.0f },
		.lightIntensity = 3.0f,
		.constantScaling = 1.0f,
		.linearScaling = 0.7f,
		.quadraticScaling = 1.8f
	};

	program->GetEntityManager()->GetEntityTransform(pointLightEntity).pos = { 3.0f, -3.0f, 0.0f };

	// Set the ambient light color
	program->GetEngineGraphics()->GetMainPipeline()->SetAmbientLightColor({ 0.02f, 0.02f, 0.02f });

	// Store the end time for component creation
	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	// Output the test component creation time
	std::chrono::duration<float> duration = endTime - startTime;
	program->GetLogger()->LogMessage(wfe::Logger::MESSAGE_LEVEL_INFO, (std::string)WFE_PROJECT_NAME + " components initialized successfully in " + std::to_string(duration.count()) + "s.");

	// Run the program
	wfe::int32_t returnCode = program->Run();

	// Destroy all created entities
	program->GetEntityManager()->DestroyEntity(rendererParentEntity);
	program->GetEntityManager()->DestroyEntity(meshRendererEntity1);
	program->GetEntityManager()->DestroyEntity(meshRendererEntity2);
	program->GetEntityManager()->DestroyEntity(sunLightEntity);
	program->GetEntityManager()->DestroyEntity(pointLightEntity);

	// Destroy the skybox
	delete skybox;

	// Destroy the asset directory
	delete assetDir;

	// Destroy the program and exit
	delete program;

	return returnCode;
}