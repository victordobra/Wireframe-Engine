#include "Asset.h"
#include <fstream>

Asset::Asset(std::string FileLocation) {
	std::ifstream FileInput(FileLocation, std::ios::binary);
	if (!FileInput)
		throw std::runtime_error("No file found at the specified location!");

	FileInput.read((char*)this, sizeof(*this));
	FileInput.close();

	if (!FileInput.good())
		throw std::runtime_error("Error occured at reading time!");
}

void Asset::SaveToFile(std::string FileLocation) const {
	std::ofstream FileOutput(FileLocation, std::ios::binary);
	if (!FileOutput)
		throw std::runtime_error("No file found at the specified location!");

	FileOutput.write((char*)this, sizeof(*this));
	FileOutput.close();

	if (!FileOutput.good())
		throw std::runtime_error("Error occured at writing time!");
}
