#pragma once

#include "Core.hpp"

namespace wfe {
	/// @brief Creates the game's window.
	void CreateWindow();
	/// @brief Deletes the game's window.
	void DeleteWindow();
	/// @brief Polls the window's events.
	void PollWindowEvents();
}