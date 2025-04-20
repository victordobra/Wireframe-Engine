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
	Program::Program(int32_t argc, char_t** args) {
        // Create the program's window and add the close listener
        this->window = new Window(200, 200, 1280, 720, "Wireframe Engine");
        this->window->GetCloseEvent().AddListener({ CloseEventListener, this });
    }

	int32_t Program::Run() {
        // Run the program as long as the main window is still open
        while(running) {
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