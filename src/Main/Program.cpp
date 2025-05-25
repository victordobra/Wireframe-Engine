#include "Program.hpp"

namespace wfe {
	// Constants
	const char* const DEFAULT_LOG_FILE = "log.txt";

#if defined(WFE_BUILD_MODE_DEBUG)
	const Logger::MessageLevelMask DEFAULT_LOG_MESSAGE_LEVELS = Logger::MESSAGE_LEVEL_ALL;
	const bool DEFAULT_LOG_CONSOLE_ENABLE = true;
#else
	const Logger::MessageLevelMask DEFAULT_LOG_MESSAGE_LEVELS = Logger::MESSAGE_LEVEL_RELEASE;
	const bool DEFAULT_LOG_CONSOLE_ENABLE = false;
#endif


	// Close event listener
	void* Program::CloseEventListener(void* userData, void* params) {
		// Get the program whose window is closing
		Program* program = (Program*)userData;

		// Close the program
		program->running = 0;

		return nullptr;
	}

	// Public functions
	Program::Program(const ProgramInfo& info, int32_t argc, char** args) : info(info) {
		// Store the start time for engine initialization
		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		// Create the program's logger
		logger = new Logger(DEFAULT_LOG_FILE, DEFAULT_LOG_MESSAGE_LEVELS, DEFAULT_LOG_CONSOLE_ENABLE);
		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, "Initializing " + (std::string)info.programName + ", version " + std::to_string(info.programVersionMajor) + '.' + std::to_string(info.programVersionMinor) + '.' + std::to_string(info.programVersionPatch) + "...");

		// Create the program's window and add the close listener
		window = new Window(200, 200, 1280, 720, info.programName);
		window->GetCloseEvent().AddListener({ CloseEventListener, this });

		// Create all other components
		renderer = new VulkanRenderer(this);
		assetManager = new AssetManager(this);

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
		}

		return 0;
	}

	Program::~Program() {
		// Destroy the program's components
		delete renderer;
		delete assetManager;
		delete window;

		logger->LogMessage(Logger::MESSAGE_LEVEL_INFO, (std::string)info.programName + " was closed successfully.");

		delete logger;
	}
}