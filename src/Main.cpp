#include <WireframeEngine.hpp>

int main(int argc, char** args) {
	// Create the program
	wfe::Program* program = wfe::NewObject<wfe::Program>(argc, args);

	// Run the program
	int returnCode = program->Run();

	// Destroy the program
	wfe::DestroyObject(program);

	return returnCode;
}