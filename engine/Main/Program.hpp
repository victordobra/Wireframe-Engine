#pragma once

#include "Core/Types/Defines.hpp"

#include "Platform/Window.hpp"

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

		/// @brief Gets the program's window.
		/// @return The program's window.
		Window* GetWindow() {
			return this->window;
		}
		/// @brief Gets the program's window.
		/// @return The program's window.
		const Window* GetWindow() const {
			return this->window;
		}

		/// @brief Destroys the program and its components.
		~Program();
	private:
		static void* CloseEventListener(void* userData, void* params);

		atomic_uint32_t running = 1;

		Window* window;
	};
}