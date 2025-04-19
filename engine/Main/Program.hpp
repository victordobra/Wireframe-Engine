#pragma once

#include "Core/Defines.hpp"

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

		/// @brief Destroys the program and its components.
		~Program();
	private:
		atomic_int32_t running;
		atomic_int32_t returnCode;
	};
}