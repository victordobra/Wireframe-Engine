#include "TextFile.hpp"
#include <fstream>

namespace wfe {
	// Virtual function definitions
	void TextFile::Load(std::istream& stream) {
		// Read the file size
		stream.seekg(0, std::ios::end);
		size_t size = (size_t)stream.tellg();
		stream.seekg(0, std::ios::beg);

		// Read the file data
		data.resize(size);
		stream.read(data.data(), size);
	}
	void TextFile::Save(std::ostream& stream) const {
		// Write the file data
		stream.write(data.data(), data.size());
	}
	void TextFile::Import(const std::string& path) {
		// Open the file
		std::ifstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open text file \"" + path + "\" for reading!");
		
		// Read the file size
		stream.seekg(0, std::ios::end);
		size_t size = (size_t)stream.tellg();
		stream.seekg(0, std::ios::beg);

		// Read the file data
		data.resize(size);
		stream.read(data.data(), size);

		// Close the file
		stream.close();
	}
	void TextFile::Export(const std::string& path) const {
		// Open the file
		std::ofstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open text file \"" + path + "\" for writing!");
		
		// Write the file data
		stream.write(data.data(), data.size());

		// Close the file
		stream.close();
	}
}