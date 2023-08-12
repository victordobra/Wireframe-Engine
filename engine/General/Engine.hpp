#pragma once

#include <Core.hpp>

namespace wfe {
	/// @brief Runs the engine's game.
	/// @param argc The number of arguments given to the program.
	/// @param args The arguments given to the program.
	/// @return The game's final return code.
	int32_t RunGame(int32_t argc = 0, char_t** args = nullptr);

	/// @brief Closes the game.
	/// @param returnCode The final return code of the game.
	void CloseGame(int32_t returnCode);
}