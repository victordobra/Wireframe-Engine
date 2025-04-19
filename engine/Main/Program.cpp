#include "Program.hpp"

namespace wfe {
    // Public functions
	Program::Program(int32_t argc, char_t** args) {
        // Create the program's window
        this->window = new Window(1280, 720, "Wireframe Engine");
    }

	int32_t Program::Run() {
        // Run the program as long as the main window is still open
        while(this->window->IsRunning()) {
            // Poll the window's events
            this->window->PollEvents();
        }

        return 0;
    }

    Program::~Program() {
        // Destroy the program's window
        delete this->window;
    }
}