#include "Program.hpp"
#include <math.h>

namespace wfe {
	// Close event listener
	void* Program::CloseEventListener(void* userData, void* params) {
		// Get the program whose window is closing
		Program* program = (Program*)userData;

		// Close the program
		program->running = 0;

		return nullptr;
	}

	// Public functions
	Program::Program(const ProgramInfo& info, const ProgramSettings& settings) : info(info), settings(settings) {
		// Store the start time for engine initialization
		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		// Create the program's logger
		logger = new Logger(settings.logFilePath, settings.logMessageLevels, settings.enableConsoleLog);
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, "Initializing " + (std::string)info.programName + ", version " + std::to_string(info.programVersionMajor) + '.' + std::to_string(info.programVersionMinor) + '.' + std::to_string(info.programVersionPatch) + "...");

		// Create the program's window and add the close listener
		window = new Window(settings.windowX, settings.windowY, settings.windowWidth, settings.windowHeight, info.programName, false, settings.startMaximized, settings.startFullscreen);
		window->GetCloseEvent().AddListener({ CloseEventListener, this });

		// Set the default camera info
		MainPipeline::CameraInfo cameraInfo {
			.pos = Vector3::ZERO,
			.rot = Quaternion::IDENTITY,
			.cameraType = MainPipeline::CAMERA_TYPE_PERSPECTIVE,
			.perspectiveInfo = {
				.fov = M_PI_2,
				.nearPlane = 0.001f,
				.farPlane = 1000.f
			}
		};

		// Create all other components
		renderer = new VulkanRenderer(this);
		assetManager = new AssetManager(this);
		entityManager = new EntityManager(settings.maxEntityCount);
		graphicsSystem = new GraphicsSystem(renderer, settings.maxFramesInFlight);
		materialManager = new MaterialManager(renderer, settings.maxMaterialCount);
		mainPipeline = new MainPipeline(this, cameraInfo);

		// Store the end time for renderer initialization
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

		// Output the initialization time
		std::chrono::duration<float> duration = endTime - startTime;
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)info.programName + " initialized successfully in " + std::to_string(duration.count()) + "s.");
	}

	int32_t Program::Run() {
		// Run the program as long as the main window is still open
		while(running) {
			// Poll the window's events
			window->PollEvents();

			// Render to the window's surface
			graphicsSystem->Render();
		}

		// Wait for all rendering to finish
		renderer->GetLoader()->vkDeviceWaitIdle(renderer->GetDevice()->GetDevice());

		return 0;
	}

	Program::~Program() {
		// Destroy the program's components
		delete mainPipeline;
		delete materialManager;
		delete graphicsSystem;
		delete entityManager;
		delete assetManager;
		delete renderer;
		delete window;

		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)info.programName + " was closed successfully.");

		delete logger;
	}
}