#include "Program.hpp"

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

		// Create all other components
		renderer = new VulkanRenderer(this);
		assetManager = new AssetManager(this);
		entityManager = new EntityManager(settings.maxEntityCount);
		graphicsSystem = new GraphicsSystem(renderer, settings.maxFramesInFlight);
		engineGraphics = new EngineGraphics(this);

		// Store the end time for engine initialization
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

		// Output the initialization time
		std::chrono::duration<float> duration = endTime - startTime;
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)info.programName + " engine initialized successfully in " + std::to_string(duration.count()) + "s.");
	}

	int32_t Program::Run() {
		// Start the frame clock
		frameClock.StartClock();

		// Run the program as long as the main window is still open
		while(running) {
			// Poll the window's events
			window->PollEvents();

			// Trigger the frame start event
			frameStartEvent.TriggerEvent(nullptr);

			// Update all entity transforms
			entityManager->UpdateGlobalTransforms();

			// Render to the window's surface
			engineGraphics->RecordRenderingCommand();

			// Update the frame clock
			frameClock.TickClock();
		}

		// Wait for all rendering to finish
		renderer->GetLoader()->vkDeviceWaitIdle(renderer->GetDevice()->GetDevice());

		return 0;
	}

	Program::~Program() {
		// Destroy the program's components
		delete engineGraphics;
		delete graphicsSystem;
		delete entityManager;
		delete assetManager;
		delete renderer;
		delete window;

		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)info.programName + " was closed successfully.");

		delete logger;
	}
}