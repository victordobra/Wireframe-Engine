#include <WireframeEngine.hpp>

int main(int argc, char** args) {
	// Create and run the program
	wfe::Program* program = new wfe::Program(argc, args);

	wfe::int32_t returnCode = program->Run();

	// Destroy the program and exit
	delete program;

	return returnCode;
}