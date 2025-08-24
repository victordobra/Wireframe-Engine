#pragma once

#include "Core/Types/Defines.hpp"
#include <istream>
#include <ostream>

namespace wfe {
	/// @brief Checks a binary file's signature to check if it is a PNG image file.
	/// @param stream The input stream of the file. Must be binary.
	/// @return True if the given file is a PNG image file.
	bool CheckPNGSignature(std::istream& stream);
	/// @brief Reads a PNG image file, outputting its raw color values.
	/// @param stream The input stream of the file. Must be binary.
	/// @param width A reference to the variable to which the image's width will be written.
	/// @param height A reference to the variable to which the image's height will be written.
	/// @return A pointer to an allocated array of every pixel's data, stored as sequences of reg, green, blue and alpha values for every pixel. Can be later freed with FreeMemory. Set to nullptr if the image's layout is incorrect.
	uint8_t* ReadPNGFile(std::istream& stream, uint32_t& width, uint32_t& height);
	/// @brief Writes a PNG file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param data A pointer to an array of every pixel's data, stored as sequences of reg, green, blue and alpha values for every pixel.
	/// @return True if the write was successful, otherwise false.
	bool WritePNGFile(std::ostream& stream, uint32_t width, uint32_t height, const uint8_t* data);
}