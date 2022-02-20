#include "Asset.h"
#include <fstream>
#include <vector>

namespace mge {
	Asset::Asset(const char_t* fileLocation) {
		std::ifstream fileInput(fileLocation, std::ios::binary);
		if (!fileInput)
			throw std::runtime_error("File not found!");

		fileInput.read((char*)this, sizeof(*this));
		fileInput.close();

		if (!fileInput.good())
			throw std::runtime_error("Error occured while reading file!");
	}
	void Asset::SaveToFile(const char_t* fileLocation) const {
		std::ofstream fileOutput(fileLocation);
		if (!fileOutput)
			throw std::runtime_error("File not found!");

		fileOutput.write((char*)this, sizeof(*this));
		fileOutput.close();

		if (!fileOutput.good())
			throw std::runtime_error("Error occured while writing to file!");
	}
}