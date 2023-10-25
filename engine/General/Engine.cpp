#include "Engine.hpp"
#include "Platform/Window.hpp"
#include "Renderer/Renderer.hpp"
#include <unistd.h>

namespace wfe {
	// Internal variables
	bool8_t gameRunning = true;
	int32_t gameReturnCode = 0;

	// Public functions
	int32_t RunGame(int32_t argc, char_t** args) {
		// Create every system
		CreateLogger("log.txt");
		CreateWindow();
		CreateRenderer();

		// Run the game loop
		while(gameRunning) {
			// Poll the window's events
			PollWindowEvents();

			// Exit the loop if the game is not running anymore
			if(!gameRunning)
				break;

			sleep(0.01);
		}

		// Destroy every system
		DestroyRenderer();
		DestroyWindow();
		DestroyLogger();

		return gameReturnCode;
	}

	void CloseGame(int32_t returnCode) {
		// Set the final return code and stop the game from running
		gameRunning = false;
		gameReturnCode = returnCode;
	}
}