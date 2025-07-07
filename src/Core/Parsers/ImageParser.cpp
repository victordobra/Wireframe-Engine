#include "ImageParser.hpp"
#include "Core/Memory/Allocator.hpp"

#define STBI_MALLOC(size) wfe::AllocMemory(size)
#define STBI_REALLOC(memory, size) wfe::ReallocMemory(memory, size)
#define STBI_FREE(memory) wfe::FreeMemory(memory)

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#include <stb/stb_image.h>

#define STBIW_MALLOC(size) wfe::AllocMemory(size)
#define STBIW_REALLOC(memory, size) wfe::ReallocMemory(memory, size)
#define STBIW_FREE(memory) wfe::FreeMemory(memory)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO 
#include <stb/stb_image_write.h>

namespace wfe {
	// IO callbacks
	static int32_t ReadCallback(void* user, char* data, int32_t size) {
		// Get the input stream
		std::istream* stream = (std::istream*)user;

		return (int32_t)stream->read((char*)data, (size_t)size).gcount();
	}
	static void SkipCallback(void* user, int32_t n) {
		// Get the input stream
		std::istream* stream = (std::istream*)user;

		stream->seekg(n, std::ios_base::cur);
	}
	static int32_t EOFCallback(void* user) {
		// Get the input stream
		std::istream* stream = (std::istream*)user;

		return (int32_t)stream->eof();
	}
	static void WriteCallback(void* context, void* data, int32_t size) {
		// Get the output stream
		std::ostream* stream = (std::ostream*)context;

		stream->write((char*)data, (size_t)size);
	}

	static const stbi_io_callbacks IO_CALLBACKS {
		.read = ReadCallback,
		.skip = SkipCallback,
		.eof = EOFCallback
	};

	// Public functions
	uint8_t* ReadImageFile(std::istream& stream, uint32_t& width, uint32_t& height) {
		// Load the image
		int32_t x, y, comp;
		uint8_t* memory = stbi_load_from_callbacks(&IO_CALLBACKS, &stream, &x, &y, &comp, STBI_rgb_alpha);
		if(!memory)
			return nullptr;
		
		// Set the image's width and height
		width = (uint32_t)x;
		height = (uint32_t)y;

		return memory;
	}

	bool WritePNGFile(std::ostream& stream, uint32_t width, uint32_t height, uint8_t* data) {
		// Write the image
		return (bool)stbi_write_png_to_func(WriteCallback, &stream, (int32_t)width, (int32_t)height, STBI_rgb_alpha, data, (int32_t)(4 * width));
	}
	bool WriteJPEGFile(std::ostream& stream, uint32_t width, uint32_t height, uint8_t* data) {
		// Write the image
		return (bool)stbi_write_jpg_to_func(WriteCallback, &stream, (int32_t)width, (int32_t)height, STBI_rgb_alpha, data, 90);
	}
	bool WriteBMPFile(std::ostream& stream, uint32_t width, uint32_t height, uint8_t* data) {
		// Write the image
		return (bool)stbi_write_bmp_to_func(WriteCallback, &stream, (int32_t)width, (int32_t)height, STBI_rgb_alpha, data);
	}
}
