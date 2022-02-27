#include "Debugger.h"
#include "Defines.h"
#include "BuildInfo.h"

//Platform specific
#ifdef PLATFORM_WINDOWS
#include "Windows/WindowsInternal.h"
#undef ERROR
#endif

#include <fstream>

namespace mge {
	enum class OutType : uint8_t {
		MESSAGE,
		WARNING,
		ERROR,
		FATAL_ERROR,
		NONE
	};

	std::ofstream fileOutput;
	OutType lastMessageType = OutType::NONE;
	
	//Internal functions
	void InitiateDebugger() {
		fileOutput.open("log.txt");
	}
	void ClearDebugger() {
		fileOutput.close();
	}

	//External functions
	void OutMessage(std::string message) {
		//Add a line break if the previous output type doesn't equal this error type
		if (lastMessageType != OutType::MESSAGE && lastMessageType != OutType::NONE)
			fileOutput << '\n';

		//Output the message
		fileOutput << "MESSAGE: " << message << '\n';
		//Set the message type
		lastMessageType = OutType::MESSAGE;
	}
	void OutWarning(std::string warning) {
		//Add a line break if the previous output type doesn't equal this error type
		if (lastMessageType != OutType::WARNING && lastMessageType != OutType::NONE)
			fileOutput << '\n';

		//Output the warning
		fileOutput << "WARNING: " << warning << '\n';
		lastMessageType = OutType::WARNING;
	}
	void OutError(std::string error) {
		//Add a line break if the previous output type doesn't equal this error type
		if (lastMessageType != OutType::ERROR && lastMessageType != OutType::NONE)
			fileOutput << '\n';

		//Output the error
		fileOutput << "ERROR: " << error << '\n';
		lastMessageType = OutType::ERROR;
	}
	void OutFatalError(std::string error) {
		//Add a line break if the previous output type doesn't equal this error type
		if (lastMessageType != OutType::FATAL_ERROR && lastMessageType != OutType::NONE)
			fileOutput << '\n';

		//Output the error
		fileOutput << "FATAL ERROR: " << error << '\n';
		lastMessageType = OutType::FATAL_ERROR;

		//Terminate the program
#ifdef PLATFORM_WINDOWS
		DestroyWindow(WindowsGetWindowHWND());
#endif
		exit(1);
	}
}