#pragma once
#include <string>

namespace mge {
	void OutMessage(std::string message);
	void OutWarning(std::string warning);
	void OutError(std::string error);
	void OutFatalError(std::string error);
}