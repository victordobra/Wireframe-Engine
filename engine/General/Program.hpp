#pragma once

#include "Platform/Window.hpp"
#include "Renderer/Renderer.hpp"

#include <Core.hpp>

namespace wfe {
	/// @brief A class containing an abstraction for the program and its components.
	class Program {
	public:
		/// @brief Creates the program and its components.
		/// @param argc The number for console arguments given. Defaulted to 0.
		/// @param args The console arguments given, or nullpre if none are present.
		Program(int32_t argc = 0, char_t** args = nullptr);
		Program(const Program&) = delete;
		Program(Program&&) noexcept = delete;

		Program& operator=(const Program&) = delete;
		Program& operator=(Program&&) = delete;

		/// @brief Runs the program.
		/// @return The program's return code.
		int32_t Run();
		/// @brief Closes the program, causing it to stop running after the current update loop.
		/// @param returnCode The program's return code.
		void Close(int32_t returnCode);

		/// @brief Destroys the program and its components.
		~Program();
	private:
		atomic_int32_t running;
		atomic_int32_t returnCode;

		Logger* logger;
		Window* window;
		Renderer* renderer;
	};
}