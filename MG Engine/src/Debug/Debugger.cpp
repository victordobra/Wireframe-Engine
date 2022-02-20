#include "Debugger.h"
#include "Defines.h"

#include <fstream>

namespace mge {
	enum class OutType : uint8_t {
		MESSAGE,
		WARNING,
		ERROR,
		NONE
	};

	std::ofstream fileOutput;
	OutType lastMessageType = OutType::NONE;
	
	//Internal functions
	void InitiateDebugger() {
		fileOutput.open("log.txt");

		if (!fileOutput)
			throw std::runtime_error("Failed to initialize file output!");
	}
	void ClearDebugger() {
		fileOutput.close();
	}

	//External functions
	void OutMessage(std::string message) {
		if (lastMessageType != OutType::MESSAGE)
			fileOutput << '\n';

		fileOutput << "MESSAGE: " << message << '\n';
		lastMessageType = OutType::MESSAGE;
	}
	void OutWarning(std::string warning) {
		if (lastMessageType != OutType::WARNING)
			fileOutput << '\n';

		fileOutput << "WARNING: " << warning << '\n';
		lastMessageType = OutType::WARNING;
	}
	void OutError(std::string error) {
		if (lastMessageType != OutType::ERROR)
			fileOutput << '\n';

		fileOutput << "ERROR: " << error << '\n';
		lastMessageType = OutType::ERROR;
	}
}