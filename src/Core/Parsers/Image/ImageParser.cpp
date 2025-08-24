#include "ImageParser.hpp"
#include "Core/Memory/Allocator.hpp"

namespace wfe {
	// Public functions
	uint8_t* ReadImageFile(std::istream& stream, uint32_t& width, uint32_t& height) {
		// Check for the file's type and parse it accordingly
		if(CheckPNGSignature(stream)) {
			return ReadPNGFile(stream, width, height);
		} else {
			return nullptr;
		}
	}
}
