#include <WireframeEngine.hpp>
#include "Info/ProjectInfo.hpp"

int main(int argc, char** args) {
	// Set the program info
	wfe::Program::ProgramInfo programInfo {
		.programName = WFE_PROJECT_NAME,
		.programVersionMajor = WFE_PROJECT_VERSION_MAJOR,
		.programVersionMinor = WFE_PROJECT_VERSION_MINOR,
		.programVersionPatch = WFE_PROJECT_VERSION_PATCH
	};

	// Create and run the program
	wfe::Program* program = new wfe::Program(programInfo, argc, args);

	wfe::int32_t returnCode = program->Run();

	// Destroy the program and exit
	delete program;

	return returnCode;
}