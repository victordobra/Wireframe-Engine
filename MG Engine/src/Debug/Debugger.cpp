#include "Debugger.h"
#include "Defines.h"

#include <fstream>

namespace mge {
	std::ofstream fileOutput;
	
	//Internal functions
	void InitiateDebugger() {
		fileOutput.open("log.txt");
	}
	void ClearDebugger() {
		fileOutput.close();
	}

	//External functions
	void OutMessage(std::string message) {
		fileOutput << "MESSAGE: " << message << '\n';
	}
	void OutWarning(std::string warning) {
		fileOutput << "WARNING: " << warning << '\n';
	}
	void OutError(std::string error) {
		fileOutput << "ERROR: " << error << '\n';
	}
}