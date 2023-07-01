#include "Engine.hpp"
#include "Platform/Window.hpp"
#include <unistd.h>

namespace wfe {
	// Internal variables
	bool8_t gameRunning = true;
	int32_t gameReturnCode = 0;

	// Public functions
	int32_t RunGame(int32_t argc, char_t** args) {
		// Create the window
		CreateWindow();

		// Run the game loop
		while(gameRunning) {
			// Poll the window's events
			PollWindowEvents();

			// Exit the loop if the game is not running anymore
			if(!gameRunning)
				break;

			sleep(1);
		}

		// Delete the window
		DeleteWindow();

		return gameReturnCode;
	}

	void CloseGame(int32_t returnCode) {
		// Set the final return code and stop the game from running
		gameRunning = false;
		gameReturnCode = returnCode;
	}
}