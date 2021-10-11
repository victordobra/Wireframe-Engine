#include "Asset.h"
#include <fstream>

void Asset::SaveToFile(std::string FileLocation) {
	char* AssetData = (char*)this;

	std::ofstream FileOutput(FileLocation, std::ios::out | std::ios::binary);
	if (!FileOutput)
		throw std::runtime_error("No file found at the specified location!");

	FileOutput.write(AssetData, sizeof(*this));
	FileOutput.close();

	if (!FileOutput.good())
		throw std::runtime_error("Error occured at writing time!");
}

void Asset::LoadFromFile(std::string FileLocation) {
	std::ifstream FileInput(FileLocation, std::ios::in | std::ios::binary);
	if(!FileInput)
		throw std::runtime_error("No file found at the specified location!");

	FileInput.read((char*)this, sizeof(*this));
	FileInput.close();

	if (!FileInput.good())
		throw std::runtime_error("Error occured at reading time!");
}

bool Asset::LoadFromExternalFile(std::string FileLocation) {
	return false;
}