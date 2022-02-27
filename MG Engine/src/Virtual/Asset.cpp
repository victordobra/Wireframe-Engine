#include "Asset.h"

#include "Debugger.h"

#include <fstream>
#include <vector>

namespace mge {
	Asset::Asset(const char_t* fileLocation) {
		std::ifstream fileInput(fileLocation, std::ios::binary);
		if (!fileInput)
			OutFatalError("File not found!");

		fileInput.read((char*)this, sizeof(*this));
		fileInput.close();

		if (!fileInput.good())
			OutFatalError("Error occured while reading file!");
	}
	void Asset::SaveToFile(const char_t* fileLocation) const {
		std::ofstream fileOutput(fileLocation);
		if (!fileOutput)
			OutFatalError("File not found!");

		fileOutput.write((char*)this, sizeof(*this));
		fileOutput.close();

		if (!fileOutput.good())
			OutFatalError("Error occured while writing to file!");
	}
}